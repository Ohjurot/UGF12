#include "pch.h"
#include "Layer.h"

GxRenderIO::LayerStack::Layer::Layer(GxDirect::XContext* ptrContext, UINT width, UINT height, GxRenderIO::LayerStack::ILayerImpl* ptrImpl, DWORD affinityMask) :
	m_ptrImpl(ptrImpl),
	m_framebuffers{
		{ptrContext, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, GxRenderIO::FrameBufferUsage::BUFFER_TEXTURE_RENDER_TARGET},
		{ptrContext, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, GxRenderIO::FrameBufferUsage::BUFFER_TEXTURE_RENDER_TARGET}
	}
{
	// Call Init on layer
	m_ptrImpl->Init();

	#if defined(_DEBUG)
	// Stream name for Buffer 0
	std::wstringstream wss;
	wss << L"Layer \"" << ptrImpl->layer_name << L"\" #0";
	
	// Set buffer 0
	m_framebuffers[0].setName(wss.str().c_str());

	// Stream name for Buffer 1
	wss.str(L"");
	wss << L"Layer \"" << ptrImpl->layer_name << L"\" #1";

	// Set buffer 1
	m_framebuffers[1].setName(wss.str().c_str());
	#endif

	// Create thread
	m_hThread = CreateThread(NULL, 0, GxRenderIO::LayerStack::Layer::ThreadProc, this, CREATE_SUSPENDED, &m_dwThreadId);
	if (!m_hThread) {
		throw EXEPTION_HR(L"CreateThread(...)", GetLastError());
	}

	// Set affinity mask
	if (!SetThreadAffinityMask(m_hThread, affinityMask)) {
		throw EXEPTION_HR(L"SetThreadAffinityMask(...)", GetLastError());
	}

	// Start thread
	if (ResumeThread(m_hThread) == -1) {
		throw EXEPTION_HR(L"ResumeThread(...)", GetLastError());
	}
	
}

GxRenderIO::LayerStack::Layer::~Layer() {
	// If thread is open
	if (m_hThread) {
		// Signal thread exit
		m_bThreadShouldExit = TRUE;

		// Wait for thread (10s)
		if (WaitForSingleObject(m_hThread, 30000) == WAIT_TIMEOUT) {
			// Force thread termination
			#pragma warning( push )
			#pragma warning( disable : 6258)
			TerminateThread(m_hThread, -1);
			#pragma warning( pop ) 
		}

		// Close thread
		CloseHandle(m_hThread);

		// Unset thread
		m_hThread = NULL;
	}

	// Call shutdown on implementation
	m_ptrImpl->Destroy();
}

DWORD WINAPI GxRenderIO::LayerStack::Layer::ThreadProc(LPVOID threadParam) {
	// Gx exeption
	GxExeption exOutput;
	
	// Call internal
	try {
		return ((GxRenderIO::LayerStack::Layer*)(threadParam))->_internalThreadProc();
	}
	catch (GxExeption ex) {
		// Overide output
		exOutput = ex;
	}
	catch (HRESULT hr) {
		// Craft GX Exeption
		exOutput = EXEPTION_HR(L"Catched HRESULT value", hr);
	}
	catch (std::exception ex) {
		// Craft GX Exeption
		std::wstringstream wss;
		wss << L"Catched std::exception: " << GxUtil::StrConverter::ConvertN2W(ex.what()).c_str();
		exOutput = EXEPTION(wss.str());
	}
	catch (...) {
		// Nothing to do
	}

	// Error thread exit
	((GxRenderIO::LayerStack::Layer*)(threadParam))->m_pyThreadExeption.setParameter(exOutput);
	((GxRenderIO::LayerStack::Layer*)(threadParam))->m_pyThreadExeption.startWork();
	return -99;
}

DWORD GxRenderIO::LayerStack::Layer::_internalThreadProc() {
	// While thread should not exit
	while (!m_bThreadShouldExit) {
		// Check for start condition
		if (m_workerPayload.requireStart()) {
			// Extract work
			LayerFrameInfo* ptrInfo = &(m_workerPayload.ptr()->frameInfo);
			GxRenderIO::FrameBuffer* ptrBuffer = m_workerPayload.ptr()->ptrFrameBuffer;
			GxRenderIO::CmdListProxy proxy(m_workerPayload.ptr()->ptrCmdManager);

			// Execute work
			m_ptrImpl->draw(ptrInfo, &proxy, ptrBuffer);

			// Check if execute is required
			if (proxy.isDirty()) {
				proxy.execute();
			}

			// Signal completion
			m_workerPayload.completeWork();
		} else {
			// Pause execution
			_mm_pause();
		}
	}

	// All ok return NULL
	return 0;
}

BOOL GxRenderIO::LayerStack::Layer::getResourceViewForBuffer(D3D12_CPU_DESCRIPTOR_HANDLE srvHandle, UINT bufferIndex) {
	// Check if index is in range
	if (bufferIndex > 1) {
		return FALSE;
	}

	// Check if resource state is as required
	if (m_framebuffers[bufferIndex].getState() != D3D12_RESOURCE_STATE_GENERIC_READ) {
		return FALSE;
	}

	// Create view
	m_framebuffers[bufferIndex].createSRV(srvHandle);

	// OK
	return TRUE;
}

void GxRenderIO::LayerStack::Layer::dispatchFrame(LayerFrameInfo* frameInfo, GxRenderIO::CmdListManger* ptrCmdManager, UINT bufferIndex) {
	// If expetion start is required
	if (m_pyThreadExeption.requireStart()) {
		// Throw exeption
		throw m_pyThreadExeption.getParameter();
	}
	
	// Check if wait is required
	if (!m_workerPayload.isDone()) {
		waitForFrame();
	}

	// Check if index is in range
	if (bufferIndex > 1) {
		throw EXEPTION(L"Buffer index out of Range!");
	}

	// Copy Frame info, cmd manager and buffer
	memcpy(&m_workerPayload.ptr()->frameInfo, frameInfo, sizeof(LayerFrameInfo));
	m_workerPayload.ptr()->ptrCmdManager = ptrCmdManager;
	m_workerPayload.ptr()->ptrFrameBuffer = &m_framebuffers[bufferIndex];

	// Signal work
	m_workerPayload.startWork();
}

void GxRenderIO::LayerStack::Layer::waitForFrame() {
	// Wait for worker
	while (!m_workerPayload.isDone()) {
		// If expetion start is required
		if (m_pyThreadExeption.requireStart()) {
			// Throw exeption
			throw m_pyThreadExeption.getParameter();
		}
		// Pause
		_mm_pause();
	}
}
