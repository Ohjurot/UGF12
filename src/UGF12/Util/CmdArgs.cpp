#include "pch.h"
#include "CmdArgs.h"

GxUtil::CmdArgs::CmdArgs(LPWSTR cmdLine) {
    // Buffers for string assembly
    std::wstring key;
    // Target into witch strings get assembled
    std::wstringstream assembler;

    // State for string assembly
    INT state = 0;
    BOOL inQuote = FALSE;
    INT escapeValue = 0;

    // Itterate through string
    for (WCHAR* pChar = cmdLine; *pChar != 0x0; pChar++) {
        switch (state) {
            // 0 = Undefined area
            case 0:
                {
                    // If start char dash reached
                    if (*pChar == L'-') {
                        state = 1;
                    }
                }
                break;

            // 1 = Name of key 
            case 1:
                {
                    // On ' ' (space) flush and abort
                    if (*pChar == L' ') {
                        assembler.str(L"");
                        state = 0;
                    }
                    // On '=' safe key, flush and go on
                    else if (*pChar == L'=') {
                        key = assembler.str();
                        assembler.str(L"");
                        state = 2;
                    }
                    // On normal literal add to stream
                    else {
                        assembler << *pChar;
                    }
                }
                break;

            // 2 = value
            case 2:
                {
                    // On space safe key and value to map, flush and go to init state
                    if (*pChar == L' ' && !inQuote && !escapeValue) {
                        m_mapKeyValue.emplace(std::pair<std::wstring, std::wstring>(key, assembler.str()));
                        assembler.str(L"");
                        state = 0;
                    }
                    // On escape char set escape
                    else if (*pChar == L'\\' && !escapeValue) {
                        escapeValue = 2;
                    }
                    // Check quote status
                    else if (*pChar == L'"' && !escapeValue) {
                        inQuote = !inQuote;
                    }
                    // On literal add to steam
                    else {
                        assembler << *pChar;
                    }
                }
                break;
        }

        // Decrement escape
        if (escapeValue > 0) {
            escapeValue--;
        }
    }

    // Final append
    if (state == 2) {
        m_mapKeyValue.emplace(std::pair<std::wstring, std::wstring>(key, assembler.str()));
    }
}

std::wstring GxUtil::CmdArgs::operator[](std::wstring key){
    // Return empy string if key not exists
    if (!this->exitst(key)) {
        return L"";
    }

    // Return at key
    return m_mapKeyValue[key];
}

BOOL GxUtil::CmdArgs::exitst(std::wstring key) {
    // Try to find and check if not end
    return (m_mapKeyValue.find(key) != m_mapKeyValue.end());
}
