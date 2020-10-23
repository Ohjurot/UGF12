#pragma once
#include "pch.h"

#include <UGF12/Util/StrConverter.h>

#include <UGF12/DirectX/XContext.h>
#include <UGF12/DirectX/XWindow.h>

#include <UGF12/RenderIO/LayerManager/LayerStackManager.h>

#include <UGF12/Layers/DebugUI/IGuiDrawable.h>
#include <UGF12/Layers/DebugUI/Static/DUI_InfoHint.h>
#include <UGF12/Layers/DebugUI/Static/DUI_SysInfo.h>
#include <UGF12/Layers/DebugUI/Static/DUI_LayerStack.h>
#include <UGF12/Layers/DebugUI/Static/DUI_Console.h>

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
				/// <param name="ptrContext">Pointer to context</param>
				/// <parma name="ptrManager">Pointer to layerstack manager</param>
				/// <param name="ptrWindow">Pointer to window</param>
				/// <param name="ptrInfoHint">Pointer to info hint</param>
				/// <param name="vsyncPtr">VSync pointer</param>
				UI_MainMenue(GxDirect::XContext* ptrContext, GxRenderIO::LayerStack::Manager* ptrManager,  GxDirect::XWindow* ptrWindow, UGF12::DebugUI::UI_InfoHint* ptrInfoHint, BOOL* vsyncPtr);

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

				/// <summary>
				/// Pointer to info hint
				/// </summary>
				UGF12::DebugUI::UI_InfoHint* m_ptrInfoHint;

			private:
				/// <summary>
				/// System info window
				/// </summary>
				UGF12::DebugUI::UI_SysInfo m_uISysInfo;

				/// <summary>
				/// Layerstack ui
				/// </summary>
				UGF12::DebugUI::UI_LayerStack m_uiLayerStack;

				/// <summary>
				/// UI For console
				/// </summary>
				UGF12::DebugUI::UI_Console m_uiConsole;

				/// <summary>
				/// Pointer for VSYNC switching
				/// </summary>
				BOOL* m_ptrVsyncBool;
		};
	}
}