#include "pch.h"
#include "Console.h"

// Singelton instance
GxUtil::Console* GxUtil::Console::s_ptrInstance = NULL;
GxUtil::ConsoleCallback GxUtil::Console::s_fptrNotifyCallback = NULL;

GxUtil::Console* GxUtil::Console::getInstance() {
    return s_ptrInstance;
}

void GxUtil::Console::setNotifyCallback(ConsoleCallback fptrCallback) {
    s_fptrNotifyCallback = fptrCallback;
}

void GxUtil::Console::Init(SIZE_T bufferSize, UINT8 messageThreshold) {
    // check if existing
    if (s_ptrInstance) {
        throw EXEPTION(L"Console was already inited");
    }

    // check buffer size
    if (bufferSize < 512) {
        throw EXEPTION(L"Minimum buffer size is 512 Chars");
    }

    // Init pointer
    s_ptrInstance = new GxUtil::Console(bufferSize, messageThreshold);
}

void GxUtil::Console::Shutdown() {
    // If not existing
    if (!s_ptrInstance) {
        throw EXEPTION(L"Console was not inited");
    }

    // Delete console
    delete s_ptrInstance;
    s_ptrInstance = NULL;
}

void GxUtil::Console::writeString(LPCSTR strMessage, UINT8 messageType) {
    // Lock this function
    GxUtil::LockGuard janitor(&m_lock);

    // Check length
    if (!*strMessage) {
        return;
    }

    // Assert message type
    if (!messageType) {
        messageType = UGF12_CONSOLE_MESSAGETYPE_DEBUG;
    }

    // Treshold 
    if (messageType < m_mesageThreshold) {
        return;
    }

    // Get time
    SYSTEMTIME now;
    ZeroMemory(&now, sizeof(SYSTEMTIME));
    GetLocalTime(&now);

    // Create string
    std::stringstream ss;
    if (messageType >= UGF12_CONSOLE_MESSAGETYPE_DEBUG) {
        ss << "[" << std::setw(2) << std::setfill('0') << now.wHour << ":" << std::setw(2) << now.wMinute << ":" << std::setw(2) << now.wSecond << std::setw(0) << "] ";
    }

    // Mode
    if (messageType <= UGF12_CONSOLE_MESSAGETYPE_NONE) {
        // None
    }
    else if (messageType <= UGF12_CONSOLE_MESSAGETYPE_DEBUG) {
        ss << "[DEBUG] ";
    }
    else if (messageType <= UGF12_CONSOLE_MESSAGETYPE_INFO) {
        ss << "[INFO] ";
    }
    else if (messageType <= UGF12_CONSOLE_MESSAGETYPE_WARNING) {
        ss << "[WARNING] ";
    }
    else if (messageType <= UGF12_CONSOLE_MESSAGETYPE_ERROR) {
        ss << "[ERROR] ";
    }
    else if (messageType <= UGF12_CONSOLE_MESSAGETYPE_EXEPTION) {
        ss << "[EXEPTION] ";
    }

    // Add text
    ss << strMessage;
    std::string str = ss.str();

    // Check length an copy
    if (str.length() > ((m_size / 3) * 2)){
        throw EXEPTION(L"Maximum message size is 2/3 of buffer size");
    }

    // Write message type
    writeAndIncrmentSafe(messageType);

    // Copy string
    for (CHAR* ptrString = str.data(); *ptrString != 0x0; ptrString++) {
        writeAndIncrmentSafe(*ptrString);
    }

    // Write terminator
    writeAndIncrmentSafe(0x0);

    // Call the callback
    if (s_fptrNotifyCallback) {
        s_fptrNotifyCallback(str.c_str(), messageType);
    }
}

void GxUtil::Console::sendCommand(LPCSTR command) {
    // TODO: Find first space
    // TODO: Gennerate hash for left part
    // TODO: Copy right part
    // TODO: Gennerate Event ( Type = CMD; Filter = HASH; Param* = (LPCSTR)ARGS )
}

DWORD GxUtil::Console::getWindowsColor(UINT8 messageType) {
    // If on type
    if (messageType <= UGF12_CONSOLE_MESSAGETYPE_DEBUG) {
        return (FOREGROUND_INTENSITY);
    }
    else if (messageType <= UGF12_CONSOLE_MESSAGETYPE_INFO) {
        return (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    else if (messageType <= UGF12_CONSOLE_MESSAGETYPE_WARNING) {
        return (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }
    else if (messageType <= UGF12_CONSOLE_MESSAGETYPE_ERROR) {
        return (FOREGROUND_RED | FOREGROUND_GREEN);
    }
    else if (messageType <= UGF12_CONSOLE_MESSAGETYPE_EXEPTION) {
        return (FOREGROUND_RED);
    }
    
    // Fallback white
    return (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

ImVec4 GxUtil::Console::getImGuiColor(UINT8 messageType) {
    // If on type
    if (messageType <= UGF12_CONSOLE_MESSAGETYPE_DEBUG) {
        return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else if (messageType <= UGF12_CONSOLE_MESSAGETYPE_INFO) {
        return ImVec4(0.129f, 0.439f, 0.749f, 1.0f);
    }
    else if (messageType <= UGF12_CONSOLE_MESSAGETYPE_WARNING) {
        return ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
    }
    else if (messageType <= UGF12_CONSOLE_MESSAGETYPE_ERROR) {
        return ImVec4(0.78f, 0.196f, 0.157f, 1.0f);
    }
    else if (messageType <= UGF12_CONSOLE_MESSAGETYPE_EXEPTION) {
        return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    }

    // Fallback white
    return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}

GxUtil::Console::Console(SIZE_T bufferSize, UINT8 messageThreshold) :
    m_size(bufferSize),
    m_mesageThreshold(messageThreshold)
{
    // Allocate buffer
    m_buffer = (CHAR*)malloc(m_size);

    // Set tail pointer
    m_ptrReadTail = m_buffer;
    m_ptrWriteHead = m_buffer + 1;

    // Initial tail
    *m_ptrReadTail = 0x0;
}

GxUtil::Console::~Console() {
    // Free buffer
    free(m_buffer);
}

void GxUtil::Console::writeAndIncrmentSafe(CHAR charToWrite) {
    // Check for overlap
    if (m_ptrWriteHead == m_ptrReadTail) {
        // Delete at read head
        deleteLastString();
    }

    // Write data
    *m_ptrWriteHead = charToWrite;

    // Incrment write head
    pointerIncrement(&m_ptrWriteHead);
}

void GxUtil::Console::deleteLastString(){
    // Increment until tail reaches end of last string
    do {
        pointerIncrement(&m_ptrReadTail);
    } while (*m_ptrReadTail != 0x0);
}

void GxUtil::Console::getReadPointer_lock(CHAR** ppChar) {
    // Lock
    m_lock.aquire();

    // Set read pointer
    *ppChar = m_ptrReadTail;
}

BOOL GxUtil::Console::getReadPointer_startString(CHAR** ppChar, UINT8* ptrMessageType) {
    // Check for 0x0
    if (**ppChar != 0x0) {
        return FALSE;
    }

    // Increment pointer
    pointerIncrement(ppChar);

    // Checking overloap
    if (*ppChar == m_ptrWriteHead) {
        return FALSE;
    }

    // Set message type
    *ptrMessageType = (**ppChar);

    // Inrement agin
    pointerIncrement(ppChar);
    
    // OK
    return TRUE;
}

void GxUtil::Console::getReadPointer_unlock() {
    // Release lock
    m_lock.release();
}

void GxUtil::Console::pointerIncrement(CHAR** ppCharToArray) {
    // Get current distance
    SIZE_T distance = *ppCharToArray - m_buffer;

    // Incrment distance
    distance = (distance + 1) % m_size;

    // Setup new pointer
    *ppCharToArray = m_buffer + distance;
}

void GxUtil::Console::pointerDecrement(CHAR** ppCharToArray) {
    // Create new pointer
    (*ppCharToArray)--;

    // Check if pointer is out of range
    if (*ppCharToArray < m_buffer) {
        // Wrapped to other side
        *ppCharToArray = m_buffer + m_size - 1;
    }
}
