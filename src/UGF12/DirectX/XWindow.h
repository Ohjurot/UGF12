#pragma once
#include "pch.h"

#include <UGF12/Window/Window.h>
#include <UGF12/DirectX/XContext.h>
#include <UGF12/DirectX/XCmdQueue.h>
#include <UGF12/DirectX/XWndDriver.h>

namespace GxDirect {
	class XWindow : public GxWindow::Window {
		public:
			XWindow(std::wstring title, UINT width, UINT height, GxDirect::XContext* ptrContext, GxDirect::XCmdQueue* ptrCmdQue);

			// Destructor to free resources
			~XWindow();

			/// <summary>
			/// Indicates if reesize was requestes
			/// </summary>
			/// <returns>If resize requested</returns>
			BOOL resizeRequested();

			/// <summary>
			/// Resizes the swap chain if required now (delayed from window resize during frame execution) also enable / diables the fullscreen state
			/// </summary>
			/// <param name="ptrWidth">Pointer to a UINT to notify new width</param>
			/// <param name="ptrHeight">Pointer to a UINT to notify new height</param>
			void resizeNow(UINT* ptrWidth, UINT* ptrHeight);

			/// <summary>
			/// Sets if the XWindow should be fullscreen this function is delayed (fulscreen is update on call of resizeNow() )
			/// </summary>
			/// <param name="state">If the window should be fullscreen</param>
			void setFullscreen(BOOL state);

			/// <summary>
			/// Begin a frame on this window
			/// </summary>
			/// <param name="ptrCmdList">Command list used to draw on window</param>
			void beginFrame(ID3D12GraphicsCommandList* ptrCmdList);

			/// <summary>
			/// End a frame on this window
			/// </summary>
			/// <param name="ptrCmdList">Command list used to draw on window</param>
			void endFrame(ID3D12GraphicsCommandList* ptrCmdList);

			/// <summary>
			/// Present the frame drawn to the user
			/// </summary>
			/// <param name="vsync">If vsync should be enabled</param>
			void present(BOOL vsync = FALSE);

			/// <summary>
			/// Handle messages occured in the window
			/// </summary>
			/// <param name="ptrResult">Pointer to the lresult to be returned when handled</param>
			/// <param name="wnd">Window for witch the message is</param>
			/// <param name="msg">Occured message code</param>
			/// <param name="wParam">WPARAM</param>
			/// <param name="lParam">LPARAM</param>
			/// <returns>If the handler did handle the message</returns>
			virtual BOOL handleWindowMessage(LRESULT* ptrResult, HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

			// Delete unuse
			void operator=(const XWindow&) = delete;
			XWindow(const XWindow&) = delete;
		private:
			/// <summary>
			/// Pointer to driver
			/// </summary>
			GxDirect::XWndDriver* m_ptrDriver;

			/// <summary>
			/// Pointer to command que
			/// </summary>
			GxDirect::XCmdQueue* m_ptrCmdQue;

			/// <summary>
			/// Bool witch holds if a resize is required
			/// </summary>
			BOOL m_bRequestResize = FALSE;

			/// <summary>
			/// Keeps track of current full screen state
			/// </summary>
			BOOL m_bFullscreenCurrent = FALSE;

			/// <summary>
			/// Value witch the fullscreen state shoudl have
			/// </summary>
			BOOL m_bFullscreenShould = FALSE;
	};
}