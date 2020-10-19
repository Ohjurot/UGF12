#pragma once
#include "pch.h"

#include <UGF12/Util/TSysInfo.h>
#include <UGF12/Util/Exeption.h>

#include <UGF12/DirectX/XContext.h>

namespace GxDirect {
	namespace FeaturSupport {
		/// <summary>
		/// Container for D3D12_FEATURE_D3D12_OPTIONS6
		/// </summary>
		class FS_Options6 {
			public:
				/// <summary>
				/// Create options based on a context
				/// </summary>
				/// <param name="ptrContext">context to query</param>
				FS_Options6(GxDirect::XContext* ptrContext);

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
				/// https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_feature_data_d3d12_options6
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getAdditionalShadingRatesSupported();
					
				/// <summary>
				/// https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_feature_data_d3d12_options6 
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getPerPrimitiveShadingRateSupportedWithViewportIndexing();
					
				/// <summary>
				/// VSR Tier
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(D3D12_VARIABLE_SHADING_RATE_TIER)& getVariableShadingRateTier();
					
				/// <summary>
				/// VSR Tile width in px
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(UINT)& getShadingRateImageTileSize();
					
				/// <summary>
				/// Indicates if background processing is supported
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getBackgroundProcessingSupported();

			private:
				/// <summary>
				/// Query result
				/// </summary>
				HRESULT m_hrQueryResult;

				/// <summary>
				/// https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_feature_data_d3d12_options6
				/// </summary>
				TSysInfo(BOOL) m_siAdditionalShadingRatesSupported = FALSE;

				/// <summary>
				/// https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_feature_data_d3d12_options6
				/// </summary>
				TSysInfo(BOOL) m_siPerPrimitiveShadingRateSupportedWithViewportIndexing = FALSE;

				/// <summary>
				/// VSR Tier
				/// </summary>
				TSysInfo(D3D12_VARIABLE_SHADING_RATE_TIER) m_siVariableShadingRateTier = D3D12_VARIABLE_SHADING_RATE_TIER_NOT_SUPPORTED;

				/// <summary>
				/// VSR Tile width in px
				/// </summary>
				TSysInfo(UINT) m_siShadingRateImageTileSize = 0;

				/// <summary>
				/// Indicates if background processing is supported
				/// </summary>
				TSysInfo(BOOL) m_siBackgroundProcessingSupported = FALSE;
		};
	}
}