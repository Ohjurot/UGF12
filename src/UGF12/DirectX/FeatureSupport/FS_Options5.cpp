#include "pch.h"
#include "FS_Options5.h"

GxDirect::FeaturSupport::FS_Options5::FS_Options5(GxDirect::XContext* ptrContext) {
    // Get device
    ID3D12Device* ptrDevice;
    ptrContext->getDevice(&ptrDevice);

    // Create feature query
    D3D12_FEATURE_DATA_D3D12_OPTIONS5 featureData;
    ZeroMemory(&featureData, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS5));

    // Query for featue support
    if (SUCCEEDED(m_hrQueryResult = ptrDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &featureData, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS5)))) {
        // Set values
        m_siSRVOnlyTiledResourceTier3 = featureData.SRVOnlyTiledResourceTier3;
        m_siRenderPassesTier = featureData.RenderPassesTier;
        m_siRaytracingTier = featureData.RaytracingTier;
    }

    // Release device
    COM_RELEASE(ptrDevice);
}

HRESULT GxDirect::FeaturSupport::FS_Options5::getQueryResult() {
    return m_hrQueryResult;
}

void GxDirect::FeaturSupport::FS_Options5::gennerateTextRepresentation() {
    #if defined(SHIPPING)
    throw EXEPTION(L"gennerateTextRepresentation(...) is not supported in shipping configuration");
    return;
    #else

    // String builder
    std::stringstream ss;

    // SRVOnlyTiledResourceTier3
    m_siSRVOnlyTiledResourceTier3.strName = "Options5 require tiled resource tier >= 3";
    if (m_siSRVOnlyTiledResourceTier3 == TRUE) {
        m_siSRVOnlyTiledResourceTier3.strValue = "Yes";
    }
    else {
        m_siSRVOnlyTiledResourceTier3.strValue = "No";
    }

    // RenderPassesTier
    m_siRenderPassesTier.strName = "Render passes tier";
    switch (m_siRenderPassesTier.data) {
        case D3D12_RENDER_PASS_TIER_0:
            m_siRenderPassesTier.strValue = "0";
            break;
        case D3D12_RENDER_PASS_TIER_1:
            m_siRenderPassesTier.strValue = "1";
            break;
        case D3D12_RENDER_PASS_TIER_2:
            m_siRenderPassesTier.strValue = "2";
            break;
    }

    // RaytracingTier
    m_siRaytracingTier.strName = "Raytracing tier";
    switch (m_siRaytracingTier.data) {
        case D3D12_RAYTRACING_TIER_1_0:
            m_siRaytracingTier.strValue = "1.0";
            break;
        case D3D12_RAYTRACING_TIER_1_1:
            m_siRaytracingTier.strValue = "1.1";
            break;

        case D3D12_RAYTRACING_TIER_NOT_SUPPORTED:
        default:
            m_siRaytracingTier.strValue = "Not supported";
            break;
    }

    #endif
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options5::getSRVOnlyTiledResourceTier3() {
    return m_siSRVOnlyTiledResourceTier3;
}

const TSysInfo(D3D12_RENDER_PASS_TIER)& GxDirect::FeaturSupport::FS_Options5::getRenderPassesTier() {
    return m_siRenderPassesTier;
}

const TSysInfo(D3D12_RAYTRACING_TIER)& GxDirect::FeaturSupport::FS_Options5::getRaytracingTier() {
    return m_siRaytracingTier;
}
