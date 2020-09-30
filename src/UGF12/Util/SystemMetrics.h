#pragma once
#include "pch.h"

namespace GxUtil {
	/// <summary>
	/// Singelton witch hold all all system metrics
	/// </summary>
	class SystemMetrics {
		public:
			/// <summary>
			/// Get the cpu core currently present (Logical cores)
			/// </summary>
			/// <returns>Core count</returns>
			static UINT getCpuCoresCount();

		private:
			/// <summary>
			/// Singelton constructor
			/// </summary>
			SystemMetrics();

			/// <summary>
			/// Singelton instance
			/// </summary>
			static SystemMetrics s_instance;

		private:
			/// <summary>
			/// CPU Corese in system (Logical cores)
			/// </summary>
			UINT m_uiCpuCoresCount;
	};
}