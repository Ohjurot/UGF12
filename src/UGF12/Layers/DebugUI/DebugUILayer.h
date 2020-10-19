#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>
#include <UGF12/Util/StrConverter.h>

#include <UGF12/DirectX/XContext.h>
#include <UGF12/DirectX/XWindow.h>

#include <UGF12/Window/ClsProc/IMessageReciver.h>

#include <UGF12/RenderIO/LayerManager/ILayerImpl.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

#include <UGF12/Layers/DebugUI/Static/DUI_InfoHint.h>
#include <UGF12/Layers/DebugUI/Static/DUI_Menue.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace UGF12 {
	namespace DebugUI {
		/// <summary>
		/// Debug UIs layer
		/// </summary>
		class DebugUILayer : public GxRenderIO::LayerStack::ILayerImpl, public GxWindow::IMessageReciver{
			public:
				/// <summary>
				/// Create debug layer
				/// </summary>
				/// <param name="ptrContext">Pointe to context</param>
				/// <param name="ptrWindow">Pointe to window</param>
				/// <param name="vsyncPtr">Pointer to the vsync bool</param>
				/// <param name="visible">If the layer should be visible</param>
				DebugUILayer(GxDirect::XContext* ptrContext, GxDirect::XWindow* ptrWindow, BOOL* vsyncPtr, BOOL visible = TRUE);

				/// <summary>
				/// Construct
				/// </summary>
				void Init();

				/// <summary>
				/// Destruct
				/// </summary>
				void Destroy();

				/// <summary>
				/// Function is called when layer should do its draw operations
				/// </summary>
				/// <param name="ptrFrameInfo">Info about the frame (pointer)</param>
				/// <param name="ptrCmdListProxy">Pointer to cmd list proxy</param>
				/// <param name="ptrFrameBuffer">Pointer to frame buffer</param>
				void draw(GxRenderIO::LayerStack::LayerFrameInfo* ptrFrameInfo, GxRenderIO::CmdListProxy* ptrCmdListProxy, GxRenderIO::FrameBuffer* ptrFrameBuffer);

				/// <summary>
				/// Gets called when a resource is recreated or changes
				/// </summary>
				/// <param name="resourceType">Type of the resource UGF12_RESOURCE_TYPE_XXXXX</param>
				/// <param name="resourceIndex">Index in multibuffering array (typical 0 or 1)</param>
				/// <param name="ptrResource">Pointer to the changes resource</param>
				void onResourceChange(UINT resource, UINT index, void* ptrResource);

				/// <summary>
				/// Gets called when the layer gets resizes (Swap Chain resize event)
				/// </summary>
				/// <param name="width">Width of the layer</param>
				/// <param name="height">Height of the layer</param>
				void onResize(UINT width, UINT height);

				/// <summary>
				/// Gets called when the layer should be enabled
				/// </summary>
				/// <returns>True if layer can be enabled</returns>
				BOOL onEnable();

				/// <summary>
				/// Window Handle for debug UI
				/// </summary>
				/// <param name="ptrResult"></param>
				/// <param name="wnd"></param>
				/// <param name="msg"></param>
				/// <param name="wParam"></param>
				/// <param name="lParam"></param>
				/// <returns></returns>
				BOOL handleWindowMessage(LRESULT* ptrResult, HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
			private:
				/// <summary>
				/// Reference to context
				/// </summary>
				GxDirect::XContext* m_ptrContext;

				/// <summary>
				/// Handle of target window
				/// </summary>
				GxDirect::XWindow* m_ptrWindow;

				/// <summary>
				/// Vsync boolean
				/// </summary>
				BOOL* m_ptrVsyncBool;

				/// <summary>
				/// ImGui descriptor heap
				/// </summary>
				ID3D12DescriptorHeap* m_ptrImGuiHeap = NULL;

				/// <summary>
				/// Render target view descriptor heap
				/// </summary>
				ID3D12DescriptorHeap* m_ptrHeapRtv = NULL;

				/// <summary>
				/// Viewport
				/// </summary>
				D3D12_VIEWPORT m_viewPort = {};

				/// <summary>
				/// Sic rect
				/// </summary>
				RECT m_sicRect = {};

			private:
				/// <summary>
				/// Internal UI Hint object
				/// </summary>
				UI_InfoHint* m_ptrInfoHint = NULL;

				/// <summary>
				/// Internal UI Main Menue objecf
				/// </summary>
				UI_MainMenue* m_ptrMainMenue = NULL;
		};
	}
}