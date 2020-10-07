#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>
#include <UGF12/Util/Time/Time.h>
#include <UGF12/Util/Time/HPC.h>

#define STOP_WATCH_OP_START GxUtil::StopWatchOperation::START
#define STOP_WATCH_OP_STOP	GxUtil::StopWatchOperation::STOP
#define STOP_WATCH_OP_RESET	GxUtil::StopWatchOperation::RESET

namespace GxUtil {
	
	enum class StopWatchOperation : UINT {
		START  = 1,
		STOP   = 0,
		RESET  = 2,
	};

	class StopWatch {
		public:
			/// <summary>
			/// Create stop watch
			/// </summary>
			/// <param name="startImidiatly">Indicates if the stop watch should start</param>
			StopWatch(BOOL startImidiatly = FALSE);

			/// <summary>
			/// Start the timer
			/// </summary>
			void start();

			/// <summary>
			/// Stop the timer
			/// </summary>
			void stop();

			/// <summary>
			/// Reset the timer
			/// </summary>
			void reset();

			/// <summary>
			/// Perform an operation on the timer
			/// </summary>
			/// <param name="op">To be performed operation</param>
			void performTimerOperation(StopWatchOperation op);

			/// <summary>
			/// Perform an operation on the timer
			/// </summary>
			/// <param name="i">i Typed Operation</param>
			void performTimerOperation(INT i);

			/// <summary>
			/// Get the eleapsed time
			/// </summary>
			/// <returns>in Microseconds (INT64)</returns>
			UINT64 getElapsedUs();

			/// <summary>
			/// Get the eleapsed time
			/// </summary>
			/// <returns>in Millisecondy (FLOAT)</returns>
			FLOAT getElapsedMs();

			// Delete unsupported
			StopWatch(const StopWatch&) = delete;
			void operator=(const StopWatch&) = delete;
		private:
			/// <summary>
			/// Indicates if the stopwatch is running
			/// </summary>
			BOOL m_bRunning = FALSE;

			/// <summary>
			/// Time stamp of current runing stat point
			/// </summary>
			GxUtil::TIMESTAMP m_tsStart = 0;
			
			/// <summary>
			/// Sum of all stop - start conditions during livetime (will be reset on reset)
			/// </summary>
			UINT64 m_uiElapsed = 0;
	};
}