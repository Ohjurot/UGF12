#include "pch.h"
#include "FS_Options3.h"

GxDirect::FeaturSupport::FS_Options3::FS_Options3(GxDirect::XContext* ptrContext) {
    // Get device
    ID3D12Device* ptrDevice;
    ptrContext->getDevice(&ptrDevice);

    // Create feature query
    D3D12_FEATURE_DATA_D3D12_OPTIONS3 featureData;
    ZeroMemory(&featureData, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS3));

    // Query for featue support
    if (SUCCEEDED(m_hrQueryResult = ptrDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS3, &featureData, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS3)))) {
        // Set values
        m_siCopyQueueTimestampQueriesSupported = featureData.CopyQueueTimestampQueriesSupported;
        m_siCastingFullyTypedFormatSupported = featureData.CastingFullyTypedFormatSupported;
        m_siViewInstancingTier = featureData.ViewInstancingTier;
        m_siBarycentricsSupported = featureData.BarycentricsSupported;
    }

    // Release device
    COM_RELEASE(ptrDevice);
}

HRESULT GxDirect::FeaturSupport::FS_Options3::getQueryResult() {
    return m_hrQueryResult;
}

void GxDirect::FeaturSupport::FS_Options3::gennerateTextRepresentation() {
    #if defined(SHIPPING)
    throw EXEPTION(L"gennerateTextRepresentation(...) is not supported in shipping configuration");
    return;
    #else

    // String builder
    std::stringstream ss;

    // CopyQueueTimestampQueriesSupported
    m_siCopyQueueTimestampQueriesSupported.strName = "Timestamp querie support";
    if (m_siCopyQueueTimestampQueriesSupported == TRUE) {
        m_siCopyQueueTimestampQueriesSupported.strValue = "Yes";
    }
    else {
        m_siCopyQueueTimestampQueriesSupported.strValue = "No";
    }

    // CastingFullyTypedFormatSupported
    m_siCastingFullyTypedFormatSupported.strName = "Type casting support";
    if (m_siCastingFullyTypedFormatSupported == TRUE) {
        m_siCastingFullyTypedFormatSupported.strValue = "Yes";
    }
    else {
        m_siCastingFullyTypedFormatSupported.strValue = "No";
    }

    // ViewInstancingTier
    m_siViewInstancingTier.strName = "View instancing tier";
    switch (m_siViewInstancingTier.data) {
        case D3D12_VIEW_INSTANCING_TIER_1:
            m_siViewInstancingTier.strValue = "1";
            break;
        case D3D12_VIEW_INSTANCING_TIER_2:
            m_siViewInstancingTier.strValue = "2";
            break;
        case D3D12_VIEW_INSTANCING_TIER_3:
            m_siViewInstancingTier.strValue = "3";
            break;

        case D3D12_VIEW_INSTANCING_TIER_NOT_SUPPORTED:
        default:
            m_siViewInstancingTier.strValue = "Not supported";
            break;
    }


    // BarycentricsSupported
    m_siBarycentricsSupported.strName = "Barycentrics support";
    if (m_siBarycentricsSupported == TRUE) {
        m_siBarycentricsSupported.strValue = "Yes";
    }
    else {
        m_siBarycentricsSupported.strValue = "No";
    }

    #endif
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options3::getCopyQueueTimestampQueriesSupported() {
    return m_siCopyQueueTimestampQueriesSupported;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options3::getCastingFullyTypedFormatSupported() {
    return m_siCastingFullyTypedFormatSupported;
}

const TSysInfo(D3D12_VIEW_INSTANCING_TIER)& GxDirect::FeaturSupport::FS_Options3::getViewInstancingTier() {
    return m_siViewInstancingTier;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options3::getBarycentricsSupported() {
    return m_siBarycentricsSupported;
}
