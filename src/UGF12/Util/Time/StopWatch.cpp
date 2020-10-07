#include "pch.h"
#include "StopWatch.h"

GxUtil::StopWatch::StopWatch(BOOL startImidiatly) {
	// Start time if required
	if (startImidiatly) {
		performTimerOperation(STOP_WATCH_OP_START);
	}
}

void GxUtil::StopWatch::start() {
	performTimerOperation(STOP_WATCH_OP_START);
}

void GxUtil::StopWatch::stop(){
	performTimerOperation(STOP_WATCH_OP_STOP);
}

void GxUtil::StopWatch::reset() {
	performTimerOperation(STOP_WATCH_OP_RESET);
}

void GxUtil::StopWatch::performTimerOperation(StopWatchOperation op) {
	switch (op) {
		// Start the timer
		case StopWatchOperation::START:
			// If not running set running and set start point
			if (!m_bRunning) {
				m_tsStart = GxUtil::HPC::queryCounter();
				m_bRunning = TRUE;
			}
			break;
		
		// Stop the timer
		case StopWatchOperation::STOP:
			// If running stop running and add to elapsed
			if (m_bRunning) {
				m_uiElapsed += (GxUtil::HPC::queryCounter() - m_tsStart);
				m_bRunning = FALSE;
			}
			break;

		// Reset the timer
		case StopWatchOperation::RESET:
			// Reset all internal states
			m_bRunning = FALSE;
			m_uiElapsed = 0;
			break;
	}
}

void GxUtil::StopWatch::performTimerOperation(INT i) {
	// Cast to stop watch operation
	performTimerOperation((GxUtil::StopWatchOperation)i);
}

UINT64 GxUtil::StopWatch::getElapsedUs() {
	// Copy storage elapsed
	UINT64 returnValue = m_uiElapsed;

	// If running add current run
	if (m_bRunning) {
		returnValue += (GxUtil::HPC::queryCounter() - m_tsStart);
	}

	// Return right value
	return returnValue;
}

FLOAT GxUtil::StopWatch::getElapsedMs() {
	// Convert to float (percision lost)
	return getElapsedUs() / 1000.0F;
}
