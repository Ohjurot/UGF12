#pragma once
#include "pch.h"

namespace GxUtil {
	/// <summary>
	/// Interface for concurent locks
	/// </summary>
	class ILock {
		public:
			/// <summary>
			/// Aquire the lock
			/// </summary>
			virtual void aquire() = 0;

			/// <summary>
			/// Try to aquire the lock
			/// </summary>
			/// <returns>If aquesition was successfull</returns>
			virtual BOOL tryAquire() = 0;

			/// <summary>
			/// Releases the lock
			/// </summary>
			virtual void release() = 0;
	};
}