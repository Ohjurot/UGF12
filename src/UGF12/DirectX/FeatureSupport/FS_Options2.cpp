#include "pch.h"
#include "FS_Options2.h"

GxDirect::FeaturSupport::FS_Options2::FS_Options2(GxDirect::XContext* ptrContext) {
    // Get device
    ID3D12Device* ptrDevice;
    ptrContext->getDevice(&ptrDevice);

    // Create feature query
    D3D12_FEATURE_DATA_D3D12_OPTIONS2 featureData;
    ZeroMemory(&featureData, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS2));

    // Query for featue support
    if (SUCCEEDED(m_hrQueryResult = ptrDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS2, &featureData, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS2)))) {
        // Set values
        m_siDepthBoundsTestSupported = featureData.DepthBoundsTestSupported;
        m_siProgrammableSamplePositionsTier = featureData.ProgrammableSamplePositionsTier;
    }

    // Release device
    COM_RELEASE(ptrDevice);
}

HRESULT GxDirect::FeaturSupport::FS_Options2::getQueryResult() {
    return m_hrQueryResult;
}

void GxDirect::FeaturSupport::FS_Options2::gennerateTextRepresentation(){   
    #if defined(SHIPPING)
    throw EXEPTION(L"gennerateTextRepresentation(...) is not supported in shipping configuration");
    return;
    #else

    // String builder
    std::stringstream ss;

    // DepthBoundsTestSupported
    m_siDepthBoundsTestSupported.strName = "Depth bound test support";
    if (m_siDepthBoundsTestSupported == TRUE) {
        m_siDepthBoundsTestSupported.strValue = "Yes";
    }
    else {
        m_siDepthBoundsTestSupported.strName = "No";
    }

    // ProgrammableSamplePositionsTier
    m_siProgrammableSamplePositionsTier.strName = "Sample position tier";
    switch (m_siProgrammableSamplePositionsTier.data) {
        case D3D12_PROGRAMMABLE_SAMPLE_POSITIONS_TIER_1:
            m_siProgrammableSamplePositionsTier.strValue = "1";
            break;
        case D3D12_PROGRAMMABLE_SAMPLE_POSITIONS_TIER_2:
            m_siProgrammableSamplePositionsTier.strValue = "2";
            break;

        case D3D12_PROGRAMMABLE_SAMPLE_POSITIONS_TIER_NOT_SUPPORTED:
        default:
            m_siProgrammableSamplePositionsTier.strValue = "Not supported";
            break;
    }

    #endif
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options2::getDepthBoundsTestSupported() {
    return m_siDepthBoundsTestSupported;
}

const TSysInfo(D3D12_PROGRAMMABLE_SAMPLE_POSITIONS_TIER)& GxDirect::FeaturSupport::FS_Options2::getProgrammableSamplePositionsTier() {
    return m_siProgrammableSamplePositionsTier;
}
