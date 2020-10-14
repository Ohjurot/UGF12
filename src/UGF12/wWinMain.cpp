#include "pch.h"

#include <UGF12/Util/CmdArgs.h>
#include <UGF12/Util/StrConverter.h>
#include <UGF12/Util/Time/HPC.h>
#include <UGF12/Util/Time/StopWatch.h>

#include <UGF12/DirectX/XContext.h>
#include <UGF12/DirectX/XFeature.h>
#include <UGF12/DirectX/XWindow.h>
#include <UGF12/DirectX/XWndDriver.h>
#include <UGF12/DirectX/XCmdQueue.h>
#include <UGF12/DirectX/XCmdList.h>

#include <UGF12/RenderIO/Executing/AsyncCmdExecutor.h>

#include <UGF12/RenderIO/LayerManager/ILayerImpl.h>
#include <UGF12/RenderIO/LayerManager/LayerStackManager.h>

#include <UGF12/Layers/DebugUI/DebugUILayer.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

class Impl : public GxRenderIO::LayerStack::ILayerImpl {
	public:
		Impl(GxDirect::XContext* ptrContext) :
			GxRenderIO::LayerStack::ILayerImpl(L"Test Layer"),
			m_ptrContext(ptrContext)
		{}

		void Init() {
			m_ptrContext->IncRef();

			ID3D12Device* pDev;
			m_ptrContext->getDevice(&pDev);

			D3D12_DESCRIPTOR_HEAP_DESC desc;
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			desc.NumDescriptors = 1;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			desc.NodeMask = NULL;

			pDev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_ptrRtvHeap));
			
			COM_RELEASE(pDev);
		}

		void Destroy() {
			m_ptrContext->DecRef();
			COM_RELEASE(m_ptrRtvHeap);
		}

		void draw(GxRenderIO::LayerStack::LayerFrameInfo* ptrFrameInfo, GxRenderIO::CmdListProxy* ptrCmdListProxy, GxRenderIO::FrameBuffer* ptrFrameBuffer) {
			// Create RTV for buffer
			ptrFrameBuffer->barrier(D3D12_RESOURCE_STATE_RENDER_TARGET, ptrCmdListProxy->get());

			// Clear Color (TEMP)
			const static FLOAT arr[] = {
				1.0f,
				0.0f,
				0.0f,
				1.0f
			};

			// Create RTV Handle
			D3D12_CPU_DESCRIPTOR_HANDLE handle = m_ptrRtvHeap->GetCPUDescriptorHandleForHeapStart();

			ptrCmdListProxy->get()->OMSetRenderTargets(1, &handle, 0, NULL);
			ptrCmdListProxy->get()->ClearRenderTargetView(handle, arr, 0, NULL);
			ptrFrameBuffer->barrier(D3D12_RESOURCE_STATE_GENERIC_READ, ptrCmdListProxy->get());
		}
		
		void onResourceChange(UINT resource, UINT index, void* ptrResource) {
			switch (resource) {
				case UGF12_RESOURCE_TYPE_LAYER_FRAMEBUFFER:
					// Cast to buffer
					GxRenderIO::FrameBuffer* ptrBuffer = (GxRenderIO::FrameBuffer*)ptrResource;
					
					// Create handle
					D3D12_CPU_DESCRIPTOR_HANDLE hRtv = m_ptrRtvHeap->GetCPUDescriptorHandleForHeapStart();
					hRtv.ptr += (SIZE_T)m_ptrContext->getIncrmentRtv() * index;
					
					// Create View
					ptrBuffer->createRTV(hRtv);
					break;
			}
		}

	private:
		GxDirect::XContext* m_ptrContext;

		ID3D12DescriptorHeap* m_ptrRtvHeap = NULL;
};


/// <summary>
/// Windows entry point
/// </summary>
/// <param name="hInstance">HINSTANCE for the application</param>
/// <param name="hPrevInstance">Invalid no longer used in this version of windows</param>
/// <param name="cmdArgs">User cmd line args</param>
/// <param name="nCmdShow">Hint for the programmen on how the windows should be created</param>
/// <returns>Programm return code</returns>
INT WINAPI wWinMain(HINSTANCE _In_ hInstance, HINSTANCE _In_opt_ hPrevInstance, LPWSTR _In_ cmdArgs, INT _In_ nCmdShow) {
	try {
		// Init COM
		HRESULT hrCom = CoInitialize(NULL);
		if (FAILED(hrCom)) {
			throw EXEPTION_HR(L"CoInitialize(...)", hrCom);
		}

		// Startup Async command worker
		GxRenderIO::AsyncCmdExecutor::init(48);

		// Parse cmdArgs
		GxUtil::CmdArgs cmd(cmdArgs);

		// Create directx context
		BOOL debugLayer = FALSE;
		#ifdef _DEBUG
			if (cmd.exitst(L"rtDebug") && cmd[L"rtDebug"] == L"true") {
				debugLayer = TRUE;
			}
		#endif
		GxDirect::XContext* ptrContext = new GxDirect::XContext(L"", -1, debugLayer);

		// Create cmd que
		GxDirect::XCmdQueue* ptrCmdQue = new GxDirect::XCmdQueue(ptrContext);

		// Create cmd list
		GxDirect::XCmdList* ptrCmdList = new GxDirect::XCmdList(ptrContext, ptrCmdQue);

		// Create window
		GxDirect::XWindow* ptrWindow = new GxDirect::XWindow(L"My Test window", 1920, 1080, ptrContext, ptrCmdQue);

		// Load and set icon
		HICON ico = (HICON)LoadImage(hInstance, L"./icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR);
		ptrWindow->setIcon(ico);

		// Show window
		ptrWindow->setWindowVisability(TRUE);

		// Create layerstack
		GxRenderIO::LayerStack::Manager* ptrLayManager = new GxRenderIO::LayerStack::Manager(ptrContext, 24, 1920, 1080);


		// DEBUG
		Impl imp(ptrContext);
		UGF12::DebugUI::DebugUILayer debugUiLayer(ptrContext, ptrWindow);

		ptrLayManager->insertLayer(&imp);
		ptrLayManager->insertLayer(&debugUiLayer);
		ptrWindow->setUILayer(&debugUiLayer);
		// DEBUG END


		// Finish layer stack manger
		ptrLayManager->init(ptrCmdList);

		// Do window loop
		while (ptrWindow->isValid()) {
			// Run message loop
			ptrWindow->runMessageLoop();

			// Run layer stack
			ptrLayManager->execute(ptrWindow, ptrCmdList);

			// Resize window if required
			if (ptrWindow->resizeRequested()) {
				// Wait for que and list
				ptrCmdQue->flushGpu();
				ptrCmdList->flush();

				// Resize window now
				UINT width = 0;
				UINT height = 0;
				ptrWindow->resizeNow(&width, &height);

				// If size changed
				if (width) {
					// Resize layer stack
					ptrLayManager->flushAndResize(width, height);
				}
			}

			// ptrLayManager->setLayerEnabled(1, FALSE);
		}

		// Delete layerstack
		delete ptrLayManager;

		// Destroy window
		delete ptrWindow;

		// Destroy list
		delete ptrCmdList;

		// Destroy que
		delete ptrCmdQue;

		// Destroy directx context
		delete ptrContext;

		// Destroy icon
		DestroyIcon(ico);

		// Shutdown Async command worker
		GxRenderIO::AsyncCmdExecutor::destroy();

		// Shutdown COM
		CoUninitialize();
	}
	catch (GxExeption ex) {
		// Build error message
		std::wstringstream wss;
		wss << ex.what << std::endl;
		wss << L"Error: " << GxUtil::StrConverter::TranslateHRESULT(ex.hr).c_str() << " (Code 0x" << std::hex << ex.hr << std::dec << L")" << std::endl;
		wss << L"File: " << GxUtil::StrConverter::ConvertN2W(ex.file).c_str() << std::endl;
		wss << L"Line: " << ex.line << std::endl;

		// Show message
		MessageBox(NULL, wss.str().c_str(), L"Exeption during application runtime", MB_OK | MB_ICONERROR);

		// Failed
		return -1;
	}

	// Return ZERO all ok
	return 0;
}