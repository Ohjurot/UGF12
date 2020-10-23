#include "pch.h"
#include "DUI_Console.h"

UGF12::DebugUI::UI_Console::UI_Console() {
    // Stride
    m_uiCmdStride = 255;
    m_uiCmdCount = 16;

    // Create cmd (histroy) buffer
    m_cmdBuffer = (CHAR*)malloc((SIZE_T)m_uiCmdStride * (SIZE_T)m_uiCmdCount);

    // Create input buffer
    m_inputBuffer = (CHAR*)malloc(m_uiCmdStride);
    m_inputBuffer[0] = 0x0;
}

UGF12::DebugUI::UI_Console::~UI_Console() {
    // Free buffer
    free(m_cmdBuffer);
    free(m_inputBuffer);
}

void UGF12::DebugUI::UI_Console::draw(GxRenderIO::LayerStack::LayerFrameInfo* ptrFrameInfo, ImGuiIO* ptrImGuiIo) {
    // Console window
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (!m_bVisible) {
        return;
    }
    if (!ImGui::Begin("Console", (bool*)&m_bVisible)) {
        ImGui::End();
        return;
    }

    // Begin text area
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

    // Get console pointer
    GxUtil::Console* ptrConsole = GxUtil::Console::getInstance();

    // Lock get pointers
    CHAR* ptrCurrentChar;
    UINT8 messageType;
    ptrConsole->getReadPointer_lock(&ptrCurrentChar);

    // Imgui possition ....
    FLOAT topLeftX = ImGui::GetCursorPosX();
    FLOAT curY = ImGui::GetCursorPosY();

    // While string can be started
    while (ptrConsole->getReadPointer_startString(&ptrCurrentChar, &messageType)) {
        // Setup color
        ImGui::PushStyleColor(ImGuiCol_Text, ptrConsole->getImGuiColor(messageType));

        // Inital X for line
        FLOAT curX = topLeftX;

        // While not null
        while (*ptrCurrentChar) {
            // Set pos
            ImGui::SetCursorPos({ curX, curY });

            // New X and draw
            FLOAT xIncr;
            ImGui::TextEx(ptrCurrentChar, ptrCurrentChar + 1, ImGuiTextFlags_NoWidthForLargeClippedText, &xIncr);

            // Incrment x
            curX += xIncr;

            // Incrment pointer
            ptrCurrentChar++;
        }

        // Increment y
        curY += 14.0f;

        // Pop color
        ImGui::PopStyleColor();
    }

    // Unlock pointers
    ptrConsole->getReadPointer_unlock();

    // Scrole
    if ((m_bAutoScrole && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) {
        ImGui::SetScrollHereY(1.0f);
    }

    // End text area
    ImGui::EndChild();

    // Input line
    BOOL setFocus = FALSE;
    if (ImGui::InputText(
        "Command", 
        m_inputBuffer, 
        m_uiCmdStride, 
        ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory, 
        &TextEditCallbackStub, this
    )) {
        // TODO: Process cmd

        // Send to console
        std::stringstream ss;
        ss << "# " << m_inputBuffer;

        // Log to console
        GxUtil::Console::getInstance()->writeString(ss.str().c_str(), UGF12_CONSOLE_MESSAGETYPE_NONE);

        // Copy to offscreen buffer
        strcpy_s(&m_cmdBuffer[m_uiBufferPos * m_uiCmdStride], m_uiCmdStride, m_inputBuffer);
        m_uiBufferPos = (m_uiBufferPos + 1) % m_uiCmdCount;

        // Commited string
        m_uiBufferCommits = std::min(m_uiBufferCommits + 1, m_uiCmdCount - 1);

        // Clear input
        m_inputBuffer[0] = 0x0;
        
        // No offset
        m_uiHistoryOffset = 0;
        

        // Indicate reefocus
        setFocus = TRUE;
    }

    // Default focus
    ImGui::SetItemDefaultFocus();
    
    // Reset focus
    if (setFocus) {
        ImGui::SetKeyboardFocusHere(-1);
    }

    ImGui::End();
}

void UGF12::DebugUI::UI_Console::setEnable(BOOL enabled) {
	m_bVisible = enabled;
}

BOOL UGF12::DebugUI::UI_Console::getEnable() {
	return m_bVisible;
}

INT UGF12::DebugUI::UI_Console::TextEditCallbackStub(ImGuiInputTextCallbackData* ptrData) {
    // Extract console
    UGF12::DebugUI::UI_Console* ptrConsole = (UGF12::DebugUI::UI_Console*)ptrData->UserData;
    // Call console
    return ptrConsole->internalTextEditCallbackStub(ptrData);
}

INT UGF12::DebugUI::UI_Console::internalTextEditCallbackStub(ImGuiInputTextCallbackData* ptrData) {
    // Event flag
    switch (ptrData->EventFlag) {
        // History Up/Down
        case ImGuiInputTextFlags_CallbackHistory: {
                // Should lookup
                BOOL lookUp = FALSE;
            
                // Up down selection
                if (ptrData->EventKey == ImGuiKey_UpArrow) {
                    m_uiHistoryOffset = std::min(m_uiHistoryOffset + 1, m_uiBufferCommits);

                    lookUp = TRUE;
                }
                else if (ptrData->EventKey == ImGuiKey_DownArrow) {
                    if (m_uiHistoryOffset) {
                        m_uiHistoryOffset--;
                    }

                    lookUp = TRUE;
                }

                if (lookUp) {
                    // Decrement lookup index
                    INT bufferOffset = m_uiBufferPos - m_uiHistoryOffset;

                    // Check if negativ
                    if (bufferOffset < 0) {
                        // Setup wrapping 
                        bufferOffset = m_uiCmdCount - 1 - abs(bufferOffset);
                    }

                    // Setup text
                    ptrData->DeleteChars(0, ptrData->BufTextLen);
                    if (m_uiHistoryOffset) {
                        ptrData->InsertChars(0, &m_cmdBuffer[bufferOffset * m_uiCmdStride]);
                    }
                }
            }
            break;
    }

    // Handled
    return 0;
}
