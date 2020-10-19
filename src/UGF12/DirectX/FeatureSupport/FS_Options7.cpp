#include "pch.h"
#include "FS_Options7.h"

GxDirect::FeaturSupport::FS_Options7::FS_Options7(GxDirect::XContext* ptrContext) {
    // Get device
    ID3D12Device* ptrDevice;
    ptrContext->getDevice(&ptrDevice);

    // Create feature query
    D3D12_FEATURE_DATA_D3D12_OPTIONS7 featureData;
    ZeroMemory(&featureData, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS7));

    // Query for featue support
    if (SUCCEEDED(m_hrQueryResult = ptrDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &featureData, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS7)))) {
        // Set values
        m_siMeshShaderTier = featureData.MeshShaderTier;
        m_siSamplerFeedbackTier = featureData.SamplerFeedbackTier;
    }

    // Release device
    COM_RELEASE(ptrDevice);
}

HRESULT GxDirect::FeaturSupport::FS_Options7::getQueryResult() {
    return m_hrQueryResult;
}

void GxDirect::FeaturSupport::FS_Options7::gennerateTextRepresentation() {
    #if defined(SHIPPING)
    throw EXEPTION(L"gennerateTextRepresentation(...) is not supported in shipping configuration");
    return;
    #else

    // String builder
    std::stringstream ss;

    // MeshShaderTier
    m_siMeshShaderTier.strName = "Mesh shader tier";
    switch (m_siMeshShaderTier.data) {
        case D3D12_MESH_SHADER_TIER_1:
            m_siMeshShaderTier.strValue = "1";
            break;

        case D3D12_MESH_SHADER_TIER_NOT_SUPPORTED:
        default:
            m_siMeshShaderTier.strValue = "Not supported";
            break;
    }

    // SamplerFeedbackTier
    m_siSamplerFeedbackTier.strName = "Sampler feedback tier";
    switch (m_siSamplerFeedbackTier.data) {
        case D3D12_SAMPLER_FEEDBACK_TIER_0_9:
            m_siSamplerFeedbackTier.strValue = "0.9";
            break;
        case D3D12_SAMPLER_FEEDBACK_TIER_1_0:
            m_siSamplerFeedbackTier.strValue = "1.0";
            break;

        case D3D12_SAMPLER_FEEDBACK_TIER_NOT_SUPPORTED:
        default:
            m_siSamplerFeedbackTier.strValue = "Not supported";
            break;
    }

    #endif
}

const TSysInfo(D3D12_MESH_SHADER_TIER)& GxDirect::FeaturSupport::FS_Options7::getMeshShaderTier() {
    return m_siMeshShaderTier;
}

const TSysInfo(D3D12_SAMPLER_FEEDBACK_TIER)& GxDirect::FeaturSupport::FS_Options7::getSamplerFeedbackTier() {
    return m_siSamplerFeedbackTier;
}
