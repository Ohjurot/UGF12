#include "pch.h"
#include "CmdListExecutionFlag.h"

GxRenderIO::CmdListExecutionFlag::CmdListExecutionFlag() {
	// Inital clear the flag
	m_flag.clear(std::memory_order_release);
}

BOOL GxRenderIO::CmdListExecutionFlag::getCompletedValue() {
	// Test for value
	return m_flag.test();
}

void GxRenderIO::CmdListExecutionFlag::__setCompleted(){
	// Set the flag
	m_flag.test_and_set(std::memory_order_acquire);
}
