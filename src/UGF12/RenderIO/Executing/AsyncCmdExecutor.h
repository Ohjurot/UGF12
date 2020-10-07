#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>
#include <UGF12/Util/Concurent/LockGuard.h>
#include <UGF12/Util/Concurent/SpinLock.h>

#include <UGF12/DirectX/XCmdList.h>

#include <UGF12/RenderIO/Executing/CmdListExecutionFlag.h>
#include <UGF12/RenderIO/Executing/CmdListPool.h>

namespace GxRenderIO {
	struct _AsyncPair {
		GxDirect::XCmdList* ptrCmdList;
		GxRenderIO::CmdListPool* ptrReturnPool;
		GxRenderIO::CmdListExecutionFlag* ptrFlag;
	};

	class AsyncCmdExecutor {
		public:
			/// <summary>
			/// tatic que function
			/// </summary>
			/// <param name="ptrCmdList">Pointer to the command list</param>
			/// <param name="ptrPool">Pointer to the pool</param>
			/// <param name="ptrFlag">Pointer to the flag to be set</param>
			/// <returns>If call was successfull</returns>
			static BOOL queExecution(GxDirect::XCmdList* ptrCmdList, GxRenderIO::CmdListPool* ptrPool, GxRenderIO::CmdListExecutionFlag* ptrFlag);

			/// <summary>
			/// Init pointer
			/// </summary>
			/// <param name="capacity">Capacity of async excutor</param>
			static void init(UINT capacity);

			/// <summary>
			/// Destroys the singelton
			/// </summary>
			static void destroy();

		public:
			/// <summary>
			/// Start execution on command list and store it
			/// </summary>
			/// <param name="ptrCmdList">Pointer to the command list</param>
			/// <param name="ptrPool">Pointer to the pool</param>
			/// <param name="ptrFlag">Pointer to the flag to be set</param>
			/// <returns>If command list could be qued on excutor</returns>
			BOOL queCmdList(GxDirect::XCmdList* ptrCmdList, GxRenderIO::CmdListPool* ptrPool, GxRenderIO::CmdListExecutionFlag* ptrFlag);

			// delete unsupported
			AsyncCmdExecutor(const AsyncCmdExecutor&) = delete;
			void operator=(const AsyncCmdExecutor&) = delete;

		private:
			/// <summary>
			/// Pointer buffer of commands lists
			/// </summary>
			_AsyncPair* m_ptrsCmdLists;

			/// <summary>
			/// Capcity of executor
			/// </summary>
			const UINT m_uiCapacity;

			/// <summary>
			/// Used count
			/// </summary>
			UINT m_uiUsed = 0;

			/// <summary>
			/// Bitset for storing witch slots are used
			/// </summary>
			UINT64* m_ptrsBitset;

			/// <summary>
			/// A index with gives a hint what the next free index could be
			/// </summary>
			UINT64 m_uiNextFreeIndex = 0;

			/// <summary>
			/// Indicates if the thread should exit
			/// </summary>
			BOOL m_bThredShouldExit = FALSE;

			/// <summary>
			/// Thread handle
			/// </summary>
			HANDLE m_hThread = NULL;

		private:
			/// <summary>
			/// 
			/// </summary>
			/// <param name="capacity">Capacity to be set if singleton does not exists </param>
			AsyncCmdExecutor(UINT capacity);

			// Destrcutor to free memory and stop thread
			~AsyncCmdExecutor();

			/// <summary>
			/// Thread entry point for async worker thread
			/// </summary>
			/// <param name="param">Instance of AsyncCmdExecutor disguysed as LPVOID</param>
			/// <returns>Return code</returns>
			static DWORD WINAPI ThreadProc(LPVOID param);

			/// <summary>
			/// Internal thread proc
			/// </summary>
			/// <returns>Return code</returns>
			DWORD __itrlThreadProc();

			/// <summary>
			/// Statick lock
			/// </summary>
			static GxUtil::SpinLock s_lock;
			
			/// <summary>
			/// Static executor
			/// </summary>
			static GxRenderIO::AsyncCmdExecutor* s_ptrExecutor;
	};
}