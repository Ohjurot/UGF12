#include "pch.h"
#include "FS_Options6.h"

GxDirect::FeaturSupport::FS_Options6::FS_Options6(GxDirect::XContext* ptrContext) {
    // Get device
    ID3D12Device* ptrDevice;
    ptrContext->getDevice(&ptrDevice);

    // Create feature query
    D3D12_FEATURE_DATA_D3D12_OPTIONS6 featureData;
    ZeroMemory(&featureData, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS6));

    // Query for featue support
    if (SUCCEEDED(m_hrQueryResult = ptrDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS6, &featureData, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS6)))) {
        // Set values
        m_siAdditionalShadingRatesSupported = featureData.AdditionalShadingRatesSupported;
        m_siPerPrimitiveShadingRateSupportedWithViewportIndexing = featureData.PerPrimitiveShadingRateSupportedWithViewportIndexing;
        m_siVariableShadingRateTier = featureData.VariableShadingRateTier;
        m_siShadingRateImageTileSize = featureData.ShadingRateImageTileSize;
        m_siBackgroundProcessingSupported = featureData.BackgroundProcessingSupported;
    }

    // Release device
    COM_RELEASE(ptrDevice);
}

HRESULT GxDirect::FeaturSupport::FS_Options6::getQueryResult() {
    return m_hrQueryResult;
}

void GxDirect::FeaturSupport::FS_Options6::gennerateTextRepresentation() {
    #if defined(SHIPPING)
    throw EXEPTION(L"gennerateTextRepresentation(...) is not supported in shipping configuration");
    return;
    #else

    // String builder
    std::stringstream ss;

    // AdditionalShadingRatesSupported
    m_siAdditionalShadingRatesSupported.strName = "AdditionalShadingRatesSupported";
    if (m_siAdditionalShadingRatesSupported == TRUE) {
        m_siAdditionalShadingRatesSupported.strValue = "Yes";
    }
    else {
        m_siAdditionalShadingRatesSupported.strValue = "No";
    }
    
    // PerPrimitiveShadingRateSupportedWithViewportIndexing
    m_siPerPrimitiveShadingRateSupportedWithViewportIndexing.strName = "PerPrimitiveShadingRateSupportedWithViewportIndexing";
    if (m_siPerPrimitiveShadingRateSupportedWithViewportIndexing == TRUE) {
        m_siPerPrimitiveShadingRateSupportedWithViewportIndexing.strValue = "Yes";
    }
    else {
        m_siPerPrimitiveShadingRateSupportedWithViewportIndexing.strValue = "No";
    }

    // VariableShadingRateTier
    m_siVariableShadingRateTier.strName = "VSR Tier";
    switch (m_siVariableShadingRateTier.data) {
        case D3D12_VARIABLE_SHADING_RATE_TIER_1:
            m_siVariableShadingRateTier.strValue = "1";
            break;
        case D3D12_VARIABLE_SHADING_RATE_TIER_2:
            m_siVariableShadingRateTier.strValue = "2";
            break;

        case D3D12_VARIABLE_SHADING_RATE_TIER_NOT_SUPPORTED:
        default:
            m_siVariableShadingRateTier.strValue = "Not supported";
            break;
    }

    // ShadingRateImageTileSize
    m_siShadingRateImageTileSize.strName = "VSR Tile size";
    ss << m_siShadingRateImageTileSize.data << "px x " << m_siShadingRateImageTileSize.data << "px";
    m_siShadingRateImageTileSize.strValue = ss.str();

    // BackgroundProcessingSupported
    m_siBackgroundProcessingSupported.strName = "Background processing supported";
    if (m_siBackgroundProcessingSupported == TRUE) {
        m_siBackgroundProcessingSupported.strValue = "Yes";
    }
    else {
        m_siBackgroundProcessingSupported.strValue = "No";
    }

    #endif
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options6::getAdditionalShadingRatesSupported() {
    return m_siAdditionalShadingRatesSupported;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options6::getPerPrimitiveShadingRateSupportedWithViewportIndexing() {
    return m_siPerPrimitiveShadingRateSupportedWithViewportIndexing;
}

const TSysInfo(D3D12_VARIABLE_SHADING_RATE_TIER)& GxDirect::FeaturSupport::FS_Options6::getVariableShadingRateTier() {
    return m_siVariableShadingRateTier;
}

const TSysInfo(UINT)& GxDirect::FeaturSupport::FS_Options6::getShadingRateImageTileSize() {
    return m_siShadingRateImageTileSize;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options6::getBackgroundProcessingSupported() {
    return m_siBackgroundProcessingSupported;
}
