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
#include <UGF12/RenderIO/LayerManager/Layer.h>
#include <UGF12/RenderIO/Executing/CmdListManager.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

class Impl : public GxRenderIO::LayerStack::ILayerImpl {
	public:
		Impl() :
			GxRenderIO::LayerStack::ILayerImpl(L"Test Layer")
		{}

		void Init() {

		}

		void Destroy() {

		}
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

		// Startup Async command worker
		GxRenderIO::AsyncCmdExecutor::init(24);

		Impl imp;
		GxRenderIO::LayerStack::Layer* ptrLay = new GxRenderIO::LayerStack::Layer(ptrContext, 1920, 1080, &imp);
		GxRenderIO::CmdListManger* ptrManager = new GxRenderIO::CmdListManger(ptrContext, ptrCmdQue, 24);

		// Do window loop
		while (ptrWindow->isValid()) {
			// Run message loop
			ptrWindow->runMessageLoop();


			ptrLay->waitForFrame();
			ptrLay->dispatchFrame();


			// Que commands
			ptrWindow->beginFrame(ptrCmdList->get());
			ptrWindow->endFrame(ptrCmdList->get());

			// Execute commands
			ptrCmdList->execute();

			// Wait for pending gpu operation of the current frame
			ptrCmdList->wait();

			// Presend
			ptrWindow->present();

			// Resize window if required
			if (ptrWindow->resizeRequested()) {
				ptrCmdQue->flushGpu();
				ptrCmdList->flush();

				// Resize window now
				UINT width = 0;
				UINT height = 0;
				ptrWindow->resizeNow(&width, &height);

				// If size changed
				if (width) {
					// Do sth
				}
			}
		}

		delete ptrLay;
		delete ptrManager;

		// Shutdown Async command worker
		GxRenderIO::AsyncCmdExecutor::destroy();

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