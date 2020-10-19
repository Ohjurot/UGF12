#pragma once
#include "pch.h"

#include <UGF12/Util/TSysInfo.h>
#include <UGF12/Util/Exeption.h>

#include <UGF12/DirectX/XContext.h>

namespace GxDirect {
	namespace FeaturSupport {
		/// <summary>
		/// Container for D3D12_FEATURE_ARCHITECTURE1
		/// </summary>
		class FS_Architecture1 {
			public:
				/// <summary>
				/// Create options based on a context
				/// </summary>
				/// <param name="ptrContext">context to query</param>
				FS_Architecture1(GxDirect::XContext* ptrContext);

				/// <summary>
				/// Retrive the result of the query
				/// </summary>
				/// <returns>Result of query</returns>
				HRESULT getQueryResult();

				/// <summary>
				/// Gennerates the text representation for feature support (Will fail on Shipping configuration!)
				/// </summary>
				void gennerateTextRepresentation();

			public:
				/// <summary>
				/// Bitmask indicating relevant nodes in multi node system
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(UINT)& getNodeIndex();

				/// <summary>
				/// Indicates if the hardware supports tile based rendering
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getTileBasedRenderer();

				/// <summary>
				/// Indicates that the hardware is UMA (Unified Memory Architecture)
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getUMA();

				/// <summary>
				/// Indicates that the hardware is cache coherent UMA (Unified Memory Architecture)
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getCacheCoherentUMA();

				/// <summary>
				/// Indicates that the hardware supports MMU (Memory Management Unit)
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getIsolatedMMU();

			private:
				/// <summary>
				/// Query result
				/// </summary>
				HRESULT m_hrQueryResult;

				/// <summary>
				/// Bitmask indicating relevant nodes in multi node system
				/// </summary>
				TSysInfo(UINT) m_siNodeIndex = 0x0;

				/// <summary>
				/// Indicates if the hardware supports tile based rendering
				/// </summary>
				TSysInfo(BOOL) m_siTileBasedRenderer = FALSE;

				/// <summary>
				/// Indicates that the hardware is UMA (Unified Memory Architecture)
				/// </summary>
				TSysInfo(BOOL) m_siUMA = FALSE;

				/// <summary>
				/// Indicates that the hardware is cache coherent UMA (Unified Memory Architecture)
				/// </summary>
				TSysInfo(BOOL) m_siCacheCoherentUMA = FALSE;

				/// <summary>
				/// Indicates that the hardware supports MMU (Memory Management Unit)
				/// </summary>
				TSysInfo(BOOL) m_siIsolatedMMU = FALSE;
			};
	}
}