#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>
#include <UGF12/Util/Concurent/LockGuard.h>
#include <UGF12/Util/Concurent/SpinLock.h>

#include <UGF12/DirectX/XCmdList.h>
#include <UGF12/DirectX/XContext.h>
#include <UGF12/DirectX/XCmdQueue.h>

namespace GxRenderIO {
	
	/// <summary>
	/// Pool for managing command lists
	/// </summary>
	class CmdListPool {
		public:
			/// <summary>
			/// Create the command list pool
			/// </summary>
			/// <param name="capacity">Maximum capacity of pool</param>
			/// <param name="initLists">Indicates if the pointers should be inited</param>
			/// <param name="ptrContext">Pointer to conext (only required if initList = TRUE)</param>
			/// <param name="ptrQue">Pointer to command que (only required if initList = TRUE)</param>
			CmdListPool(UINT capacity, BOOL initLists, GxDirect::XContext* ptrContext = nullptr, GxDirect::XCmdQueue* ptrQue = nullptr);

			// Destructor for pool
			~CmdListPool();

			/// <summary>
			/// Pull a command list from the pool
			/// </summary>
			/// <param name="ppCmdList">Pointer to pointer to be set</param>
			/// <returns>If pointer has been set</returns>
			BOOL pull(GxDirect::XCmdList** ppCmdList);

			/// <summary>
			/// Pushes a command list to the pool
			/// </summary>
			/// <param name="ptrCmdList">List to be pushed</param>
			/// <returns>If list could be pushed</returns>
			BOOL push(GxDirect::XCmdList* ptrCmdList);

			/// <summary>
			/// Lock the internal guard and retriev access to internal object
			/// </summary>
			/// <param name="ppArrayBegin">Pointer to the external pointer to first array element</param>
			/// <param name="ppUiStock">Pointer to the external pointer for stock</param>
			/// <returns>0 if array could not be locked; > 0 if array could be lock. Return value is capacity of pool</returns>
			UINT getPointer_lock(GxDirect::XCmdList** ppArrayBegin, UINT** ppUiStock);

			/// <summary>
			/// Unlocks the array (must only be called after a getPointer_lock() call)
			/// </summary>
			void getPointer_unlock();

		private:
			/// <summary>
			/// Lock for guarding internal objects
			/// </summary>
			GxUtil::SpinLock m_lock;

			/// <summary>
			/// Array of managed command lists
			/// </summary>
			GxDirect::XCmdList** m_ptrCommandLists;

			/// <summary>
			/// Maximum capacity of pool
			/// </summary>
			const UINT m_uiCapacity;

			/// <summary>
			/// Current stock of pool
			/// </summary>
			UINT m_uiStock;
	};
}