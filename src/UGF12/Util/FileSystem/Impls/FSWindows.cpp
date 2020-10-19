#include "pch.h"
#include "FSWindows.h"

// Implement static isntance
GxUtil::FS::FS_Windows GxUtil::FS::FS_Windows::s_instance;

GxUtil::FS::IFileSystemImpl* GxUtil::FS::FS_Windows::getInstance() {
    return &s_instance;
}

UGF12::File GxUtil::FS::FS_Windows::openFile(LPCWSTR path, WORD openFlags) {
    // Access mode
    DWORD accessMode = 0x0;
    if (openFlags & UGF12_FILE_OPEN_READ) {
        accessMode |= GENERIC_READ;
    }
    if (openFlags & UGF12_FILE_OPEN_WRITE) {
        accessMode |= GENERIC_WRITE;
    }

    // Creation mode
    DWORD creationMode = 0x0;
    if (openFlags & UGF12_FILE_OPEN_EXISTING) {
        creationMode |= OPEN_EXISTING;
    }
    if (openFlags & UGF12_FILE_OPEN_NEW) {
        creationMode |= CREATE_NEW;
    }
    if (openFlags & UGF12_FILE_OPEN_ALLWAYS) {
        creationMode |= OPEN_ALWAYS;
    }

    // Create file
    HANDLE hFile = CreateFileW(path, accessMode, NULL, NULL, creationMode, FILE_ATTRIBUTE_NORMAL, NULL);

    // Return 0 if invalid
    if (hFile == INVALID_HANDLE_VALUE) {
        return 0;
    }

    // Return file (interpreted as uint)
    return (UGF12::File)hFile;
}

void GxUtil::FS::FS_Windows::closeFile(UGF12::File file) {
    // Close handle
    CloseHandle((HANDLE)file);
}

SIZE_T GxUtil::FS::FS_Windows::getFileSize(UGF12::File file) {
    // Get file size
    LARGE_INTEGER fileSize;
    fileSize.LowPart = GetFileSize((HANDLE)file, (DWORD*)&fileSize.HighPart);

    // Return
    return fileSize.QuadPart;
}

void GxUtil::FS::FS_Windows::setFilePointer(UGF12::File file, WORD mode, SIZE_T value) {
    // Insert into large int
    LARGE_INTEGER moveDistance;
    moveDistance.QuadPart = value;

    // Set file pointer
    SetFilePointer((HANDLE)file, moveDistance.LowPart, &moveDistance.HighPart, mode);
}

DWORD GxUtil::FS::FS_Windows::readFile(UGF12::File file, DWORD numberOfBytesToRead, void* ptrBuffer) {
    // Read file and store read amount
    DWORD read = 0;
    if (ReadFile((HANDLE)file, ptrBuffer, numberOfBytesToRead, &read, NULL)) {
        // Return number of bytes read
        return read;
    }

    // Failed return 0
    return 0;
}

DWORD GxUtil::FS::FS_Windows::writeFile(UGF12::File file, DWORD numerOfByteToWrite, void* ptrBuffer) {
    // Write file and store write amount
    DWORD written = 0;
    if (WriteFile((HANDLE)file, ptrBuffer, numerOfByteToWrite, &written, NULL)) {
        // Return number of bytes written
        return written;
    }

    // Failed return 0
    return 0;
}
