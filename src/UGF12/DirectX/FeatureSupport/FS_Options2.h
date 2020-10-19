#pragma once
#include "pch.h"

#include <UGF12/Util/TSysInfo.h>
#include <UGF12/Util/Exeption.h>

#include <UGF12/DirectX/XContext.h>

namespace GxDirect {
	namespace FeaturSupport {
		/// <summary>
		/// Container for D3D12_FEATURE_D3D12_OPTIONS2
		/// </summary>
		class FS_Options2 {
			public:
				/// <summary>
				/// Create options based on a context
				/// </summary>
				/// <param name="ptrContext">context to query</param>
				FS_Options2(GxDirect::XContext* ptrContext);

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
				/// Indicates depth bounds test support
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getDepthBoundsTestSupported();

				/// <summary>w
				/// Number of sample positions supported
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(D3D12_PROGRAMMABLE_SAMPLE_POSITIONS_TIER)& getProgrammableSamplePositionsTier();

			private:
				/// <summary>
				/// Query result
				/// </summary>
				HRESULT m_hrQueryResult;

				/// <summary>
				/// Indicates depth bounds test support
				/// </summary>
				TSysInfo(BOOL) m_siDepthBoundsTestSupported = FALSE;

				/// <summary>
				/// Number of sample positions supported
				/// </summary>
				TSysInfo(D3D12_PROGRAMMABLE_SAMPLE_POSITIONS_TIER) m_siProgrammableSamplePositionsTier = D3D12_PROGRAMMABLE_SAMPLE_POSITIONS_TIER_NOT_SUPPORTED;

		};
	}
}