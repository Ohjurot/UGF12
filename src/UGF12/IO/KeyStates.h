#pragma once
#include "pch.h"

#include <UGF12/IO/KeyBind.h>

namespace GxIO {
	/// <summary>
	/// Clase witch holds the keystate of all keys abd bindings
	/// </summary>
	class KeyState {
		public:
			/// <summary>
			/// Update XInput
			/// </summary>
			void update();

			/// <summary>
			/// Notify called by window for a key down
			/// </summary>
			/// <param name="key">Input keyboard key</param>
			void keyDown(KB_KEY key);

			/// <summary>
			/// Notify called by window for a key up
			/// </summary>
			/// <param name="key">Input keyboard key</param>
			void keyUp(KB_KEY key);

			/// <summary>
			/// Clear function call by window on deactivate
			/// </summary>
			void clearKeys();

			/// <summary>
			/// Add a static callback to the list of callbacks
			/// </summary>
			/// <param name="keyboard">Key of the keyboard</param>
			/// <param name="controler">Key of the controler</param>
			/// <param name="ptrListener">Object who wants to listen</param>
			void addCallback(KB_KEY keyboard, HC_KEY controler, GxIO::IKeyListener* ptrListener);
		private:
			/// <summary>
			/// Assignments to be called
			/// </summary>
			std::vector<GxIO::KeyAssign> m_vecAssignments;

			/// <summary>
			/// Old ximput state
			/// </summary>
			XINPUT_STATE m_xStateOld = {};
	};
}