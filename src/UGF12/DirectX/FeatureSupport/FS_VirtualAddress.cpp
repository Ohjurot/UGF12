#include "pch.h"
#include "FS_VirtualAddress.h"

GxDirect::FeaturSupport::FS_VirtualAddress::FS_VirtualAddress(GxDirect::XContext* ptrContext) {
    // Get device
    ID3D12Device* ptrDevice;
    ptrContext->getDevice(&ptrDevice);

    // Create feature query
    D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT featureData;
    ZeroMemory(&featureData, sizeof(D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT));

    // Query for featue support
    if (SUCCEEDED(m_hrQueryResult = ptrDevice->CheckFeatureSupport(D3D12_FEATURE_GPU_VIRTUAL_ADDRESS_SUPPORT, &featureData, sizeof(D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT)))) {
        // Set values
        m_siMaxGPUVirtualAddressBitsPerResource = featureData.MaxGPUVirtualAddressBitsPerResource;
        m_siMaxGPUVirtualAddressBitsPerProcess = featureData.MaxGPUVirtualAddressBitsPerProcess;
    }

    // Release device
    COM_RELEASE(ptrDevice);
}

HRESULT GxDirect::FeaturSupport::FS_VirtualAddress::getQueryResult() {
    return m_hrQueryResult;
}

void GxDirect::FeaturSupport::FS_VirtualAddress::gennerateTextRepresentation() {
    #if defined(SHIPPING)
    throw EXEPTION(L"gennerateTextRepresentation(...) is not supported in shipping configuration");
    return;
    #else

    // String builder
    std::stringstream ss;

    // MaxGPUVirtualAddressBitsPerResource
    m_siMaxGPUVirtualAddressBitsPerResource.strName = "MaxGPUVirtualAddressBitsPerResource";
    ss << m_siMaxGPUVirtualAddressBitsPerResource.data;
    m_siMaxGPUVirtualAddressBitsPerResource.strValue = ss.str();

    // MaxGPUVirtualAddressBitsPerProcess
    m_siMaxGPUVirtualAddressBitsPerProcess.strName = "MaxGPUVirtualAddressBitsPerProcess";
    ss.str(""); ss << m_siMaxGPUVirtualAddressBitsPerProcess.data;
    m_siMaxGPUVirtualAddressBitsPerProcess.strValue = ss.str();

    #endif
}

const TSysInfo(UINT)& GxDirect::FeaturSupport::FS_VirtualAddress::getMaxGPUVirtualAddressBitsPerResource() {
    return m_siMaxGPUVirtualAddressBitsPerResource;
}

const TSysInfo(UINT)& GxDirect::FeaturSupport::FS_VirtualAddress::getMaxGPUVirtualAddressBitsPerProcess() {
    return m_siMaxGPUVirtualAddressBitsPerProcess;
}
