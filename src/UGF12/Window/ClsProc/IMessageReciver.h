#pragma once
#include "pch.h"

namespace GxWindow {
	/// <summary>
	/// Interface for all window message reciving classes
	/// </summary>
	class IMessageReciver {
		public:
			/// <summary>
			/// Required: Handle a message in derivated class
			/// </summary>
			/// <param name="ptrResult">Pointer to the lresult to be returned when handled</param>
			/// <param name="wnd">Window for witch the message is</param>
			/// <param name="msg">Occured message code</param>
			/// <param name="wParam">WPARAM</param>
			/// <param name="lParam">LPARAM</param>
			/// <returns>If the handler did handle the message</returns>
			virtual BOOL handleWindowMessage(LRESULT* ptrResult, HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	};
}