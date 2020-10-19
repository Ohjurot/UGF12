#include "pch.h"
#include "FS_Options4.h"

GxDirect::FeaturSupport::FS_Options4::FS_Options4(GxDirect::XContext* ptrContext){
    // Get device
    ID3D12Device* ptrDevice;
    ptrContext->getDevice(&ptrDevice);

    // Create feature query
    D3D12_FEATURE_DATA_D3D12_OPTIONS4 featureData;
    ZeroMemory(&featureData, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS4));

    // Query for featue support
    if (SUCCEEDED(m_hrQueryResult = ptrDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS4, &featureData, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS4)))) {
        // Set values
        m_siMSAA64KBAlignedTextureSupported = featureData.MSAA64KBAlignedTextureSupported;
        m_siSharedResourceCompatibilityTier = featureData.SharedResourceCompatibilityTier;
        m_siNative16BitShaderOpsSupported = featureData.Native16BitShaderOpsSupported;
    }

    // Release device
    COM_RELEASE(ptrDevice);
}

HRESULT GxDirect::FeaturSupport::FS_Options4::getQueryResult() {
    return m_hrQueryResult;
}

void GxDirect::FeaturSupport::FS_Options4::gennerateTextRepresentation() {
    #if defined(SHIPPING)
    throw EXEPTION(L"gennerateTextRepresentation(...) is not supported in shipping configuration");
    return;
    #else

    // String builder
    std::stringstream ss;

    // MSAA64KBAlignedTextureSupported
    m_siMSAA64KBAlignedTextureSupported.strName = "64KB aligned MSAA texture support";
    if (m_siMSAA64KBAlignedTextureSupported == TRUE) {
        m_siMSAA64KBAlignedTextureSupported.strValue = "Yes";
    }
    else {
        m_siMSAA64KBAlignedTextureSupported.strValue = "No";
    }

    // SharedResourceCompatibilityTier
    m_siSharedResourceCompatibilityTier.strName = "Cross api share tier";
    switch (m_siSharedResourceCompatibilityTier.data) {
        case D3D12_SHARED_RESOURCE_COMPATIBILITY_TIER_0:
            m_siSharedResourceCompatibilityTier.strValue = "0";
            break;
        case D3D12_SHARED_RESOURCE_COMPATIBILITY_TIER_1:
            m_siSharedResourceCompatibilityTier.strValue = "1";
            break;
        case D3D12_SHARED_RESOURCE_COMPATIBILITY_TIER_2:
            m_siSharedResourceCompatibilityTier.strValue = "2";
            break;
    }

    // Native16BitShaderOpsSupported
    m_siNative16BitShaderOpsSupported.strName = "Shader 16-Bit native operations support";
    if (m_siNative16BitShaderOpsSupported == TRUE) {
        m_siNative16BitShaderOpsSupported.strValue = "Yes";
    }
    else {
        m_siNative16BitShaderOpsSupported.strValue = "No";
    }

    #endif
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options4::getMSAA64KBAlignedTextureSupported() {
    return m_siMSAA64KBAlignedTextureSupported;
}

const TSysInfo(D3D12_SHARED_RESOURCE_COMPATIBILITY_TIER)& GxDirect::FeaturSupport::FS_Options4::getSharedResourceCompatibilityTier() {
    return m_siSharedResourceCompatibilityTier;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options4::getNative16BitShaderOpsSupported() {
    return m_siNative16BitShaderOpsSupported;
}
