#pragma once
#include "pch.h"

#include <UGF12/Util/TSysInfo.h>
#include <UGF12/Util/Exeption.h>

#include <UGF12/DirectX/XContext.h>

namespace GxDirect {
	namespace FeaturSupport {
		/// <summary>
		/// Container for D3D12_FEATURE_D3D12_OPTIONS1
		/// </summary>
		class FS_Options1 {
			public:
				/// <summary>
				/// Create options based on a context
				/// </summary>
				/// <param name="ptrContext">context to query</param>
				FS_Options1(GxDirect::XContext* ptrContext);

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
				/// Indicates if the adapter supportsd HSLS 6.0 wave operations 
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getWaveOps();

				/// <summary>
				/// Wavefront size / Warp width
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(UINT)& getWaveLaneCount();

				/// <summary>
				/// Number of cores of the gpu (SIMD lanes)
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(UINT)& getTotalLaneCount();

				/// <summary>
				/// Indicates that the adapter supports 64-Bit integer operations
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getInt64ShaderOps();

			private:
				/// <summary>
				/// Query result
				/// </summary>
				HRESULT m_hrQueryResult;

				/// <summary>
				/// Indicates if the adapter supportsd HSLS 6.0 wave operations
				/// </summary>
				TSysInfo(BOOL) m_siWaveOps = FALSE;

				/// <summary>
				/// Wavefront size / Warp width
				/// </summary>
				TSysInfo(UINT) m_siWaveLaneCountMin = 0;

				/// <summary>
				/// Number of cores of the gpu (SIMD lanes)
				/// </summary>
				TSysInfo(UINT) m_siTotalLaneCount = 0;

				/// <summary>
				/// Indicates that the adapter supports 64-Bit integer operations
				/// </summary>
				TSysInfo(BOOL) m_siInt64ShaderOps = FALSE;
		};
	}
}