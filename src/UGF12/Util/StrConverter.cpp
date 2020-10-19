#include "pch.h"
#include "StrConverter.h"

std::wstring GxUtil::StrConverter::ConvertN2W(std::string input) {
    // Temporayry output string
    std::wstring out;

    // Check length of ouput
    SIZE_T strLen = MultiByteToWideChar(CP_UTF8, NULL, input.c_str(), -1, NULL, NULL);

    // Make space in string
    out.reserve(strLen);

    // Convert String
    MultiByteToWideChar(CP_UTF8, NULL, input.c_str(), -1, out.data(), strLen);

    // Return new String
    return out;
}

std::string GxUtil::StrConverter::ConvertW2N(std::wstring input) {
    // Temporary output string
    std::string out;

    // Check length of output
    SIZE_T strLen = WideCharToMultiByte(CP_UTF8, NULL, input.c_str(), -1, NULL, NULL, NULL, NULL);
    
    // Make space in string
    out.reserve(strLen);

    // Convert String
    WideCharToMultiByte(CP_UTF8, NULL, input.c_str(), -1, out.data(), strLen, NULL, NULL);

    // Return new string
    return out;
}

std::wstring GxUtil::StrConverter::TranslateHRESULT(HRESULT hr) {
    // Temporary strings
    std::wstring out;
    LPWSTR tempBuff = NULL;
    
    // Get length
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&tempBuff, 0, NULL);

    // Copy text
    out = tempBuff;

    // Free temp
    LocalFree(tempBuff);

    // Return new string
    return out;
}

std::string GxUtil::StrConverter::memoryToString(UINT64 memory) {
    // Left memory
    UINT divisionsCount = 0;
    DOUBLE left = memory;

    // While dividable
    while (left >= 1024.0) {
        left = left / 1024.0;
        divisionsCount++;
    }

    // Create string
    std::stringstream ss;
    ss << left;

    // Typename
    switch (divisionsCount) {
        case 0:
            ss << " Bytes";
            break;
        case 1:
            ss << " KiB";
            break;
        case 2:
            ss << " MiB";
            break;
        case 3:
            ss << " GiB";
            break;
        case 4:
            ss << " TiB";
            break;
        case 5:
            ss << " PiB";
            break;
        default:
            ss << " * (1024 ^ " << (divisionsCount - 5) << ") PiB";
            break;
    }

    // Return string
    return ss.str();
}
