#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>

namespace GxDirect {
	/// <summary>
	/// Base class for ref counting
	/// </summary>
	class XCounted {
		public:
			// Destructor
			~XCounted() noexcept(false);

			/// <summary>
			/// Increment Ref Count
			/// </summary>
			void IncRef();
			
			/// <summary>
			/// Decrement Ref Count;
			/// </summary>
			void DecRef();

		private:
			/// <summary>
			/// Ref count for external class refernce
			/// </summary>
			UINT m_uiRefCount = 0;
	};
}