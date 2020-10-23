#include "pch.h"
#include "DUI_LayerStack.h"

UGF12::DebugUI::UI_LayerStack::UI_LayerStack(GxRenderIO::LayerStack::Manager* ptrLayerStackManager) :
	m_ptrLayerStackManager(ptrLayerStackManager)
{}

void UGF12::DebugUI::UI_LayerStack::draw(GxRenderIO::LayerStack::LayerFrameInfo* ptrFrameInfo, ImGuiIO* ptrImGuiIo){
	if (m_bVisible &&  ImGui::Begin("Layerstack manager", (bool*)&m_bVisible)) {
		// Reset button
		if (ImGui::Button("Reset min, max and avg")) {
			// Global
			m_ltGlobal.max = 0.0f;
			m_ltGlobal.min = 999999.9f;
			m_ltGlobal.avg = 0.0f;
			m_ltGlobal.sumOfFrameTimes = 0.0f;

			// Layer
			for (UINT i = 0; i < 16; i++) {
				m_arrLtLayers[i].max = 0.0f;
				m_arrLtLayers[i].min = 999999.9f;
				m_arrLtLayers[i].avg = 0.0f;
				m_arrLtLayers[i].sumOfFrameTimes = 0.0f;
			}

			// Count 
			m_uiSumsCount = 0;
		}
		
		// Update global timing
		if (m_uiSumsCount == m_uiSumThreshold) {
			m_ltGlobal.avg = ((m_ltGlobal.sumOfFrameTimes / m_uiSumThreshold) + m_ltGlobal.avg) / 2;
			m_ltGlobal.sumOfFrameTimes = 0.0f;
		}

		m_ltGlobal.sumOfFrameTimes += ptrFrameInfo->deltaTMs;
		m_ltGlobal.min = std::min<FLOAT>(m_ltGlobal.min, ptrFrameInfo->deltaTMs);
		m_ltGlobal.max = std::max<FLOAT>(m_ltGlobal.max, ptrFrameInfo->deltaTMs);
		
		// Global meta (UI)
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Overall timing:");
		ImGui::InputFloat("Frametime", &ptrFrameInfo->deltaTMs, 0.0f, 0.0f, 3, ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat("Frametime Avg", &m_ltGlobal.avg, 0.0f, 0.0f, 3, ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat("Frametime Min", &m_ltGlobal.min, 0.0f, 0.0f, 3, ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat("Frametime Max", &m_ltGlobal.max, 0.0f, 0.0f, 3, ImGuiInputTextFlags_ReadOnly);
		
		// Get layers meta
		GxRenderIO::LayerStack::LayerMeta layerMeta;
		for (UINT i = 0; m_ptrLayerStackManager->getLayerMeta(i, &layerMeta); i++) {
			// Update timing
			if (m_uiSumsCount == m_uiSumThreshold) {
				m_arrLtLayers[i].avg = ((m_arrLtLayers[i].sumOfFrameTimes / m_uiSumThreshold) + m_arrLtLayers[i].avg) / 2;
				m_arrLtLayers[i].sumOfFrameTimes = 0.0f;
			}

			m_arrLtLayers[i].sumOfFrameTimes += layerMeta.layerFrametime;
			m_arrLtLayers[i].min = std::min<FLOAT>(m_arrLtLayers[i].min, layerMeta.layerFrametime);
			m_arrLtLayers[i].max = std::max<FLOAT>(m_arrLtLayers[i].max, layerMeta.layerFrametime);

			// Draw UI
			std::stringstream ss;
			ss << "\nLayer #" << i << ": " << GxUtil::StrConverter::ConvertW2N(layerMeta.layerName).c_str();

			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ss.str().c_str());
			ImGui::Checkbox("Enabled", (bool*)&layerMeta.layerEnabled);

			if (layerMeta.layerEnabled) {
				ImGui::InputFloat("Frametime", &layerMeta.layerFrametime, 0.0f, 0.0f, 3, ImGuiInputTextFlags_ReadOnly);
				ImGui::InputFloat("Frametime Avg", &m_arrLtLayers[i].avg, 0.0f, 0.0f, 3, ImGuiInputTextFlags_ReadOnly);
				ImGui::InputFloat("Frametime Min", &m_arrLtLayers[i].min, 0.0f, 0.0f, 3, ImGuiInputTextFlags_ReadOnly);
				ImGui::InputFloat("Frametime Max", &m_arrLtLayers[i].max, 0.0f, 0.0f, 3, ImGuiInputTextFlags_ReadOnly);
			}
		}

		// Check if reset is required
		if (m_uiSumsCount == m_uiSumThreshold) {
			m_uiSumsCount = 0;
		}

		// Increment sum count
		m_uiSumsCount++;
		ImGui::End();
	}
}

void UGF12::DebugUI::UI_LayerStack::setEnable(BOOL enabled){
	m_bVisible = TRUE;
}

BOOL UGF12::DebugUI::UI_LayerStack::getEnable() {
	return m_bVisible;
}
