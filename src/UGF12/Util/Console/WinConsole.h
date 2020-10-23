#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>
#include <UGF12/Util/Console/Console.h>

namespace GxUtil {
	class WinConsole {
		public:
			/// <summary>
			/// Get the singelton instance of the console
			/// </summary>
			/// <returns></returns>
			static GxUtil::WinConsole* getInstance();

			/// <summary>
			/// Enable / Disable console
			/// </summary>
			/// <param name="showFlag"></param>
			void enableConsole(BOOL showFlag);

		private:
			/// <summary>
			/// Singelton construct
			/// </summary>
			WinConsole();

			// Destruct
			~WinConsole();

			#if !defined(SHIPPING)
			/// <summary>
			/// Static instance
			/// </summary>
			static WinConsole s_instance;
			#endif

			/// <summary>
			/// Notify called by global console
			/// </summary>
			/// <param name="consoleText">Text to be displayed</param>
			/// <param name="messageType">Message type</param>
			static void ConsoleNotify(LPCSTR consoleText, UINT8 messageType);

		private:
			/// <summary>
			/// Indicates if the console should be enabled
			/// </summary>
			BOOL m_bEnabled = TRUE;

			/// <summary>
			/// Window handle of the console
			/// </summary>
			HWND m_hwndConsole;

			/// <summary>
			/// Windows console cout handle
			/// </summary>
			HANDLE m_hConsoleOut;

			/// <summary>
			/// Windows console cin handle
			/// </summary>
			HANDLE m_hConsoleIn;
	};
}