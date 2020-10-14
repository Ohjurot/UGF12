#pragma once
#include "pch.h"

namespace GxUtil {
	/// <summary>
	/// Concurent execution counter
	/// </summary>
	class ExecutionCounter {
		public:
			/// <summary>
			/// Create execution counters
			/// </summary>
			/// <param name="startValue">Numbers of signals required for done condition</param>
			ExecutionCounter(LONG64 startValue);

			/// <summary>
			/// Start execution
			/// </summary>
			void execute();

			/// <summary>
			/// Signals end of execution
			/// </summary>
			void signal();

			/// <summary>
			/// Test if execution should start
			/// </summary>
			/// <param name="ptrTrargetLastIndex">Pointer to last index of worker</param>
			/// <returns>If execution should start</returns>
			BOOL requireExecute(volatile LONG64* ptrTrargetLastIndex);

			/// <summary>
			/// Check if execution is done
			/// </summary>
			/// <returns>If has been signaled N times (N specified in consturct) after last execute</returns>
			BOOL executionDone();

			// Delete unused
			ExecutionCounter(const ExecutionCounter&) = delete;
			void operator==(const ExecutionCounter&) = delete;
		private:
			/// <summary>
			/// Start value on execution
			/// </summary>
			const LONG64 m_counterStartValue;

			/// <summary>
			/// Executions left counter
			/// </summary>
			volatile LONG64 m_counterValue = 0;

			/// <summary>
			/// Index of current execution
			/// </summary>
			volatile LONG64 m_executionIndex = 0;
	};
}