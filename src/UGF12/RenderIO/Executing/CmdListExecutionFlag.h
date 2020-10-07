#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>
#include <UGF12/Util/Concurent/LockGuard.h>
#include <UGF12/Util/Concurent/SpinLock.h>

#include <UGF12/DirectX/XCmdList.h>
#include <UGF12/DirectX/XContext.h>
#include <UGF12/DirectX/XCmdQueue.h>

#include <UGF12/RenderIO/Executing/CmdListPool.h>

namespace GxRenderIO {
	/// <summary>
	/// Handle for checking if a list was executed successfully
	/// </summary>
	class CmdListExecutionFlag {
		public:
			/// <summary>
			/// Constructor to initialy clear the flag
			/// </summary>
			CmdListExecutionFlag();
			
			/// <summary>
			/// Read the current completion value
			/// </summary>
			/// <returns>If List was executed successfully</returns>
			BOOL getCompletedValue();

			/// <summary>
			/// Waits until flag is set
			/// </summary>
			void waitForCompletion();

			/// <summary>
			/// Set the handle to be completed
			/// </summary>
			void __setCompleted();

		private:
			/// <summary>
			/// Flag for storing the excution state
			/// </summary>
			std::atomic_flag m_flag;
	};
}