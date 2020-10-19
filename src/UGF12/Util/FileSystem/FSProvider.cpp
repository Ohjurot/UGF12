#include "pch.h"
#include "FSProvider.h"

// Singelton instance
GxUtil::FS::FSProvider GxUtil::FS::FSProvider::s_instance;

GxUtil::FS::FileSystem* GxUtil::FS::FSProvider::getExecDirFs() {
    return &s_instance.m_fsExec;
}

GxUtil::FS::FileSystem* GxUtil::FS::FSProvider::getAppDirFs() {
    return &s_instance.m_fsApp;
}

GxUtil::FS::FSProvider::FSProvider() {
    // Get exec dir
    WCHAR workDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, workDir);

    // Create filesystem
    m_fsExec = GxUtil::FS::FileSystem(GxUtil::FS::FS_Windows::getInstance(), workDir);

    // Get app dir
    WCHAR appModule[MAX_PATH];
    GetModuleFileName(NULL, appModule, MAX_PATH);

    // Get last slash
    SIZE_T lastSlashPos = 0;
    for (SIZE_T i = 0; i < wcslen(appModule); i++) {
        if (appModule[i] == L'\\') {
            lastSlashPos = i;
        }
    }

    // Set null terminator
    appModule[lastSlashPos] = 0x0;

    // Create filesystem
    m_fsApp = GxUtil::FS::FileSystem(GxUtil::FS::FS_Windows::getInstance(), appModule);
}
