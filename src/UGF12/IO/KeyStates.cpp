#include "pch.h"
#include "KeyStates.h"

void GxIO::KeyState::update() {
	// Controler state
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	// Quer controler and go on if succeed
	if (XInputGetState(0, &state) == ERROR_SUCCESS) {
		// Check for fast return
		if (m_xStateOld.dwPacketNumber == state.dwPacketNumber) {
			return;
		}

		// Find difference (BUTTON)
		for (UINT i = 0; i < 14; i++) {
			// Compute bitmask
			WORD bitMask = (1 << i);

			// Check if differ
			if ((m_xStateOld.Gamepad.wButtons & bitMask) != (state.Gamepad.wButtons & bitMask)) {
				// Check if is down
				BOOL isDown = ((state.Gamepad.wButtons & bitMask) != 0x0);

				// Find assign
				for (auto it = m_vecAssignments.begin(); it != m_vecAssignments.end(); it++) {
					// Compare key
					if ((*it).keys.controlerKey == (HC_KEY)bitMask) {
						// Call the callback
						(*it).ptrListener->keyEvent(NULL, (HC_KEY)bitMask, isDown);
					}
				}
			}
		}

		// Update internal state
		m_xStateOld = state;
	}
}

void GxIO::KeyState::keyDown(KB_KEY key){
	// Find assign
	for (auto it = m_vecAssignments.begin(); it != m_vecAssignments.end(); it++) {
		// Compare key
		if ((*it).keys.keyboardKey == (KB_KEY)key) {
			// Call the callback
			(*it).ptrListener->keyEvent(key, NULL, TRUE);
		}
	}
}

void GxIO::KeyState::keyUp(KB_KEY key) {
	// Find assign
	for (auto it = m_vecAssignments.begin(); it != m_vecAssignments.end(); it++) {
		// Compare key
		if ((*it).keys.keyboardKey == (KB_KEY)key) {
			// Call the callback
			(*it).ptrListener->keyEvent(key, NULL, FALSE);
		}
	}
}

void GxIO::KeyState::clearKeys(){
	// Find assign
	for (auto it = m_vecAssignments.begin(); it != m_vecAssignments.end(); it++) {
		// Call callback
		(*it).ptrListener->keyEvent(NULL, NULL, FALSE);
	}
}

void GxIO::KeyState::addCallback(KB_KEY keyboard, HC_KEY controler, GxIO::IKeyListener* ptrListener) {
	// Create new assignment
	GxIO::KeyAssign assign;

	// Write in data
	assign.ptrListener = ptrListener;
	assign.keys.keyboardKey = keyboard;
	assign.keys.controlerKey = controler;

	// Add to collection
	m_vecAssignments.push_back(assign);
}
