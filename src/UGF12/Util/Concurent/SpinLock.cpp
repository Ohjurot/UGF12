#include "pch.h"
#include "SpinLock.h"

GxUtil::SpinLock::SpinLock() {
	// Clear is the init state
	m_flag.clear(std::memory_order_release);
}

void GxUtil::SpinLock::aquire() {
	// Try to aquire until succeed
	while (!tryAquire()) {
		// Pause thread
		_mm_pause();
	}
}

BOOL GxUtil::SpinLock::tryAquire() {
	// Set the flag. If already set the aquisiton failed
	return !(m_flag.test_and_set(std::memory_order_acquire));
}

void GxUtil::SpinLock::release(){
	// Rlease the lock / flag
	m_flag.clear(std::memory_order_release);
}
