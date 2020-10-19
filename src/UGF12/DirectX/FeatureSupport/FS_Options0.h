#pragma once
#include "pch.h"

#include <UGF12/Util/TSysInfo.h>
#include <UGF12/Util/Exeption.h>

#include <UGF12/DirectX/XContext.h>

namespace GxDirect {
	namespace FeaturSupport {
		/// <summary>
		/// Container for D3D12_FEATURE_D3D12_OPTIONS
		/// </summary>
		class FS_Options0 {
			public:
				/// <summary>
				/// Create options based on a context
				/// </summary>
				/// <param name="ptrContext">context to query</param>
				FS_Options0(GxDirect::XContext* ptrContext);

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
				/// Indicates weather double percision floating point operations are supported in shaders
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getDoublePrecisionFloatShaderOps();

				/// <summary>
				/// Indicates that logical blending operations are supported by the output merger
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getOutputMergerLogicOp();

				/// <summary>
				/// TSysInfo(D3D12_SHADER_MIN_PRECISION_SUPPORT)&
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(D3D12_SHADER_MIN_PRECISION_SUPPORT)& getMinPrecisionSupport();

				/// <summary>
				/// Tiled resource tier
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(D3D12_TILED_RESOURCES_TIER)& getTiledResourcesTier();

				/// <summary>
				/// Resource binding tier
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(D3D12_RESOURCE_BINDING_TIER)& getResourceBindingTier();

				/// <summary>
				/// Indicates if pixel shader stencil ref is supported
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getPSSpecifiedStencilRefSupported();

				/// <summary>
				/// Indicates if UAVs support additional formats
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getTypedUAVLoadAdditionalFormats();

				/// <summary>
				/// Indicates ROV support
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getROVsSupported();

				/// <summary>
				/// Conservative rasterizer tier
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(D3D12_CONSERVATIVE_RASTERIZATION_TIER)& getConservativeRasterizationTier();

				/// <summary>
				/// Indicates support for 64KB texture swizzle pattern
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getStandardSwizzle64KBSupported();

				/// <summary>
				/// Specifies the tier of sharing within an multiple node adapter
				/// </summary>
				/// <returns>ref Info object</returns>
				const TSysInfo(D3D12_CROSS_NODE_SHARING_TIER)& getCrossNodeSharingTier();

				/// <summary>
				/// Indicates support for SRV, UAV and RTV acreoss adapters for row major textures
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getCrossAdapterRowMajorTextureSupported();

				/// <summary>
				/// https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_feature_data_d3d12_options
				/// </summary>
				/// <returns>const ref Info object</returns>
				const TSysInfo(BOOL)& getVPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation();

				/// <summary>
				/// Resource heap tier
				/// </summary>
				/// <returns>const ref Info object<</returns>
				const TSysInfo(D3D12_RESOURCE_HEAP_TIER)& getResourceHeapTier();

			private:
				/// <summary>
				/// Query result
				/// </summary>
				HRESULT m_hrQueryResult;

				/// <summary>
				/// Indicates weather double percision floating point operations are supported in shaders
				/// </summary>
				TSysInfo(BOOL) m_siDoublePrecisionFloatShaderOps = FALSE;

				/// <summary>
				/// Indicates that logical blending operations are supported by the output merger
				/// </summary>
				TSysInfo(BOOL) m_siOutputMergerLogicOp = FALSE;

				/// <summary>
				/// Combination of D3D12_SHADER_MIN_PRECISION_SUPPORT values that indicate if lower than 32 bit percision is supported
				/// </summary>
				TSysInfo(D3D12_SHADER_MIN_PRECISION_SUPPORT) m_siMinPrecisionSupport = D3D12_SHADER_MIN_PRECISION_SUPPORT_NONE;

				/// <summary>
				/// Tiled resource tier
				/// </summary>
				TSysInfo(D3D12_TILED_RESOURCES_TIER) m_siTiledResourcesTier = D3D12_TILED_RESOURCES_TIER_NOT_SUPPORTED;

				/// <summary>
				/// Rsource binding tier
				/// </summary>
				TSysInfo(D3D12_RESOURCE_BINDING_TIER) m_siResourceBindingTier = D3D12_RESOURCE_BINDING_TIER_1;

				/// <summary>
				/// Indicates if pixel shader stencil ref is supported
				/// </summary>
				TSysInfo(BOOL) m_siPSSpecifiedStencilRefSupported = FALSE;
				
				/// <summary>
				/// Indicates if UAVs support additional formats
				/// </summary>
				TSysInfo(BOOL) m_siTypedUAVLoadAdditionalFormats = FALSE;

				/// <summary>
				/// Indicates ROV support
				/// </summary>
				TSysInfo(BOOL) m_siROVsSupported = FALSE;

				/// <summary>
				/// Conservative rasterizer tier
				/// </summary>
				TSysInfo(D3D12_CONSERVATIVE_RASTERIZATION_TIER) m_siConservativeRasterizationTier = D3D12_CONSERVATIVE_RASTERIZATION_TIER_NOT_SUPPORTED;

				/// <summary>
				/// Indicates support for 64KB texture swizzle pattern
				/// </summary>
				TSysInfo(BOOL) m_siStandardSwizzle64KBSupported = FALSE;

				/// <summary>
				/// Specifies the tier of sharing within an multiple node adapter
				/// </summary>
				TSysInfo(D3D12_CROSS_NODE_SHARING_TIER) m_siCrossNodeSharingTier = D3D12_CROSS_NODE_SHARING_TIER_NOT_SUPPORTED;

				/// <summary>
				/// Indicates support for SRV, UAV and RTV acreoss adapters for row major textures
				/// </summary>
				TSysInfo(BOOL) m_siCrossAdapterRowMajorTextureSupported = FALSE;

				/// <summary>
				/// https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_feature_data_d3d12_options
				/// </summary>
				TSysInfo(BOOL) m_siVPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation = FALSE;

				/// <summary>
				/// Resource heap tier
				/// </summary>
				TSysInfo(D3D12_RESOURCE_HEAP_TIER) m_siResourceHeapTier = D3D12_RESOURCE_HEAP_TIER_1;
		};
	}
}