#include "pch.h"
#include "DUI_Menue.h"

UGF12::DebugUI::UI_MainMenue::UI_MainMenue(GxDirect::XWindow* ptrWindow) :
	m_ptrWindow(ptrWindow)
{}

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
            ImGui::MenuItem("System Info");


            // Logical seperator
            ImGui::Separator();

            // Fullscreen
            BOOL fullScreenState = m_ptrWindow->getFullscreen();
            if (ImGui::MenuItem("Fullscreen", NULL, fullScreenState)) {
                m_ptrWindow->setFullscreen(!fullScreenState);
            }

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
    }
}
