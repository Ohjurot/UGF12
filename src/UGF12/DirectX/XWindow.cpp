#include "pch.h"
#include "XWindow.h"

GxDirect::XWindow::XWindow(std::wstring title, UINT width, UINT height, GxDirect::XContext* ptrContext, GxDirect::XCmdQueue* ptrCmdQue) :
	GxWindow::Window(title, width, height),
	m_ptrCmdQue(ptrCmdQue)
{
	// Create window driver
	m_ptrDriver = new GxDirect::XWndDriver(getHandle(), ptrContext, ptrCmdQue);
}

GxDirect::XWindow::~XWindow(){
	// Check if exit of fullscreen is required
	if (m_bFullscreenCurrent) {
		// Update fullscreen
		setFullscreen(FALSE);
		
		// Selresize
		resizeNow(NULL, NULL);
	}

	// Delete window driver
	delete m_ptrDriver;
}

BOOL GxDirect::XWindow::resizeRequested(){
	// Return if fullscreen should change or resize requried
	return ( (m_bFullscreenCurrent != m_bFullscreenShould) || (m_bRequestResize) );
}

void GxDirect::XWindow::resizeNow(UINT* ptrWidth, UINT* ptrHeight) {
	// Check if shoud and is diverge
	if (m_bFullscreenCurrent != m_bFullscreenShould) {
		// Resize to screen size if fullscreen on
		if (m_bFullscreenShould) {
			// Get nearest monitor
			HMONITOR mon = MonitorFromWindow(getHandle(), MONITOR_DEFAULTTONEAREST);

			// Create monInfo struckt
			MONITORINFO monInfo;
			ZeroMemory(&monInfo, sizeof(monInfo));
			monInfo.cbSize = sizeof(MONITORINFO);

			// Get monitor info
			if (GetMonitorInfo(mon, &monInfo)) {
				// Resize arcording to monitor
				m_ptrDriver->resizeSwapChain(monInfo.rcMonitor.right - monInfo.rcMonitor.left, monInfo.rcMonitor.bottom - monInfo.rcMonitor.top);

				if (ptrWidth) {
					// Notify width and height
					*ptrWidth = monInfo.rcMonitor.right - monInfo.rcMonitor.left;
					*ptrHeight = monInfo.rcMonitor.bottom - monInfo.rcMonitor.top;
				}
			}
			
			// Clear reszize request
			m_bRequestResize = FALSE;
		}
		// Resize to normal later
		else {
			m_bRequestResize = TRUE;
		}
		
		// Set the swap chain fullscreen state
		m_ptrDriver->setFullscreen(m_bFullscreenShould);

		// Update current 
		m_bFullscreenCurrent = m_bFullscreenShould;
	}

	// If resize request is pendign
	if (m_bRequestResize) {
		// Get client rect
		RECT cr;
		if (GetClientRect(getHandle(), &cr)) {
			// Calculate width and height
			UINT width = cr.right - cr.left;
			UINT height = cr.bottom - cr.top;

			// Do not resize on miminize
			if (width) {
				// Resize window driver
				m_ptrDriver->resizeSwapChain(width, cr.bottom - cr.top);

				if (ptrWidth) {
					// Notify width and height
					*ptrWidth = width;
					*ptrHeight = height;
				}
			}

			// Unset flag
			m_bRequestResize = FALSE;
		}
	}


}

void GxDirect::XWindow::setFullscreen(BOOL state){
	// Set should state
	m_bFullscreenShould = state;
}

void GxDirect::XWindow::beginFrame(ID3D12GraphicsCommandList* ptrCmdList) {
	// Redirect to driver
	m_ptrDriver->beginFrame(ptrCmdList);
}

void GxDirect::XWindow::endFrame(ID3D12GraphicsCommandList* ptrCmdList) {
	// Redirect to driver
	m_ptrDriver->endFrame(ptrCmdList);
}

void GxDirect::XWindow::present(BOOL vsync) {
	// Redirect to driver
	m_ptrDriver->present(vsync);
}

BOOL GxDirect::XWindow::handleWindowMessage(LRESULT* ptrResult, HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	// switch on message
	switch (msg) {
		case WM_SIZE:
			{
				// Resize accepted
				m_bRequestResize = TRUE;
				return TRUE;
			}
			break;

		case WM_SETFOCUS:
		case WM_ACTIVATE:
			{
				m_bFullscreenCurrent = m_ptrDriver->isFullscreen();
			}
			break;
	}

	// Super class may handle request
	return GxWindow::Window::handleWindowMessage(ptrResult, wnd, msg, wParam, lParam);
}