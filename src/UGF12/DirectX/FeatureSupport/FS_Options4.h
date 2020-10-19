#pragma once
#include "pch.h"

#include <UGF12/Util/TSysInfo.h>
#include <UGF12/Util/Exeption.h>

#include <UGF12/DirectX/XContext.h>

namespace GxDirect {
	namespace FeaturSupport {
		/// <summary>
		/// Container for D3D12_FEATURE_D3D12_OPTIONS4
		/// </summary>
		class FS_Options4 {
			public:
				/// <summary>
				/// Create options based on a context
				/// </summary>
				/// <param name="ptrContext">context to query</param>
				FS_Options4(GxDirect::XContext* ptrContext);

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
				/// Indicates 64KB aligned MSAA texture support
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getMSAA64KBAlignedTextureSupported();

				/// <summary>
				/// Cross api sharing tier
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(D3D12_SHARED_RESOURCE_COMPATIBILITY_TIER)& getSharedResourceCompatibilityTier();

				/// <summary>
				/// Indicates native 16-Bit shader operations support
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getNative16BitShaderOpsSupported();

			private:
				/// <summary>
				/// Query result
				/// </summary>
				HRESULT m_hrQueryResult;

				/// <summary>
				/// Indicates 64KB aligned MSAA texture support
				/// </summary>
				TSysInfo(BOOL) m_siMSAA64KBAlignedTextureSupported = FALSE;

				/// <summary>
				/// Cross api sharing tier
				/// </summary>
				TSysInfo(D3D12_SHARED_RESOURCE_COMPATIBILITY_TIER) m_siSharedResourceCompatibilityTier = D3D12_SHARED_RESOURCE_COMPATIBILITY_TIER_0;

				/// <summary>
				/// Indicates native 16-Bit shader operations support
				/// </summary>
				TSysInfo(BOOL) m_siNative16BitShaderOpsSupported = FALSE;
		};
	}
}