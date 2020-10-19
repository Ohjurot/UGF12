#include "pch.h"
#include "FS_Architecture1.h"

GxDirect::FeaturSupport::FS_Architecture1::FS_Architecture1(GxDirect::XContext* ptrContext) {
    // Get device
    ID3D12Device* ptrDevice;
    ptrContext->getDevice(&ptrDevice);

    // Create feature query
    D3D12_FEATURE_DATA_ARCHITECTURE1 featureData;
    ZeroMemory(&featureData, sizeof(D3D12_FEATURE_DATA_ARCHITECTURE1));

    // Query for featue support
    if (SUCCEEDED(m_hrQueryResult = ptrDevice->CheckFeatureSupport(D3D12_FEATURE_ARCHITECTURE1, &featureData, sizeof(D3D12_FEATURE_DATA_ARCHITECTURE1)))) {
        // Set values
        m_siNodeIndex = featureData.NodeIndex;
        m_siTileBasedRenderer = featureData.TileBasedRenderer;
        m_siUMA = featureData.UMA;
        m_siCacheCoherentUMA = featureData.CacheCoherentUMA;
        m_siIsolatedMMU = featureData.IsolatedMMU;
    }

    // Release device
    COM_RELEASE(ptrDevice);
}

HRESULT GxDirect::FeaturSupport::FS_Architecture1::getQueryResult() {
    return m_hrQueryResult;
}

void GxDirect::FeaturSupport::FS_Architecture1::gennerateTextRepresentation() {
    #if defined(SHIPPING)
    throw EXEPTION(L"gennerateTextRepresentation(...) is not supported in shipping configuration");
    return;
    #else

    // String builder
    std::stringstream ss;

    // NodeIndex
    m_siNodeIndex.strName = "Relevant nodes (Multi node system)";
    ss << std::setbase(2) << m_siNodeIndex.data << std::dec << " (" << __popcnt(m_siNodeIndex.data) << ")";
    m_siNodeIndex.strValue = ss.str();

    // TileBasedRenderer
    m_siTileBasedRenderer.strName = "Tile base renderer";
    if (m_siTileBasedRenderer == TRUE) {
        m_siTileBasedRenderer.strValue = "Yes";
    }
    else {
        m_siTileBasedRenderer.strValue = "No";
    }

    // UMA
    m_siUMA.strName = "UMA (Unified Memory Architecture)";
    if (m_siUMA == TRUE) {
        m_siUMA.strValue = "Yes";
    }
    else {
        m_siUMA.strValue = "No";
    }

    // CacheCoherentUMA
    m_siCacheCoherentUMA.strName = "Cache coherent UMA";
    if (m_siCacheCoherentUMA == TRUE) {
        m_siCacheCoherentUMA.strValue = "Yes";
    }
    else {
        m_siCacheCoherentUMA.strValue = "No";
    }

    // IsolatedMMU
    m_siIsolatedMMU.strName = "Memory Management Unit";
    if (m_siIsolatedMMU == TRUE) {
        m_siIsolatedMMU.strValue = "Yes";
    }
    else {
        m_siIsolatedMMU.strValue = "No";
    }

    #endif
}

const TSysInfo(UINT)& GxDirect::FeaturSupport::FS_Architecture1::getNodeIndex() {
    return m_siNodeIndex;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Architecture1::getTileBasedRenderer() {
    return m_siTileBasedRenderer;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Architecture1::getUMA() {
    return m_siUMA;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Architecture1::getCacheCoherentUMA() {
    return m_siCacheCoherentUMA;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_Architecture1::getIsolatedMMU() {
    return m_siIsolatedMMU;
}
