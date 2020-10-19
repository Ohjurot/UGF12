#pragma once
#include "pch.h"

#include <UGF12/Util/TSysInfo.h>
#include <UGF12/Util/Exeption.h>

#include <UGF12/DirectX/XContext.h>

namespace GxDirect {
	namespace FeaturSupport {
		/// <summary>
		/// Container for D3D12_FEATURE_D3D12_OPTIONS5
		/// </summary>
		class FS_Options5 {
			public:
				/// <summary>
				/// Create options based on a context
				/// </summary>
				/// <param name="ptrContext">context to query</param>
				FS_Options5(GxDirect::XContext* ptrContext);

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
				/// Flag that indicates if thees options require TiledResourcTier >= 3 (Options0 / D3D12_FEATURE_DATA_D3D12_OPTIONS )
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getSRVOnlyTiledResourceTier3();

				/// <summary>
				/// Render passes tier
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(D3D12_RENDER_PASS_TIER)& getRenderPassesTier();

				/// <summary>
				/// Raytracing tier
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(D3D12_RAYTRACING_TIER)& getRaytracingTier();

			private:
				/// <summary>
				/// Query result
				/// </summary>
				HRESULT m_hrQueryResult;

				/// <summary>
				/// Flag that indicates if thees options require TiledResourcTier >= 3 (Options0 / D3D12_FEATURE_DATA_D3D12_OPTIONS )
				/// </summary>
				TSysInfo(BOOL) m_siSRVOnlyTiledResourceTier3 = FALSE;

				/// <summary>
				/// Render passes tier
				/// </summary>
				TSysInfo(D3D12_RENDER_PASS_TIER) m_siRenderPassesTier = D3D12_RENDER_PASS_TIER_0;
				
				/// <summary>
				/// Raytracing tier
				/// </summary>
				TSysInfo(D3D12_RAYTRACING_TIER) m_siRaytracingTier = D3D12_RAYTRACING_TIER_NOT_SUPPORTED;
		};
	}
}