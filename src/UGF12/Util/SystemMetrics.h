#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>

namespace GxUtil {
	/// <summary>
	/// Singelton witch hold all all system metrics
	/// </summary>
	class SystemMetrics {
		public:
			/// <summary>
			/// CPU Cache descriptor
			/// </summary>
			struct CpuCache {
				/// <summary>
				/// Cache level (1, 2, 3 - L1, L2, L3)
				/// </summary>
				UINT level = 0;

				/// <summary>
				/// Cache size
				/// </summary>
				UINT size = 0;

				/// <summary>
				/// Cache count
				/// </summary>
				UINT count = 0;

				/// <summary>
				/// Type of cache
				/// </summary>
				PROCESSOR_CACHE_TYPE type = CacheUnified;

				/// <summary>
				/// Translated string fore cache size
				/// </summary>
				std::string translatedString;
			};

		public:
			/// <summary>
			/// Get total cpu count
			/// </summary>
			/// <returns></returns>
			static UINT getCpuCount();

			/// <summary>
			/// Get the cpu core currently present (Logical cores)
			/// </summary>
			/// <returns>Core count</returns>
			static UINT getCpuCoresCount();

			/// <summary>
			/// Get the logical cpu core count
			/// </summary>
			/// <returns></returns>
			static UINT getCpuLogicalCoreCount();

			/// <summary>
			/// Get the number of cpu caches count
			/// </summary>
			/// <returns>Number of caches existing</returns>
			static UINT getCacheCount();

			/// <summary>
			/// Get a CPU Cache entry
			/// </summary>
			/// <param name="index">Index of entry</param>
			/// <returns>CpuCache</returns>
			static GxUtil::SystemMetrics::CpuCache getCpuCache(UINT index);

			/// <summary>
			/// Get the total pyhsical memory installed
			/// </summary>
			/// <returns>System memory in KiB</returns>
			static UINT64 getSystemMemory();

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
			/// Total CPU count
			/// </summary>
			UINT m_uiCpuCount;

			/// <summary>
			/// CPU Corese in system (Logical cores)
			/// </summary>
			UINT m_uiCpuCoresCount;

			/// <summary>
			/// Logical core count
			/// </summary>
			UINT m_uiCpuLogicalCoresCount;

			/// <summary>
			/// System total memory
			/// </summary>
			UINT64 m_uiSystemMemory;

			/// <summary>
			/// Total caches
			/// </summary>
			std::vector<SystemMetrics::CpuCache> m_vecCaches;
	};
}