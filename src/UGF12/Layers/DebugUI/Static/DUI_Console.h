#pragma once
#include "pch.h"

#include <UGF12/Util/StrConverter.h>
#include <UGF12/Util/Console/Console.h>

#include <UGF12/DirectX/XContext.h>

#include <UGF12/Layers/DebugUI/IGuiDrawable.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace UGF12 {
	namespace DebugUI {
		/// <summary>
		/// Interface for GUI draw operations
		/// </summary>
		class UI_Console : public UGF12::DebugUI::IGuiDrawable {
			public:
				/// <summary>
				/// Constructor
				/// </summary>
				UI_Console();

				// Destruct
				~UI_Console();

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
				UI_Console(const UI_Console&) = delete;
				void operator==(const UI_Console&) = delete;
			private:
				/// <summary>
				/// Static imgui callback
				/// </summary>
				/// <param name="ptrData">Pointer to data object</param>
				/// <returns></returns>
				static INT TextEditCallbackStub(ImGuiInputTextCallbackData* ptrData);

				/// <summary>
				/// Internal callback (see static)
				/// </summary>
				/// <param name="ptrData"></param>
				/// <returns></returns>
				INT internalTextEditCallbackStub(ImGuiInputTextCallbackData* ptrData);
				
			private:
				/// <summary>
				/// Indicates if UI element should be visible
				/// </summary>
				BOOL m_bVisible = FALSE;

				/// <summary>
				/// Autoscrole
				/// </summary>
				BOOL m_bAutoScrole = TRUE;

				/// <summary>
				/// Stride of commands
				/// </summary>
				UINT m_uiCmdStride;

				/// <summary>
				/// Command count
				/// </summary>
				UINT m_uiCmdCount;

				/// <summary>
				/// Command buffer
				/// </summary>
				CHAR* m_cmdBuffer;

				/// <summary>
				/// Input buffer
				/// </summary>
				CHAR* m_inputBuffer;

				/// <summary>
				/// Position in buffer
				/// </summary>
				UINT m_uiBufferPos = 0;

				/// <summary>
				/// Commits
				/// </summary>
				UINT m_uiBufferCommits = 0;

				/// <summary>
				/// History offset
				/// </summary>
				UINT m_uiHistoryOffset = 0;
		};
	}
}