#include "pch.h"
#include "DUI_Menue.h"

UGF12::DebugUI::UI_MainMenue::UI_MainMenue(GxDirect::XContext* ptrContext, GxDirect::XWindow* ptrWindow, UGF12::DebugUI::UI_InfoHint* ptrInfoHint, BOOL* vsyncPtr) :
	m_ptrWindow(ptrWindow),
    m_ptrInfoHint(ptrInfoHint),
    m_ptrVsyncBool(vsyncPtr)
{
    m_ptrUISysInfo = new UGF12::DebugUI::UI_SysInfo(ptrContext);
}

UGF12::DebugUI::UI_MainMenue::~UI_MainMenue() {
    delete m_ptrUISysInfo;
}

void UGF12::DebugUI::UI_MainMenue::draw(GxRenderIO::LayerStack::LayerFrameInfo* ptrFrameInfo, ImGuiIO* ptrImGuiIo) {
    if (ImGui::BeginMainMenuBar()) {
        // Begin always on main item
        if (ImGui::BeginMenu("UGF12")) {
            
            // TODO: General controle

            // Logical seperator
            ImGui::Separator();

            // Layers
            ImGui::MenuItem("Layerstack");

            // Settings 
            ImGui::MenuItem("Settings");

            // System Info
            if (ImGui::MenuItem("System Info")) {
                m_ptrUISysInfo->setEnable(TRUE);
            }

            // Logical seperator
            ImGui::Separator();

            // Fullscreen
            BOOL fullScreenState = m_ptrWindow->getFullscreen();
            if (ImGui::MenuItem("Fullscreen", NULL, fullScreenState)) {
                m_ptrWindow->setFullscreen(!fullScreenState);
            }

            // System metric
            BOOL hintState = m_ptrInfoHint->getEnable();
            if (ImGui::MenuItem("Info Hint", NULL, hintState)) {
                m_ptrInfoHint->setEnable(!hintState);
            }

            // VSYNC
            if(ImGui::MenuItem("VSYNC", NULL, *m_ptrVsyncBool)) {
                *m_ptrVsyncBool = !*m_ptrVsyncBool;
            }

            // Logical seperator
            ImGui::Separator();

            // Exit application
            if (ImGui::MenuItem("Exit", "Alt+F4")) {
                // Close window
                m_ptrWindow->close();
            }

            // End Item
            ImGui::EndMenu();
        }
        
        // End Menu Bar
        ImGui::EndMainMenuBar();
        
        // Draw components
        m_ptrUISysInfo->draw(ptrFrameInfo, ptrImGuiIo);
    }
}
