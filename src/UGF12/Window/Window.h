#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>

#include <UGF12/Window/ClsProc/IMessageReciver.h>
#include <UGF12/Window/ClsProc/WindowClass.h>

#include <UGF12/IO/KeyStates.h>

namespace GxWindow {
	class Window : public GxWindow::IMessageReciver {
		public:
			/// <summary>
			/// Create a window with a title and size
			/// </summary>
			/// <param name="title">Title of the window</param>
			/// <param name="width">Width of the window</param>
			/// <param name="height">Height of the window</param>
			Window(std::wstring title, UINT width, UINT height);

			// Destruc to destroy window if alive
			~Window();

			/// <summary>
			/// Executes the message loop for this window
			/// </summary>
			/// <param name="messageLimit">Limit of message to be proccess until function reurns (-1 is equal to infinity)</param>
			void runMessageLoop(INT messageLimit = -1);

			/// <summary>
			/// Sets the visability of the window
			/// </summary>
			/// <param name="visiable"></param>
			void setWindowVisability(BOOL visable);

			/// <summary>
			/// Set the windows icon
			/// </summary>
			/// <param name="icon">HICON to be set as icon</param>
			void setIcon(HICON icon);

			/// <summary>
			/// Closes the window (destroy)
			/// </summary>
			void close();

			/// <summary>
			/// Returns if window is still valid
			/// </summary>
			/// <returns>HWND != NULL</returns>
			BOOL isValid();

			/// <summary>
			/// Retrive if the window is active
			/// </summary>
			/// <returns>If the window is active</returns>
			BOOL isActive();

			/// <summary>
			/// Retrives the handle to the window
			/// </summary>
			/// <returns>HWND of the window</returns>
			HWND getHandle();

			/// <summary>
			/// Expose pointer to internal key dispatcher
			/// </summary>
			/// <returns></returns>
			GxIO::KeyState* getKeyControler();

			/// <summary>
			/// Get the state of a key
			/// </summary>
			/// <param name="vKey">Key code</param>
			/// <returns>If the key is down</returns>
			BOOL getKeyState(UINT8 vKey);

			/// <summary>
			/// Setup the UI Layer
			/// </summary>
			/// <param name="ptrUiLayer">Input UI Layer</param>
			void setUILayer(GxWindow::IMessageReciver* ptrUiLayer);

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
			void operator=(const Window&) = delete;
			Window(const Window&) = delete;
		private:
			/// <summary>
			/// Window handle
			/// </summary>
			HWND m_hWindow;
			
			/// <summary>
			/// Store keybinds for this window
			/// </summary>
			GxIO::KeyState m_keyBind;

			/// <summary>
			/// Window is active
			/// </summary>
			BOOL m_bIsActive = FALSE;

			/// <summary>
			/// Indicates that the window should close
			/// </summary>
			BOOL m_bShouldClose = FALSE;

			/// <summary>
			/// Pointer for UI hook
			/// </summary>
			GxWindow::IMessageReciver* m_ptrUiLayer = NULL;
	};
}