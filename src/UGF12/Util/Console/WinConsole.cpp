#include "pch.h"
#include "WinConsole.h"

#if !defined(SHIPPING)
// Instance
GxUtil::WinConsole GxUtil::WinConsole::s_instance;
#endif

GxUtil::WinConsole* GxUtil::WinConsole::getInstance() {
    #if !defined(SHIPPING)
        return &s_instance;
    #else
        return NULL;
    #endif
}

void GxUtil::WinConsole::enableConsole(BOOL showFlag) {
    // Store state
    m_bEnabled = showFlag;

    // Set window
    if (showFlag) {
        ShowWindow(m_hwndConsole, SW_SHOW);
    }
    else {
        ShowWindow(m_hwndConsole, SW_HIDE);
    }
}

GxUtil::WinConsole::WinConsole() {
    // Create console
    AllocConsole();

    // Set console name
    SetConsoleTitle(L"UGF12 - Console");

    // Get console window
    m_hwndConsole = GetConsoleWindow();

    // Get console handles
    m_hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    m_hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

    // Disable close
    EnableMenuItem(GetSystemMenu(m_hwndConsole, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
    
    // Set console mode
    SetConsoleMode(m_hConsoleIn, ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_WINDOW_INPUT);

    // Show console window
    ShowWindow(m_hwndConsole, SW_SHOW);

    // Setup callback
    GxUtil::Console::setNotifyCallback(&GxUtil::WinConsole::ConsoleNotify);
}

GxUtil::WinConsole::~WinConsole(){
    // Hide window
    ShowWindow(m_hwndConsole, SW_HIDE);

    // Destroy console
    FreeConsole();
}

void GxUtil::WinConsole::ConsoleNotify(LPCSTR consoleText, UINT8 messageType) {
    // If not enabled return
    if (!s_instance.m_bEnabled) {
        return;
    }
    
    // Color
    SetConsoleTextAttribute(s_instance.m_hConsoleOut, GxUtil::Console::getInstance()->getWindowsColor(messageType));

    // Write to console
    WriteConsoleA(s_instance.m_hConsoleOut, consoleText, strlen(consoleText), NULL, NULL);
    WriteConsoleA(s_instance.m_hConsoleOut, "\n", 1, NULL, NULL);
}
