#pragma once
#include "pch.h"

/// <summary>
/// A keycode is a uint8 value (VKEY range) & 16-bit mask for xinput
/// </summary>
typedef WORD KEYCODE;

/// <summary>
/// KB_KEY is a keyboard key
/// </summary>
typedef KEYCODE KB_KEY;

/// <summary>
/// HC_KEY is a handheld controler key
/// </summary>
typedef KEYCODE HC_KEY;

namespace GxIO {
	/// <summary>
	/// Struct witch stores the keyboard key and the controler key (both optional)
	/// </summary>
	struct KeyBind {
		/// <summary>
		/// Keyboard key to match bind
		/// </summary>
		KB_KEY keyboardKey = 0x0;

		/// <summary>
		/// Controler key to match bind
		/// </summary>
		HC_KEY controlerKey = 0x0;
	};

	/// <summary>
	/// Listener interface for key events
	/// </summary>
	class IKeyListener {
		public:
			/// <summary>
			/// Event callback for a key event
			/// </summary>
			/// <param name="keyboardKey">Input keyboard key (can be null)</param>
			/// <param name="controlerKey">Input controler key (can be null)</param>
			/// <param name="isDown">If the key is been pressed (true) or been release (false)</param>
			virtual void keyEvent(KB_KEY keyboardKey, HC_KEY controlerKey, BOOL isDown) = 0;
	};

	/// <summary>
	/// Assignment of a bind and a callback
	/// </summary>
	struct KeyAssign {
		/// <summary>
		/// Key binding
		/// </summary>
		KeyBind keys;

		/// <summary>
		/// Target callback
		/// </summary>
		IKeyListener* ptrListener;
	};

}