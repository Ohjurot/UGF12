#pragma once
#include "pch.h"

#include <UGF12/Util/StrConverter.h>

#include <UGF12/Layers/DebugUI/IGuiDrawable.h>

#include <UGF12/RenderIO/LayerManager/LayerStackManager.h>

#include <imgui.h>

namespace UGF12 {
	namespace DebugUI {
		/// <summary>
		/// Layer Stack metrics
		/// </summary>
		class UI_LayerStack : public UGF12::DebugUI::IGuiDrawable {
			public:
				/// <summary>
				/// Constructor
				/// </summary>
				/// <param name="ptrContext">Pointer to context</param>
				/// <param name="visible">If the ui element shoukd be visible</param>
				UI_LayerStack(GxRenderIO::LayerStack::Manager* ptrLayerStackManager);

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

				/// <summary>
				/// Retrive the enable state
				/// </summary>
				/// <returns>If enabled</returns>
				BOOL getEnable();

				// Delete unsupported
				UI_LayerStack(const UI_LayerStack&) = delete;
				void operator==(const UI_LayerStack&) = delete;
			private:
				struct LayerTiming {
					/// <summary>
					/// Sum of frametimes
					/// </summary>
					FLOAT sumOfFrameTimes = 0.0f;

					/// <summary>
					/// Average frame timing
					/// </summary>
					FLOAT avg = 0.0f;

					/// <summary>
					/// Minimum frametimes
					/// </summary>
					FLOAT min = 999.9f;

					/// <summary>
					/// Maxiumum layer frametime
					/// </summary>
					FLOAT max = 0.0f;
				};
			private:
				/// <summary>
				/// Pointer to layerstack manger
				/// </summary>
				GxRenderIO::LayerStack::Manager* m_ptrLayerStackManager;

				/// <summary>
				/// Indicates if UI element should be visible
				/// </summary>
				BOOL m_bVisible = FALSE;

				/// <summary>
				/// Global timing
				/// </summary>
				LayerTiming m_ltGlobal;

				/// <summary>
				/// Arrays of layer timings
				/// </summary>
				LayerTiming m_arrLtLayers[16];

				/// <summary>
				/// Summing count of frametimes
				/// </summary>
				UINT m_uiSumsCount = 0;

				/// <summary>
				/// Numbers of sums until averag is calculated
				/// </summary>
				UINT m_uiSumThreshold = 100;
		};
	}
}