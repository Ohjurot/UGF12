#include "pch.h"
#include "ExecutionCounter.h"

GxUtil::ExecutionCounter::ExecutionCounter(LONG64 startValue) :
	m_counterStartValue(startValue)
{}

void GxUtil::ExecutionCounter::execute(){
	// Set counter
	InterlockedExchange64(&m_counterValue, m_counterStartValue);
	
	// Set index
	LONG64 lastValue;
	do {
		lastValue = m_executionIndex;
	} while (InterlockedCompareExchange64(&m_executionIndex, (lastValue + 1) % LONG64_MAX, lastValue) != lastValue);
}

void GxUtil::ExecutionCounter::signal(){
	// Decrement counter
	InterlockedDecrement64(&m_counterValue);
}

BOOL GxUtil::ExecutionCounter::requireExecute(volatile LONG64* ptrTrargetLastIndex) {
	// Check if index dont match
	if ((*ptrTrargetLastIndex) != m_executionIndex) {
		// Set pointer
		*ptrTrargetLastIndex = m_executionIndex;

		// Start execution
		return TRUE;
	}
	
	// No execution required
	return FALSE;
}

BOOL GxUtil::ExecutionCounter::executionDone() {
	return (m_counterValue == 0);
}
