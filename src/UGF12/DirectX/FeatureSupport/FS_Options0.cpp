#include "pch.h"
#include "FS_Options0.h"

GxDirect::FeaturSupport::FS_Options0::FS_Options0(GxDirect::XContext* ptrContext) {
    // Get device
    ID3D12Device* ptrDevice;
    ptrContext->getDevice(&ptrDevice);

    // Create feature query
    D3D12_FEATURE_DATA_D3D12_OPTIONS featureData;
    ZeroMemory(&featureData, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS));
    
    // Query for featue support
    if (SUCCEEDED(m_hrQueryResult = ptrDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &featureData, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS)))) {
        // Set values
        m_siDoublePrecisionFloatShaderOps = featureData.DoublePrecisionFloatShaderOps;
        m_siOutputMergerLogicOp = featureData.OutputMergerLogicOp;
        m_siMinPrecisionSupport = featureData.MinPrecisionSupport;
        m_siTiledResourcesTier = featureData.TiledResourcesTier;
        m_siResourceBindingTier = featureData.ResourceBindingTier;
        m_siPSSpecifiedStencilRefSupported = featureData.PSSpecifiedStencilRefSupported;
        m_siTypedUAVLoadAdditionalFormats = featureData.TypedUAVLoadAdditionalFormats;
        m_siROVsSupported = featureData.ROVsSupported;
        m_siConservativeRasterizationTier = featureData.ConservativeRasterizationTier;
        m_siStandardSwizzle64KBSupported = featureData.StandardSwizzle64KBSupported;
        m_siCrossNodeSharingTier = featureData.CrossNodeSharingTier;
        m_siCrossAdapterRowMajorTextureSupported = featureData.CrossAdapterRowMajorTextureSupported;
        m_siVPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation =
            featureData.VPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation;
        m_siResourceHeapTier = featureData.ResourceHeapTier;
    }

    // Release device
    COM_RELEASE(ptrDevice);
}

HRESULT GxDirect::FeaturSupport::FS_Options0::getQueryResult() {
    return m_hrQueryResult;
}

void GxDirect::FeaturSupport::FS_Options0::gennerateTextRepresentation() {
    #if defined(SHIPPING)
    throw EXEPTION(L"gennerateTextRepresentation(...) is not supported in shipping configuration");
    return;
    #else

    // String builder
    std::stringstream ss;

    // Double shader OP
    m_siDoublePrecisionFloatShaderOps.strName = "Double percision float operation support";
    if (m_siDoublePrecisionFloatShaderOps == TRUE) {
        m_siDoublePrecisionFloatShaderOps.strValue = "Yes";
    } else {
        m_siDoublePrecisionFloatShaderOps.strValue = "No";
    }

    // Output merger logic op
    m_siOutputMergerLogicOp.strName = "Logical blending support";
    if (m_siOutputMergerLogicOp == TRUE) {
        m_siOutputMergerLogicOp.strValue = "Yes";
    }
    else {
        m_siOutputMergerLogicOp.strValue = "No";
    }

    // Shader min percision
    m_siMinPrecisionSupport.strName = "Shader minimal percision";
    if (m_siMinPrecisionSupport == D3D12_SHADER_MIN_PRECISION_SUPPORT_NONE) {
        m_siMinPrecisionSupport.strValue = "32-Bit";
    } else {
        // Build text
        ss.str("");
        ss << "32-Bit and ";

        if (m_siMinPrecisionSupport.data & D3D12_SHADER_MIN_PRECISION_SUPPORT_16_BIT) {
            ss << "16-Bit";
        }
        if (m_siMinPrecisionSupport.data & D3D12_SHADER_MIN_PRECISION_SUPPORT_10_BIT) {
            ss << " or 10-Bit";
        }

        // Set
        m_siMinPrecisionSupport.strValue = ss.str();
    }

    // TiledResourcesTier
    m_siTiledResourcesTier.strName = "Tiled resource tier";
    switch (m_siTiledResourcesTier.data) {
        case D3D12_TILED_RESOURCES_TIER_1:
            m_siTiledResourcesTier.strValue = "1";
            break;
        case D3D12_TILED_RESOURCES_TIER_2:
            m_siTiledResourcesTier.strValue = "2";
            break;
        case D3D12_TILED_RESOURCES_TIER_3:
            m_siTiledResourcesTier.strValue = "3";
            break;
        case D3D12_TILED_RESOURCES_TIER_4:
            m_siTiledResourcesTier.strValue = "4";
            break;

        case D3D12_TILED_RESOURCES_TIER_NOT_SUPPORTED:
        default:
            m_siTiledResourcesTier.strValue = "Not supported";
            break;
    }

    // ResourceBindingTier
    m_siResourceBindingTier.strName = "Resource binding tier";
    switch (m_siResourceBindingTier.data) {
        case D3D12_RESOURCE_BINDING_TIER_1:
            m_siResourceBindingTier.strValue = "1";
            break;
        case D3D12_RESOURCE_BINDING_TIER_2:
            m_siResourceBindingTier.strValue = "2";
            break;
        case D3D12_RESOURCE_BINDING_TIER_3:
            m_siResourceBindingTier.strValue = "3";
            break;
    }

    // PSSpecifiedStencilRefSupported
    m_siPSSpecifiedStencilRefSupported.strName = "Pixel shader stencil ref support";
    if (m_siPSSpecifiedStencilRefSupported == TRUE) {
        m_siPSSpecifiedStencilRefSupported.strValue = "Yes";
    }
    else {
        m_siPSSpecifiedStencilRefSupported.strValue = "No";
    }

    // TypedUAVLoadAdditionalFormats
    m_siTypedUAVLoadAdditionalFormats.strName = "UAV additional format support";
    if (m_siTypedUAVLoadAdditionalFormats == TRUE) {
        m_siTypedUAVLoadAdditionalFormats.strValue = "Yes";
    }
    else {
        m_siTypedUAVLoadAdditionalFormats.strValue = "No";
    }

    // ROVsSupported
    m_siROVsSupported.strName = "ROV Support";
    if (m_siROVsSupported == TRUE) {
        m_siROVsSupported.strValue = "Yes";
    }
    else {
        m_siROVsSupported.strValue = "No";
    }

    // ConservativeRasterizationTier
    m_siConservativeRasterizationTier.strName = "Conservative rasterization tier";
    switch (m_siConservativeRasterizationTier.data) {
        case D3D12_CONSERVATIVE_RASTERIZATION_TIER_1:
            m_siConservativeRasterizationTier.strValue = "1";
            break;
        case D3D12_CONSERVATIVE_RASTERIZATION_TIER_2:
            m_siConservativeRasterizationTier.strValue = "2";
            break;
        case D3D12_CONSERVATIVE_RASTERIZATION_TIER_3:
            m_siConservativeRasterizationTier.strValue = "3";
            break;

        case D3D12_CONSERVATIVE_RASTERIZATION_TIER_NOT_SUPPORTED:
        default:
            m_siConservativeRasterizationTier.strValue = "Not supported";
            break;
    }

    // StandardSwizzle64KBSupported
    m_siStandardSwizzle64KBSupported.strName = "64KB swizzle pattern texture support";
    if (m_siStandardSwizzle64KBSupported == TRUE) {
        m_siStandardSwizzle64KBSupported.strValue = "Yes";
    }
    else {
        m_siStandardSwizzle64KBSupported.strValue = "No";
    }

    // CrossNodeSharingTier
    m_siCrossNodeSharingTier.strName = "Cross node sharing tier";
    switch(m_siCrossNodeSharingTier.data){
        case D3D12_CROSS_NODE_SHARING_TIER_1_EMULATED:
            m_siCrossNodeSharingTier.strValue = "1 (Emulated)";
            break;
        case D3D12_CROSS_NODE_SHARING_TIER_1:
            m_siCrossNodeSharingTier.strValue = "1";
            break;
        case D3D12_CROSS_NODE_SHARING_TIER_2:
            m_siCrossNodeSharingTier.strValue = "2";
            break;
        case D3D12_CROSS_NODE_SHARING_TIER_3:
            m_siCrossNodeSharingTier.strValue = "3";
            break;

        case D3D12_CROSS_NODE_SHARING_TIER_NOT_SUPPORTED:
        default:
            m_siCrossNodeSharingTier.strValue = "Not supported";
            break;
    }

    // CrossAdapterRowMajorTextureSupported
    m_siCrossAdapterRowMajorTextureSupported.strName = "Cross adapter row major texture support";
    if (m_siCrossAdapterRowMajorTextureSupported == TRUE) {
        m_siCrossAdapterRowMajorTextureSupported.strValue = "Yes";
    }
    else {
        m_siCrossAdapterRowMajorTextureSupported.strValue = "No";
    }

    // VPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation
    m_siVPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation.strName = "VPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation";
    if (m_siVPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation == TRUE) {
        m_siVPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation.strValue = "Yes";
    } else {
        m_siVPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation.strValue = "No";
    }

    // ResourceHeapTier
    m_siResourceHeapTier.strName = "Resource heap tier";
    switch (m_siResourceHeapTier.data) {
        case D3D12_RESOURCE_HEAP_TIER_1:
            m_siResourceHeapTier.strValue = "1";
            break;
        case D3D12_RESOURCE_HEAP_TIER_2:
            m_siResourceHeapTier.strValue = "2";
            break;
    }

    #endif
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options0::getDoublePrecisionFloatShaderOps() {
    return m_siDoublePrecisionFloatShaderOps;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options0::getOutputMergerLogicOp() {
    return m_siOutputMergerLogicOp;
}

const TSysInfo(D3D12_SHADER_MIN_PRECISION_SUPPORT)& GxDirect::FeaturSupport::FS_Options0::getMinPrecisionSupport() {
    return m_siMinPrecisionSupport;
}

const TSysInfo(D3D12_TILED_RESOURCES_TIER)& GxDirect::FeaturSupport::FS_Options0::getTiledResourcesTier() {
    return m_siTiledResourcesTier;
}

const TSysInfo(D3D12_RESOURCE_BINDING_TIER)& GxDirect::FeaturSupport::FS_Options0::getResourceBindingTier() {
    return m_siResourceBindingTier;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options0::getPSSpecifiedStencilRefSupported() {
    return m_siPSSpecifiedStencilRefSupported;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options0::getTypedUAVLoadAdditionalFormats() {
    return m_siTypedUAVLoadAdditionalFormats;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options0::getROVsSupported() {
    return m_siROVsSupported;
}

const TSysInfo(D3D12_CONSERVATIVE_RASTERIZATION_TIER)& GxDirect::FeaturSupport::FS_Options0::getConservativeRasterizationTier() {
    return m_siConservativeRasterizationTier;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options0::getStandardSwizzle64KBSupported() {
    return m_siStandardSwizzle64KBSupported;
}

const TSysInfo(D3D12_CROSS_NODE_SHARING_TIER)& GxDirect::FeaturSupport::FS_Options0::getCrossNodeSharingTier() {
    return m_siCrossNodeSharingTier;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options0::getCrossAdapterRowMajorTextureSupported() {
    return m_siCrossAdapterRowMajorTextureSupported;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options0::getVPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation() {
    return m_siVPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation;
}

const TSysInfo(D3D12_RESOURCE_HEAP_TIER)& GxDirect::FeaturSupport::FS_Options0::getResourceHeapTier() {
    return m_siResourceHeapTier;
}
