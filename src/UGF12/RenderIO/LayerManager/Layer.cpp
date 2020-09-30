#include "pch.h"
#include "Layer.h"

GxRenderIO::LayerStack::Layer::Layer(GxRenderIO::LayerStack::ILayerImpl* ptrImpl, DWORD affinityMask) :
	m_ptrImpl(ptrImpl)
{
	// Call Init on layer
	m_ptrImpl->Init();

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
		// TODO: Signal thread exit

		// Wait for thread (10s)
		if (WaitForSingleObject(m_hThread, 10000) == WAIT_TIMEOUT) {
			// Force thread termination
			#pragma warning( push )
			#pragma warning( disable : 6258)
			TerminateThread(m_hThread, -1);
			#pragma warning( pop ) 
		}
		else {
			// Close thread
			CloseHandle(m_hThread);
		}

		// Unset thread
		m_hThread = NULL;
	}

	// Call shutdown on implementation
	m_ptrImpl->Destroy();
}

DWORD WINAPI GxRenderIO::LayerStack::Layer::ThreadProc(LPVOID threadParam) {
	// Call internal
	return ((GxRenderIO::LayerStack::Layer*)(threadParam))->_internalThreadProc();
}

DWORD GxRenderIO::LayerStack::Layer::_internalThreadProc() {
	// TODO: Wait for Start Collection

	// All ok return NULL
	return 0;
}
