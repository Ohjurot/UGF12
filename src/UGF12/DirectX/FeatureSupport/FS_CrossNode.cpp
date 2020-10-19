#include "pch.h"
#include "FS_CrossNode.h"

GxDirect::FeaturSupport::FS_CrossNode::FS_CrossNode(GxDirect::XContext* ptrContext) {
    // Get device
    ID3D12Device* ptrDevice;
    ptrContext->getDevice(&ptrDevice);

    // Create feature query
    D3D12_FEATURE_DATA_CROSS_NODE featureData;
    ZeroMemory(&featureData, sizeof(D3D12_FEATURE_DATA_CROSS_NODE));

    // Query for featue support
    if (SUCCEEDED(m_hrQueryResult = ptrDevice->CheckFeatureSupport(D3D12_FEATURE_CROSS_NODE, &featureData, sizeof(D3D12_FEATURE_DATA_CROSS_NODE)))) {
        // Set values
        m_siSharingTier = featureData.SharingTier;
        m_siAtomicShaderInstructions = featureData.AtomicShaderInstructions;
    }

    // Release device
    COM_RELEASE(ptrDevice);
}

HRESULT GxDirect::FeaturSupport::FS_CrossNode::getQueryResult() {
    return m_hrQueryResult;
}

void GxDirect::FeaturSupport::FS_CrossNode::gennerateTextRepresentation() {
    #if defined(SHIPPING)
    throw EXEPTION(L"gennerateTextRepresentation(...) is not supported in shipping configuration");
    return;
    #else

    // String builder
    std::stringstream ss;

    // SharingTier
    m_siSharingTier.strName = "Cross adapter sharing tier";
    switch (m_siSharingTier.data) {
        case D3D12_CROSS_NODE_SHARING_TIER_1_EMULATED:
            m_siSharingTier.strValue = "1 (Emulated)";
            break;
        case D3D12_CROSS_NODE_SHARING_TIER_1:
            m_siSharingTier.strValue = "1";
            break;
        case D3D12_CROSS_NODE_SHARING_TIER_2:
            m_siSharingTier.strValue = "2";
            break;
        case D3D12_CROSS_NODE_SHARING_TIER_3:
            m_siSharingTier.strValue = "3";
            break;

        case D3D12_CROSS_NODE_SHARING_TIER_NOT_SUPPORTED:
        default:
            m_siSharingTier.strValue = "Not supported";
            break;
    }

    // AtomicShaderInstructions
    m_siAtomicShaderInstructions.strName = "Atomic shader instructions";
    if (m_siAtomicShaderInstructions == TRUE) {
        m_siAtomicShaderInstructions.strValue = "Yes";
    }
    else {
        m_siAtomicShaderInstructions.strValue = "No";
    }

    #endif
}

const TSysInfo(D3D12_CROSS_NODE_SHARING_TIER)& GxDirect::FeaturSupport::FS_CrossNode::getSharingTier() {
    return m_siSharingTier;
}

const TSysInfo(BOOL)& GxDirect::FeaturSupport::FS_CrossNode::getAtomicShaderInstructions() {
    return m_siAtomicShaderInstructions;
}
