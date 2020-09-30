#pragma once
#include "pch.h"

#include <UGF12/DirectX/XContext.h>
#include <UGF12/Util/Exeption.h>

namespace GxDirect {
	class XCmdQueue {
		public:
			/// <summary>
			/// Create a command que on context
			/// </summary>
			/// <param name="ptrContext">Context to create command que on</param>
			XCmdQueue(GxDirect::XContext* ptrContext);

			// Destructor to free memory
			~XCmdQueue();

			/// <summary>
			/// Set a pointer to this command que (dont forget to relase the new pointer sice this function calls AddRef() )
			/// </summary>
			/// <param name="ppQue">Pointer to the pointer to be set</param>
			void getCommandQueue(ID3D12CommandQueue** ppQue);

			/// <summary>
			/// Executes one or many cmd lists on this que
			/// </summary>
			/// <param name="ptrsCmdList">Pointer to list of cmd lists</param>
			/// <param name="listCount">Number of cmd lists to be executed</param>
			/// <returns>The value to wait on if execution of cmd list is critical</returns>
			UINT64 execute(ID3D12CommandList** ptrsCmdList, UINT listCount);
			
			/// <summary>
			/// Checks if a specivice value has already be finished executing
			/// </summary>
			/// <param name="value">Value returned by execute(...)</param>
			/// <returns>If value is fnished on gpu</returns>
			BOOL finished(UINT64 value);

			/// <summary>
			/// Waits until a specific value is reached
			/// </summary>
			/// <param name="value">Value to be reached</param>
			/// <param name="forceSignal">Forces a signal on que for fence bevore executrion</param>
			void waitForValue(UINT64 value, BOOL forceSignal = FALSE);

			/// <summary>
			/// FLushes all commands currently in flight on gpu
			/// </summary>
			void flushGpu();

			// Delete unused
			XCmdQueue(const XCmdQueue&) = delete;
			void operator=(const XCmdQueue&) = delete;
		private:
			/// <summary>
			/// Command que for command execution
			/// </summary>
			ID3D12CommandQueue* m_ptrCmdQue = NULL;

			/// <summary>
			/// Fence for command que signaling
			/// </summary>
			ID3D12Fence* m_ptrFence = NULL;

			/// <summary>
			/// Value of last dispatched call to que
			/// </summary>
			UINT64 m_uiFenceValue = 0;
	};
}