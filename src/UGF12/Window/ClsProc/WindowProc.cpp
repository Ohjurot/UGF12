#include "pch.h"
#include "WindowProc.h"

LRESULT GxWindow::WndProc::wndProcSetup(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // Wait for WM_NCCREATE
    if (msg == WM_NCCREATE) {
        // Get pointer to create info
        CREATESTRUCT* ptrCrStruct = (CREATESTRUCT*)lParam;

        // Store void* to window userdata
        SetWindowLongPtr(wnd, GWLP_USERDATA, (LONG_PTR)ptrCrStruct->lpCreateParams);

        // Redirect window proc to window proc run
        SetWindowLongPtr(wnd, GWLP_WNDPROC, (LONG_PTR)&GxWindow::WndProc::wndProcRun);

        // Redirect to run proc
        return GxWindow::WndProc::wndProcRun(wnd, msg, wParam, lParam);
    }

    // Return default proc
    return DefWindowProc(wnd, msg, wParam, lParam);
}

LRESULT GxWindow::WndProc::wndProcRun(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // Store return value for pointer in IMessageReciver
    LRESULT returnValue = NULL;

    // Get Message reciver
    GxWindow::IMessageReciver* ptrReciver = (GxWindow::IMessageReciver*)GetWindowLongPtr(wnd, GWLP_USERDATA);

    // Check if message reciver handles message
    if (ptrReciver->handleWindowMessage(&returnValue, wnd, msg, wParam, lParam)) {
        // Return the handle lresult
        return returnValue;
    }
    else {
        // Else return default proc
        return DefWindowProc(wnd, msg, wParam, lParam);
    }
}
