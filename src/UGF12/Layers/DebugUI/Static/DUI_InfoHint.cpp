#include "pch.h"
#include "DUI_InfoHint.h"

UGF12::DebugUI::UI_InfoHint::UI_InfoHint(GxDirect::XContext* ptrContext, BOOL visible) :
	m_bVisible(visible),
	m_ptrContext(ptrContext)
{
	// Increment ref
	m_ptrContext->IncRef();
}

UGF12::DebugUI::UI_InfoHint::~UI_InfoHint() {
	// Decrement ref
	m_ptrContext->DecRef();
}

void UGF12::DebugUI::UI_InfoHint::draw(GxRenderIO::LayerStack::LayerFrameInfo * ptrFrameInfo, ImGuiIO * ptrImGuiIo) {
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

	ImVec2 window_pos = ImVec2(10.0f, ptrImGuiIo->DisplaySize.y - 10.0f);
	ImVec2 window_pos_pivot = ImVec2(0.0f, 1.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

	if (ImGui::Begin("Overlay", (bool*)&m_bVisible, window_flags)) {
		std::wstringstream wss;
		wss << L"Frametime: " << ptrFrameInfo->deltaTMs << L"ms (" << (INT)ceil(1000 / std::max<FLOAT>(0.001f, ptrFrameInfo->deltaTMs)) << L" FPS)" << std::endl;
		wss << m_ptrContext->getGpuInfo().Description << std::endl;
		wss << m_ptrContext->getFeatureString() << std::endl;
		wss << L"Window Size: " << ptrImGuiIo->DisplaySize.x << L" x " << ptrImGuiIo->DisplaySize.y;
		ImGui::Text(GxUtil::StrConverter::ConvertW2N(wss.str()).c_str());
	}
	ImGui::End();
}

void UGF12::DebugUI::UI_InfoHint::setEnable(BOOL enabled) {
	m_bVisible = enabled;
}
