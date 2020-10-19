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
		class FS_Options3 {
			public:
				/// <summary>
				/// Create options based on a context
				/// </summary>
				/// <param name="ptrContext">context to query</param>
				FS_Options3(GxDirect::XContext* ptrContext);

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
				/// Indicates if timestamp queries are supported
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getCopyQueueTimestampQueriesSupported();

				/// <summary>
				/// Indicates if type casting is supported
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getCastingFullyTypedFormatSupported();

				/// <summary>
				/// View instancing tier
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(D3D12_VIEW_INSTANCING_TIER)& getViewInstancingTier();

				/// <summary>
				/// Indicates if barycentrics are supported
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getBarycentricsSupported();
			
			private:
				/// <summary>
				/// Query result
				/// </summary>
				HRESULT m_hrQueryResult;

				/// <summary>
				/// Indicates if timestamp queries are supported
				/// </summary>
				TSysInfo(BOOL) m_siCopyQueueTimestampQueriesSupported = FALSE;

				/// <summary>
				/// Indicates if type casting is supported
				/// </summary>
				TSysInfo(BOOL) m_siCastingFullyTypedFormatSupported = FALSE;

				/// <summary>
				/// View instancing tier
				/// </summary>
				TSysInfo(D3D12_VIEW_INSTANCING_TIER) m_siViewInstancingTier = D3D12_VIEW_INSTANCING_TIER_NOT_SUPPORTED;

				/// <summary>
				/// Indicates if barycentrics are supported
				/// </summary>
				TSysInfo(BOOL) m_siBarycentricsSupported = FALSE;
		};
	}
}