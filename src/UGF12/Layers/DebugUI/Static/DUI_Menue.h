#pragma once
#include "pch.h"

#include <UGF12/Util/StrConverter.h>

#include <UGF12/DirectX/XWindow.h>

#include <UGF12/Layers/DebugUI/IGuiDrawable.h>

#include <imgui.h>

namespace UGF12 {
	namespace DebugUI {
		/// <summary>
		/// Main UI Menue
		/// </summary>
		class UI_MainMenue : public UGF12::DebugUI::IGuiDrawable {
			public:
				/// <summary>
				/// Create with pointer to window
				/// </summary>
				/// <param name="ptrWindow"></param>
				UI_MainMenue(GxDirect::XWindow* ptrWindow);

				/// <summary>
				/// Draw GUI part
				/// </summary>
				/// <param name="ptrFrameInfo">Frame info</param>
				/// <param name="ptrImGuiIo">ImGuiIO</param>
				void draw(GxRenderIO::LayerStack::LayerFrameInfo* ptrFrameInfo, ImGuiIO* ptrImGuiIo);

				// Delete unused
				UI_MainMenue(const UI_MainMenue&) = delete;
				void operator==(const UI_MainMenue&) = delete;
			private:
				/// <summary>
				/// Pointer to window
				/// </summary>
				GxDirect::XWindow* m_ptrWindow;
		};
	}
}