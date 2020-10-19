#include "pch.h"
#include "FileSystem.h"

GxUtil::FS::FileSystem::FileSystem() {
    m_uiPathLength = 0;
    m_ptrImpl = nullptr;
}

GxUtil::FS::FileSystem::FileSystem(GxUtil::FS::IFileSystemImpl* ptrFileSystemImpl, LPCWSTR basePath) {
    // Get length 
    m_uiPathLength = wcslen(basePath);

    // Copy string
    wcscpy_s(m_strBasePath, m_uiPathLength + 1, basePath);

    // Set impl
    m_ptrImpl = ptrFileSystemImpl;

    // Check ending slash
    if (m_strBasePath[m_uiPathLength - 1] != L'\\') {
        if (m_uiPathLength + 2 <= MAX_PATH) {
            m_uiPathLength++;
            m_strBasePath[m_uiPathLength - 1] = L'\\';
            m_strBasePath[m_uiPathLength] = 0x0;
        }
        else {
            throw EXEPTION(L"Maximum path length exeeded!");
        }
    }
}

GxUtil::FS::FileSystem::FileSystem(GxUtil::FS::FileSystem* ptrParentFileSystem, LPCWSTR subPath) {
    // Get length 
    m_uiPathLength = ptrParentFileSystem->m_uiPathLength;

    // Copy string
    wcscpy_s(m_strBasePath, ptrParentFileSystem->m_uiPathLength + 1, ptrParentFileSystem->m_strBasePath);

    // Set impl
    m_ptrImpl = ptrParentFileSystem->m_ptrImpl;

    // Combine string
    createPath(subPath);

    // Compute length again
    m_uiPathLength = wcslen(m_strBasePath);

    // Check ending slash
    if (m_strBasePath[m_uiPathLength - 1] != L'\\') {
        if (m_uiPathLength + 2 <= MAX_PATH) {
            m_uiPathLength++;
            m_strBasePath[m_uiPathLength - 1] = L'\\';
            m_strBasePath[m_uiPathLength] = 0x0;
        }
        else {
            throw EXEPTION(L"Maximum path length exeeded!");
        }
    }
}

GxUtil::FS::IFileSystemImpl* GxUtil::FS::FileSystem::getImpl() {
    return m_ptrImpl;
}

LPCWSTR GxUtil::FS::FileSystem::createPath(LPCWSTR file) {
    // Compute length
    SIZE_T inputLen = wcslen(file) + 1;

    // Check if too long
    if (m_uiPathLength + inputLen > MAX_PATH) {
        throw EXEPTION(L"Maximum path length exeeded!");
    }

    // Copy into internal buffer
    wcscpy_s(&m_strBasePath[m_uiPathLength], inputLen + 1, file);

    // Return internal
    return m_strBasePath;
}
