#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>
#include <UGF12/DirectX/XContext.h>
#include <UGF12/DirectX/XCmdQueue.h>

namespace GxDirect {
	/// <summary>
	/// Enum for storing a state of a command list
	/// </summary>
	enum class CmdListState : UINT {
		CMD_LIST_INVALID	= 0,
		CMD_LIST_OPEN		= 1,
		CMD_LIST_CLOSED		= 2,
		CMD_LIST_EXECUTING	= 3,
	};

	/// <summary>
	/// Command list (Graffic cmd list) for command listing
	/// </summary>
	class XCmdList {
		public:
			/// <summary>
			/// Construct list on que and context
			/// </summary>
			/// <param name="ptrContext">Pointer to context</param>
			/// <param name="ptrCmdQue">Pointer to que</param>
			XCmdList(GxDirect::XContext* ptrContext, GxDirect::XCmdQueue* ptrCmdQue);

			// Destructor to free data
			~XCmdList();

			/// <summary>
			/// Gets the pointer to the command list (No addref)
			/// </summary>
			/// <param name="doAssertState">Indicates if the state should be check. Function will block when list is in flight untli free</param>
			/// <returns>Pointer to OPEN (when doAssertState == TRUE) command list for command recording</returns>
			ID3D12GraphicsCommandList* get(BOOL doAssertState = TRUE);

			/// <summary>
			/// Get the state of the command list
			/// </summary>
			/// <returns>State of command list</returns>
			GxDirect::CmdListState getState();

			/// <summary>
			/// Closes the command list (New state will be CMD_LIST_CLOSED; Only excutes when state is not equal to that)
			/// </summary>
			void close();

			/// <summary>
			/// Executes the command list (New state will be CMD_LIST_EXECUTING; Only excutes when state is not equal to that)
			/// </summary>
			void execute();

			/// <summary>
			/// Waits for the command list (New state will be CMD_LIST_OPEN; Only excutes when state is not equal to that)
			/// </summary>
			void wait();

			/// <summary>
			/// Indicate that gpu has been flush 
			/// </summary>
			void flush();

			/// <summary>
			/// Get the recoradbale state of the command list
			/// </summary>
			/// <returns>If list can record commands</returns>
			BOOL recordable();

			// Delete unused
			XCmdList(const XCmdList&) = delete;
			void operator=(const XCmdList&) = delete;
		private:
			/// <summary>
			/// Pointer to cmd que for execution
			/// </summary>
			GxDirect::XCmdQueue* m_ptrCmdQue = NULL;

			/// <summary>
			/// Command allocator for command list
			/// </summary>
			ID3D12CommandAllocator* m_ptrCmdAlloc = NULL;

			/// <summary>
			/// Internal cmd list object
			/// </summary>
			ID3D12GraphicsCommandList* m_ptrCmdList = NULL;

			/// <summary>
			/// State of the command list
			/// </summary>
			CmdListState m_state = CmdListState::CMD_LIST_INVALID;

			/// <summary>
			/// Value to wait for on execution
			/// </summary>
			UINT m_uiExecutionValue = 0;
	};
}