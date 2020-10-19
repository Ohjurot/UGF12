#pragma once
#include "pch.h"

#include <UGF12/Util/TSysInfo.h>
#include <UGF12/Util/Exeption.h>

#include <UGF12/DirectX/XContext.h>

namespace GxDirect {
	namespace FeaturSupport {
		/// <summary>
		/// Container for D3D12_FEATURE_D3D12_OPTIONS7
		/// </summary>
		class FS_Options7 {
			public:
				/// <summary>
				/// Create options based on a context
				/// </summary>
				/// <param name="ptrContext">context to query</param>
				FS_Options7(GxDirect::XContext* ptrContext);

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
				/// Mesh shader tier
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(D3D12_MESH_SHADER_TIER)& getMeshShaderTier();
				
				/// <summary>
				/// Sampler feedback tier
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(D3D12_SAMPLER_FEEDBACK_TIER)& getSamplerFeedbackTier();

			private:
				/// <summary>
				/// Query result
				/// </summary>
				HRESULT m_hrQueryResult;

				/// <summary>
				/// Mesh shader tier
				/// </summary>
				TSysInfo(D3D12_MESH_SHADER_TIER) m_siMeshShaderTier = D3D12_MESH_SHADER_TIER_NOT_SUPPORTED;

				/// <summary>
				/// Sampler feedback tier
				/// </summary>
				TSysInfo(D3D12_SAMPLER_FEEDBACK_TIER) m_siSamplerFeedbackTier = D3D12_SAMPLER_FEEDBACK_TIER_NOT_SUPPORTED;
		};
	}
}