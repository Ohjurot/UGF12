#pragma once
#include "pch.h"

#include <UGF12/Util/Concurent/ILock.h>

namespace GxUtil {
	/// <summary>
	/// Interface for concurent locks
	/// </summary>
	class SpinLock : public GxUtil::ILock {
		public:
			/// <summary>
			/// Constructor to asert released lock
			/// </summary>
			SpinLock();

			/// <summary>
			/// Aquire the lock
			/// </summary>
			void aquire();

			/// <summary>
			/// Try to aquire the lock
			/// </summary>
			/// <returns>If aquesition was successfull</returns>
			BOOL tryAquire();

			/// <summary>
			/// Releases the lock
			/// </summary>
			void release();

			// Delete unsupported
			SpinLock(const SpinLock&) = delete;
			void operator=(const SpinLock&) = delete;
		private:
			/// <summary>
			/// Atomic flag to be locked
			/// </summary>
			std::atomic_flag m_flag;
	};
}