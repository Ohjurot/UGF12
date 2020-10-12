#include "pch.h"
#include "Window.h"

GxWindow::Window::Window(std::wstring title, UINT width, UINT height) {
	// Create rect
	RECT wr;
	wr.top = 0;
	wr.bottom = height;
	wr.left = 0;
	wr.right = width;

	// Adjust rect
	AdjustWindowRect(&wr, WS_BORDER | WS_OVERLAPPED | WS_SIZEBOX | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
	
	// Create window
	m_hWindow = CreateWindowEx(
		NULL,
		GxWindow::WndClass::getClassName(),
		title.c_str(),
		WS_BORDER | WS_OVERLAPPED | WS_SIZEBOX | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		100,
		100,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		GxWindow::WndClass::getHInstance(),
		this
	);

	// Check if error
	if (m_hWindow == NULL) {
		throw EXEPTION_HR(L"CreateWindowEx(...)", GetLastError());
	}
}

GxWindow::Window::~Window() {
	// If window is not closed close if
	if (m_hWindow) {
		close();
	}
}

void GxWindow::Window::runMessageLoop(INT messageLimit) {
	// Check window
	if (!m_hWindow) {
		throw EXEPTION(L"runMessageLoop(...) on destroyed window");
	}
	
	// Copy int 
	INT left = messageLimit;
	
	// Message buffer
	MSG msg = {};
	
	// While trys are leef and peek message succeed
	while (left != 0 && PeekMessage(&msg, m_hWindow, 0, 0, PM_REMOVE)) {
		// Handle message
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		// Decrement left if if bigger than 0
		if (left > 0) {
			left--;
		}
	}

	// Dispatch XInput keys
	m_keyBind.update();
}

void GxWindow::Window::setWindowVisability(BOOL visable) {
	// Check window
	if (!m_hWindow) {
		throw EXEPTION(L"setWindowVisability(...) on destroyed window");
	}
	
	if (visable) {
		// Show window
		ShowWindow(m_hWindow, SW_SHOW);
	}
	else {
		// Hide window
		ShowWindow(m_hWindow, SW_HIDE);
	}
}

void GxWindow::Window::setIcon(HICON icon) {
	// Check window
	if (!m_hWindow) {
		throw EXEPTION(L"setIcon(...) on destroyed window");
	}

	// Send message for big and small icon
	SendMessage(m_hWindow, WM_SETICON, ICON_BIG, (LPARAM)icon);
	SendMessage(m_hWindow, WM_SETICON, ICON_SMALL, (LPARAM)icon);
}

void GxWindow::Window::close() {
	// Check window
	if (!m_hWindow) {
		throw EXEPTION(L"close(...) on destroyed window");
	}

	// Destroy and unset window
	DestroyWindow(m_hWindow);
	m_hWindow = NULL;
}

BOOL GxWindow::Window::isValid() {
	// Compare if window in not equal to null
	return (m_hWindow != NULL);
}

BOOL GxWindow::Window::isActive() {
	// Return if the window is active
	return m_bIsActive;
}

HWND GxWindow::Window::getHandle() {
	// Return internal window handle
	return m_hWindow;
}

GxIO::KeyState* GxWindow::Window::getKeyControler(){
	// Return pointer to internalss
	return &m_keyBind;
}

BOOL GxWindow::Window::getKeyState(UINT8 vKey) {
	return (((1 << 15)) & GetAsyncKeyState(VK_SPACE));
}

BOOL GxWindow::Window::handleWindowMessage(LRESULT* ptrResult, HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {	
	// Check for message
	switch (msg) {
		case WM_CLOSE:
			{
				// Close window
				close();

				// Handled here
				return TRUE;
			}
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			{
				// Send keydown and return
				m_keyBind.keyDown((KB_KEY)wParam);
				return TRUE;
			}
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			{
				// Send keyup and return
				m_keyBind.keyUp((KB_KEY)wParam);
				return TRUE;
			}
			break;

		case WM_ACTIVATE: 
			{
				// If window gets deactivated clear all keys
				if (wParam == WA_INACTIVE) {
					m_keyBind.clearKeys();
					m_bIsActive = FALSE;
				}
				else if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE) {
					m_bIsActive = TRUE;
				}
			}
			break;

		case WM_KILLFOCUS:
			{
				// Window out of focus (lost keyboard controle)
				m_keyBind.clearKeys();
				m_bIsActive = FALSE;
			}
			break;

		case WM_SETFOCUS:
			{
				// Window in focus
				m_bIsActive = TRUE;
			}
	}
	
	// By defaul nothing is handled
	return FALSE;
}
