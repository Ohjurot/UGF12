#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>
#include <UGF12/Util/Concurent/LockGuard.h>
#include <UGF12/Util/Concurent/SpinLock.h>

#include <UGF12/DirectX/XCmdList.h>
#include <UGF12/DirectX/XContext.h>
#include <UGF12/DirectX/XCmdQueue.h>
#include <UGF12/DirectX/XCounted.h>

#include <UGF12/RenderIO/Executing/CmdListPool.h>
#include <UGF12/RenderIO/Executing/CmdListExecutionFlag.h>
#include <UGF12/RenderIO/Executing/AsyncCmdExecutor.h>

namespace GxRenderIO {

	/// <summary>
	/// Manger who manges command routing
	/// </summary>
	class CmdListManger : public GxDirect::XCounted {
		public:
			/// <summary>
			/// Enum for the disptach mode
			/// </summary>
			enum class DispatchMode : UINT {
				RESULT_REQUIRED_THIS_FRAME	= 0,	// Dispatched when all command list have been build
				RESULT_REQUIRED_NOW			= 1,	// Disptached now (will block)
				RESULT_REQUIRED_LATER		= 2,	// Dispatched now (will not block -> aditional hanlde required)
			};

		public:
			/// <summary>
			/// Constructor for creating the manager
			/// </summary>
			/// <param name="ptrContext">Pointer to context</param>
			/// <param name="ptrCommandQue">Pointer to command que</param>
			/// <param name="commandListCount">Number of command lists to be managed</param>
			CmdListManger(GxDirect::XContext* ptrContext, GxDirect::XCmdQueue* ptrCommandQue, UINT commandListCount);

			// Destructor to decrement reference
			~CmdListManger();

			/// <summary>
			/// Get a list from the idle pool
			/// </summary>
			/// <param name="ppCmdList">Pointer to pointer to be set</param>
			/// <returns>If idle pool had a list avalible</returns>
			BOOL getFirstList(GxDirect::XCmdList** ppCmdList);

			/// <summary>
			/// Swap lists and exeute the old one
			/// </summary>
			/// <param name="ppCmdList">pointer to pointer to command list to persform opertion</param>
			/// <param name="disptachMode">Execution mode</param>
			/// <param name="ptrFlag">Flag to be set when execution finishes (only with disptachMode==RESULT_REQUIRED_LATER)</param>
			/// <returns>If list could be executed</returns>
			BOOL swapExecute(GxDirect::XCmdList** ppCmdList, GxRenderIO::CmdListManger::DispatchMode disptachMode, GxRenderIO::CmdListExecutionFlag* ptrFlag = nullptr);

			/// <summary>
			/// Return a list to the manger
			/// </summary>
			/// <param name="ptrList">Pointer to the list to be returned. List need to be in open state</param>
			/// <returns>If list could be returned</returns>
			BOOL returnList(GxDirect::XCmdList* ptrList);

			/// <summary>
			/// Execute Frame based command lists
			/// </summary>
			void executeCommandLists();

			/// <summary>
			/// Wait for frame based commands lists
			/// </summary>
			void waitForCommandLists();

			// Delete unsupported
			CmdListManger(const CmdListManger&) = delete;
			void operator=(const CmdListManger&) = delete;
		private:
			/// <summary>
			/// Enum for the frame pools state
			/// </summary>
			enum class FramePoolState : UINT {
				STATE_RECORD,
				STATE_EXECUTING,
			};

		private:
			/// <summary>
			/// Ref pointer to command que
			/// </summary>
			GxDirect::XCmdQueue* m_ptrCmdQue;

			/// <summary>
			/// Pool of idle command lists
			/// </summary>
			GxRenderIO::CmdListPool m_clPoolIdle;

			/// <summary>
			/// Pool of command lists staged for current frame
			/// </summary>
			GxRenderIO::CmdListPool m_clPoolFrame;

			/// <summary>
			/// State of the frame pool
			/// </summary>
			FramePoolState m_psFramePoolState = FramePoolState::STATE_RECORD;

			/// <summary>
			/// Pointer to frames stock count
			/// </summary>
			UINT* m_ptrFramesStockCount = 0;

			/// <summary>
			/// Pointer to frames array
			/// </summary>
			GxDirect::XCmdList** m_ptrsFramesLists = nullptr;

			/// <summary>
			/// Internal array for frame execution
			/// </summary>
			ID3D12CommandList** m_ptrExecuteArray = nullptr;

			/// <summary>
			/// Execution value for commands in flight
			/// </summary>
			UINT64 m_uiExecutionValue = 0;
	};


}