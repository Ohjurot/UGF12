#pragma once
#include "pch.h"

#include <UGF12/Window/ClsProc/WindowProc.h>

namespace GxWindow {
	/// <summary>
	/// Singelton class for registring the window class
	/// </summary>
	class WndClass {
		public:
			/// <summary>
			/// Retrive the class name of the singelton
			/// </summary>
			/// <returns></returns>
			static LPCWSTR getClassName();

			/// <summary>
			/// Retrive if init was successfull
			/// </summary>
			/// <returns>If the init was successfull</returns>
			static BOOL getInitStatus();

			/// <summary>
			/// Retrive the HINSTANCE of the window class
			/// </summary>
			/// <returns>HINSTANCE</returns>
			static HINSTANCE getHInstance();

		private:
			/// <summary>
			/// Singelton instance
			/// </summary>
			static WndClass s_instance;

			/// <summary>
			/// Singelton constructor to init window class
			/// </summary>
			WndClass();

			// Singelton destruct to unreguster window class
			~WndClass();

			/// <summary>
			/// Store the hInstance to query only in constructor
			/// </summary>
			HINSTANCE m_hInstance = 0;

			/// <summary>
			/// Keeps track if window clas could be registered
			/// </summary>
			BOOL m_initOk = FALSE;

			/// <summary>
			/// Static string storing the class name
			/// </summary>
			LPCWSTR m_className = L"GxWndClass";
	};
}