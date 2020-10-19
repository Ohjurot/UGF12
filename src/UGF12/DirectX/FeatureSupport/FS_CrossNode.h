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
		class FS_CrossNode {
			public:
				/// <summary>
				/// Create options based on a context
				/// </summary>
				/// <param name="ptrContext">context to query</param>
				FS_CrossNode(GxDirect::XContext* ptrContext);

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
				/// Indicates cross adapter sharing tier
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(D3D12_CROSS_NODE_SHARING_TIER)& getSharingTier();

				/// <summary>
				/// Indicates if adpater support atomic instructions (cross device instructions)
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getAtomicShaderInstructions();

			private:
				/// <summary>
				/// Query result
				/// </summary>
				HRESULT m_hrQueryResult;

				/// <summary>
				/// Indicates cross adapter sharing tier
				/// </summary>
				TSysInfo(D3D12_CROSS_NODE_SHARING_TIER) m_siSharingTier = D3D12_CROSS_NODE_SHARING_TIER_NOT_SUPPORTED;
				
				/// <summary>
				/// Indicates if adpater support atomic instructions (cross device instructions)
				/// </summary>
				TSysInfo(BOOL) m_siAtomicShaderInstructions = FALSE;
		};
	}
}