#include "pch.h"
#include "FS_Options1.h"

GxDirect::FeaturSupport::FS_Options1::FS_Options1(GxDirect::XContext* ptrContext) {
    // Get device
    ID3D12Device* ptrDevice;
    ptrContext->getDevice(&ptrDevice);

    // Create feature query
    D3D12_FEATURE_DATA_D3D12_OPTIONS1 featureData;
    ZeroMemory(&featureData, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS1));

    // Query for featue support
    if (SUCCEEDED(m_hrQueryResult = ptrDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS1, &featureData, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS1)))) {
        // Set values
        m_siWaveOps = featureData.WaveOps;
        m_siWaveLaneCountMin = featureData.WaveLaneCountMin;
        m_siTotalLaneCount = featureData.TotalLaneCount;
        m_siInt64ShaderOps = featureData.Int64ShaderOps;
    }

    // Release device
    COM_RELEASE(ptrDevice);
}

HRESULT GxDirect::FeaturSupport::FS_Options1::getQueryResult() {
    return m_hrQueryResult;
}

void GxDirect::FeaturSupport::FS_Options1::gennerateTextRepresentation() {
    #if defined(SHIPPING)
    throw EXEPTION(L"gennerateTextRepresentation(...) is not supported in shipping configuration");
    return;
    #else

    // String builder
    std::stringstream ss;

    // WaveOps
    m_siWaveOps.strName = "HLSL 6.0 wave operation support";
    if (m_siWaveOps == TRUE) {
        m_siWaveOps.strValue = "Yes";
    }
    else {
        m_siWaveOps.strValue = "No";
    }

    // WaveLaneCountMin
    m_siWaveLaneCountMin.strName = "Wavefront / Warp width";
    ss.str(""); ss << m_siWaveLaneCountMin.data;
    m_siWaveLaneCountMin.strValue = ss.str();

    // TotalLaneCount
    m_siTotalLaneCount.strName = "Shader core count";
    ss.str(""); ss << m_siTotalLaneCount.data;
    m_siTotalLaneCount.strValue = ss.str();

    // Int64ShaderOps
    m_siInt64ShaderOps.strName = "Shader INT64 operation support";
    if (m_siInt64ShaderOps == TRUE) {
        m_siInt64ShaderOps.strValue = "Yes";
    }
    else {
        m_siInt64ShaderOps.strValue = "No";
    }

    #endif
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options1::getWaveOps() {
    return m_siWaveOps;
}

const TSysInfo(UINT)& GxDirect::FeaturSupport::FS_Options1::getWaveLaneCount() {
    return m_siWaveLaneCountMin;
}

const TSysInfo(UINT)& GxDirect::FeaturSupport::FS_Options1::getTotalLaneCount() {
    return m_siTotalLaneCount;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Options1::getInt64ShaderOps() {
    return m_siInt64ShaderOps;
}
