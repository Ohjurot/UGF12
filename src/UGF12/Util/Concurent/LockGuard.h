#pragma once
#include "pch.h"

#include <UGF12/Util/Concurent/ILock.h>

namespace GxUtil {
	/// <summary>
	/// An automatic guard witch locks and release on its own scope based
	/// </summary>
	class LockGuard {
		public:
			/// <summary>
			/// Create auto guard lock
			/// </summary>
			/// <param name="ptrLock">Pointer to lock to be used</param>
			LockGuard(GxUtil::ILock* ptrLock);

			// Destructor to release lock
			~LockGuard();

			// Delete unsupported
			LockGuard(const LockGuard&) = delete;
			void operator=(const LockGuard&) = delete;
		private:
			GxUtil::ILock* m_ptrLock;
	};
}