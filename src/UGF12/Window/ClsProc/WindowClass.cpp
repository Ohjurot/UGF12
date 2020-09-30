#include "pch.h"
#include "WindowClass.h"

/// <summary>
/// Declare static instance inside WindowClass.obj
/// </summary>
GxWindow::WndClass GxWindow::WndClass::s_instance;

LPCWSTR GxWindow::WndClass::getClassName() {
    // Return name of singelton
    return s_instance.m_className;
}

BOOL GxWindow::WndClass::getInitStatus() {
    // Return if init was ok
    return s_instance.m_initOk;
}

HINSTANCE GxWindow::WndClass::getHInstance() {
    return s_instance.m_hInstance;
}

GxWindow::WndClass::WndClass() {
    // Quer HINSTANCE
    m_hInstance = GetModuleHandle(NULL);

    // Create window desriptior
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));


    // Describe window
    wcex.cbSize             = sizeof(WNDCLASSEX);
    wcex.style              = CS_OWNDC;
    wcex.lpfnWndProc        = &GxWindow::WndProc::wndProcSetup;
    wcex.cbClsExtra         = 0;
    wcex.cbWndExtra         = 0;
    wcex.hInstance          = m_hInstance;
    wcex.hIcon              = NULL;
    wcex.hCursor            = LoadCursor(m_hInstance, IDC_APPSTARTING);
    wcex.hbrBackground      = NULL;
    wcex.lpszMenuName       = NULL;
    wcex.lpszClassName      = m_className;
    wcex.hIconSm            = NULL;

    // Register window class
    if (RegisterClassEx(&wcex)) {
        m_initOk = TRUE;
    }
}
GxWindow::WndClass::~WndClass() {
    // Unregister class if it was registered
    if (m_initOk) {
        UnregisterClass(m_className, m_hInstance);
    }
}
