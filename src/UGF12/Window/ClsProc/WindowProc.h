#pragma once
#include "pch.h"

#include <UGF12/Window/ClsProc/IMessageReciver.h>

namespace GxWindow {
	/// <summary>
	/// Class witch holds the static instances of the two window procs
	/// </summary>
	class WndProc {
		public:
			/// <summary>
			/// Setup window proc will setup the void* in the creatstruct to the window userdata and reroutes the windowproc to wndProc run
			/// </summary>
			/// <param name="wnd">Window for witch the message is</param>
			/// <param name="msg">Occured message code</param>
			/// <param name="wParam">WPARAM</param>
			/// <param name="lParam">LPARAM</param>
			/// <returns>Result of the window proc</returns>
			static LRESULT CALLBACK wndProcSetup(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
			
			/// <summary>
			/// Redirects the window message to a IMessageReciver stored in the userdata
			/// </summary>
			/// <param name="wnd">Window for witch the message is</param>
			/// <param name="msg">Occured message code</param>
			/// <param name="wParam">WPARAM</param>
			/// <param name="lParam">LPARAM</param>
			/// <returns>Result of the window proc</returns>
			static LRESULT CALLBACK wndProcRun(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}