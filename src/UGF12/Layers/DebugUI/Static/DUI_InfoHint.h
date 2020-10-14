#pragma once
#include "pch.h"

#include <UGF12/Util/StrConverter.h>

#include <UGF12/DirectX/XContext.h>

#include <UGF12/Layers/DebugUI/IGuiDrawable.h>

#include <imgui.h>

namespace UGF12 {
	namespace DebugUI {
		/// <summary>
		/// Interface for GUI draw operations
		/// </summary>
		class UI_InfoHint : public UGF12::DebugUI::IGuiDrawable {
			public:
				/// <summary>
				/// Constructor
				/// </summary>
				/// <param name="ptrContext">Pointer to context</param>
				/// <param name="visible">If the ui element shoukd be visible</param>
				UI_InfoHint(GxDirect::XContext* ptrContext, BOOL visible = TRUE);

				// Destructor (ref counting)
				~UI_InfoHint();

				/// <summary>
				/// Draw GUI part
				/// </summary>
				/// <param name="ptrFrameInfo">Frame info</param>
				/// <param name="ptrImGuiIo">ImGuiIO</param>
				void draw(GxRenderIO::LayerStack::LayerFrameInfo* ptrFrameInfo, ImGuiIO* ptrImGuiIo);

				/// <summary>
				/// Set if the UI Element should be enabled
				/// </summary>
				/// <param name="enabled">Enable flag</param>
				void setEnable(BOOL enabled);

				// Delete unsupported
				UI_InfoHint(const UI_InfoHint&) = delete;
				void operator==(const UI_InfoHint&) = delete;
			private:
				/// <summary>
				/// Pointer to context
				/// </summary>
				GxDirect::XContext* m_ptrContext;

				/// <summary>
				/// Indicates if UI element should be visible
				/// </summary>
				BOOL m_bVisible;
		};
	}
}