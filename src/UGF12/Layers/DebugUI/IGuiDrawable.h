#pragma once
#include "pch.h"

#include <UGF12/RenderIO/LayerManager/ILayerImpl.h>

#include <imgui.h>

namespace UGF12 {
	namespace DebugUI {
		/// <summary>
		/// Interface for GUI draw operations
		/// </summary>
		class IGuiDrawable {
			public:
				/// <summary>
				/// Draw GUI part
				/// </summary>
				/// <param name="ptrFrameInfo">Frame info</param>
				/// <param name="ptrImGuiIo">ImGuiIO</param>
				virtual void draw(GxRenderIO::LayerStack::LayerFrameInfo* ptrFrameInfo, ImGuiIO* ptrImGuiIo) = 0;
		};
	}
}