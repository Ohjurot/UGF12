#include "pch.h"
#include "XFeatureSupport.h"

GxDirect::XFeatureSupport::XFeatureSupport(GxDirect::XContext* ptrContext) :
    m_options0(ptrContext),
    m_options1(ptrContext),
    m_options2(ptrContext),
    m_options3(ptrContext),
    m_options4(ptrContext),
    m_options5(ptrContext),
    m_options6(ptrContext),
    m_options7(ptrContext),

    m_architecture1(ptrContext),
    m_virtualAddress(ptrContext),
    m_crossNode(ptrContext)
{
    // Get device
    ptrContext->getDevice(&m_ptrDevice);

    // Query shader model
    D3D12_FEATURE_DATA_SHADER_MODEL shaderModelQuery;
    ZeroMemory(&shaderModelQuery, sizeof(D3D12_FEATURE_DATA_SHADER_MODEL));
    shaderModelQuery.HighestShaderModel = D3D_SHADER_MODEL_6_6;
    if (SUCCEEDED(m_ptrDevice->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &shaderModelQuery, sizeof(D3D12_FEATURE_DATA_SHADER_MODEL)))) {
        m_siHighstShaderModell = shaderModelQuery.HighestShaderModel;
    }

    // Query root signature verison
    D3D12_FEATURE_DATA_ROOT_SIGNATURE rootsigVersionQuery;
    ZeroMemory(&rootsigVersionQuery, sizeof(D3D12_FEATURE_DATA_ROOT_SIGNATURE));
    rootsigVersionQuery.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
    if (SUCCEEDED(m_ptrDevice->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &rootsigVersionQuery, sizeof(D3D12_FEATURE_DATA_ROOT_SIGNATURE)))) {
        m_siRootSignatureVersion = rootsigVersionQuery.HighestVersion;
    }

    // Query shader cache flags support
    D3D12_FEATURE_DATA_SHADER_CACHE shaderCacheQuery;
    ZeroMemory(&shaderCacheQuery, sizeof(D3D12_FEATURE_DATA_SHADER_CACHE));
    if (SUCCEEDED(m_ptrDevice->CheckFeatureSupport(D3D12_FEATURE_SHADER_CACHE, &shaderCacheQuery, sizeof(D3D12_FEATURE_DATA_SHADER_CACHE)))) {
        m_siShaderCache = shaderCacheQuery.SupportFlags;
    }

    // Existing heap query
    D3D12_FEATURE_DATA_EXISTING_HEAPS existingHeapSupportQuery;
    ZeroMemory(&existingHeapSupportQuery, sizeof(D3D12_FEATURE_DATA_EXISTING_HEAPS));
    if (SUCCEEDED(m_ptrDevice->CheckFeatureSupport(D3D12_FEATURE_EXISTING_HEAPS, &existingHeapSupportQuery, sizeof(D3D12_FEATURE_DATA_EXISTING_HEAPS)))) {
        m_siExistingHeapSupport = existingHeapSupportQuery.Supported;
    }

    // Gennerate strings
    #if !defined(SHIPPING)
        // External Objects
        m_options0.gennerateTextRepresentation();
        m_options1.gennerateTextRepresentation();
        m_options2.gennerateTextRepresentation();
        m_options3.gennerateTextRepresentation();
        m_options4.gennerateTextRepresentation();
        m_options5.gennerateTextRepresentation();
        m_options6.gennerateTextRepresentation();
        m_options7.gennerateTextRepresentation();

        m_architecture1.gennerateTextRepresentation();
        m_virtualAddress.gennerateTextRepresentation();
        m_crossNode.gennerateTextRepresentation();

        // String builder
        std::stringstream ss;

        // D3D_SHADER_MODEL
        m_siHighstShaderModell.strName = "Shader model";
        switch (m_siHighstShaderModell.data) {
            case D3D_SHADER_MODEL_5_1:
                m_siHighstShaderModell.strValue = "5.1";
            break;
            case D3D_SHADER_MODEL_6_0:
                m_siHighstShaderModell.strValue = "6.0";
                break;
            case D3D_SHADER_MODEL_6_1:
                m_siHighstShaderModell.strValue = "6.1";
                break;
            case D3D_SHADER_MODEL_6_2:
                m_siHighstShaderModell.strValue = "6.2";
                break;
            case D3D_SHADER_MODEL_6_3:
                m_siHighstShaderModell.strValue = "6.3";
                break;
            case D3D_SHADER_MODEL_6_4:
                m_siHighstShaderModell.strValue = "6.4";
                break;
            case D3D_SHADER_MODEL_6_5:
                m_siHighstShaderModell.strValue = "6.5";
                break;
            case D3D_SHADER_MODEL_6_6:
                m_siHighstShaderModell.strValue = "6.6";
                break;
        }

        // D3D_ROOT_SIGNATURE_VERSION 
        m_siRootSignatureVersion.strName = "Root-Signature version";
        switch (m_siRootSignatureVersion.data) {
            case D3D_ROOT_SIGNATURE_VERSION_1_0:
                m_siRootSignatureVersion.strValue = "1.0";
                break;
            case D3D_ROOT_SIGNATURE_VERSION_1_1:
                m_siRootSignatureVersion.strValue = "1.1";
                break;
        }

        // D3D12_SHADER_CACHE_SUPPORT_FLAGS 
        m_siShaderCache.strName = "Supported shader cache";
        BOOL shaderCacheHad = FALSE;
        if (m_siShaderCache.data & D3D12_SHADER_CACHE_SUPPORT_SINGLE_PSO) {
            ss << "SINGLE_PSO";
            shaderCacheHad = TRUE;
        }
        if (m_siShaderCache.data & D3D12_SHADER_CACHE_SUPPORT_LIBRARY) {
            if (shaderCacheHad) {
                ss << ", ";
            }
            ss << "LIBRARY";
            shaderCacheHad = TRUE;
        }
        if (m_siShaderCache.data & D3D12_SHADER_CACHE_SUPPORT_AUTOMATIC_INPROC_CACHE) {
            if (shaderCacheHad) {
                ss << ", ";
            }
            ss << "INPROC_CACHE";
            shaderCacheHad = TRUE;
        }
        if (m_siShaderCache.data & D3D12_SHADER_CACHE_SUPPORT_AUTOMATIC_DISK_CACHE) {
            if (shaderCacheHad) {
                ss << ", ";
            }
            ss << "AUTOMATIC_DISK_CACHE";
        }
        m_siShaderCache.strValue = ss.str();

        // D3D12_FEATURE_EXISTING_HEAPS
        m_siExistingHeapSupport.strName = "Existing heap support";
        if (m_siExistingHeapSupport.data) {
            m_siExistingHeapSupport.strValue = "Yes";
        }
        else {
            m_siExistingHeapSupport.strValue = "No";
        }

    #endif

    
}

GxDirect::XFeatureSupport::~XFeatureSupport() {
    COM_RELEASE(m_ptrDevice);
}

UINT GxDirect::XFeatureSupport::checkMsaaSupport(DXGI_FORMAT formate, UINT sampleCount) {
    // Create struct
    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msaaQuery;
    ZeroMemory(&msaaQuery, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));

    // Fill info
    msaaQuery.Format = formate;
    msaaQuery.SampleCount = sampleCount;

    // Query
    if (SUCCEEDED(m_ptrDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msaaQuery, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS)))) {
        return msaaQuery.NumQualityLevels;
    }

    // Fallback null
    return 0;
}

BOOL GxDirect::XFeatureSupport::checkFormatSupport(DXGI_FORMAT format, D3D12_FORMAT_SUPPORT1* ptrOutputFormat1, D3D12_FORMAT_SUPPORT2* ptrOutputFormat2) {
    // Create struct
    D3D12_FEATURE_DATA_FORMAT_SUPPORT formatQuery;
    ZeroMemory(&formatQuery, sizeof(D3D12_FEATURE_DATA_FORMAT_SUPPORT));

    // Fill in info
    formatQuery.Format = format;

    // Query
    if (SUCCEEDED(m_ptrDevice->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &formatQuery, sizeof(D3D12_FEATURE_DATA_FORMAT_SUPPORT)))) {
        // Copy values and return true
        *ptrOutputFormat1 = formatQuery.Support1;
        *ptrOutputFormat2 = formatQuery.Support2;
        return TRUE;
    }

    // Fallback false
    return FALSE;
}

BOOL GxDirect::XFeatureSupport::checkCommandQueuePriority(D3D12_COMMAND_LIST_TYPE listType, D3D12_COMMAND_QUEUE_PRIORITY priority) {
    // Create Struckt
    D3D12_FEATURE_DATA_COMMAND_QUEUE_PRIORITY quePrioQuery;
    ZeroMemory(&quePrioQuery, sizeof(D3D12_FEATURE_DATA_COMMAND_QUEUE_PRIORITY));

    // Fill information
    quePrioQuery.CommandListType = listType;
    quePrioQuery.Priority = priority;

    // Query
    if (SUCCEEDED(m_ptrDevice->CheckFeatureSupport(D3D12_FEATURE_COMMAND_QUEUE_PRIORITY, &quePrioQuery, sizeof(D3D12_FEATURE_DATA_COMMAND_QUEUE_PRIORITY)))) {
        return quePrioQuery.PriorityForTypeIsSupported;
    }

    // Fallback false
    return FALSE;
}

D3D12_HEAP_SERIALIZATION_TIER GxDirect::XFeatureSupport::getHeapSerializationTierForNode(UINT nodeIndex) {
    // Create query struct
    D3D12_FEATURE_DATA_SERIALIZATION heapSerQuery;
    ZeroMemory(&heapSerQuery, sizeof(D3D12_FEATURE_DATA_SERIALIZATION));

    // Fill
    heapSerQuery.NodeIndex = nodeIndex;

    // Query
    HRESULT hr;
    if (FAILED(hr = m_ptrDevice->CheckFeatureSupport(D3D12_FEATURE_SERIALIZATION, &heapSerQuery, sizeof(D3D12_FEATURE_DATA_SERIALIZATION)))) {
        throw EXEPTION_HR(L"ID3D12Devive->CheckFeatureSupport(...) for D3D12_FEATURE_SERIALIZATION", hr);
    }
    
    // Return result
    return heapSerQuery.HeapSerializationTier;
}

const TSysInfo(D3D_SHADER_MODEL)& GxDirect::XFeatureSupport::getShaderModel() {
    return m_siHighstShaderModell;
}

const TSysInfo(D3D_ROOT_SIGNATURE_VERSION)& GxDirect::XFeatureSupport::getRootSignatureVersion() {
    return m_siRootSignatureVersion;
}

const TSysInfo(D3D12_SHADER_CACHE_SUPPORT_FLAGS)& GxDirect::XFeatureSupport::getShaderCacheSupportFlags() {
    return m_siShaderCache;
}

const TSysInfo(BOOL)& GxDirect::XFeatureSupport::getExisitingHeapSupport() {
    return m_siExistingHeapSupport;
}

GxDirect::FeaturSupport::FS_Options0& GxDirect::XFeatureSupport::getOptions0() {
    return m_options0;
}

GxDirect::FeaturSupport::FS_Options1& GxDirect::XFeatureSupport::getOptions1() {
    return m_options1;
}

GxDirect::FeaturSupport::FS_Options2& GxDirect::XFeatureSupport::getOptions2(){
    return m_options2;
}

GxDirect::FeaturSupport::FS_Options3& GxDirect::XFeatureSupport::getOptions3() {
    return m_options3;
}

GxDirect::FeaturSupport::FS_Options4& GxDirect::XFeatureSupport::getOptions4() {
    return m_options4;
}

GxDirect::FeaturSupport::FS_Options5& GxDirect::XFeatureSupport::getOptions5() {
    return m_options5;
}

GxDirect::FeaturSupport::FS_Options6& GxDirect::XFeatureSupport::getOptions6() {
    return m_options6;
}

GxDirect::FeaturSupport::FS_Options7& GxDirect::XFeatureSupport::getOptions7() {
    return m_options7;
}

GxDirect::FeaturSupport::FS_Architecture1& GxDirect::XFeatureSupport::getArchitecture1() {
    return m_architecture1;
}

GxDirect::FeaturSupport::FS_VirtualAddress& GxDirect::XFeatureSupport::getVirtualAddress() {
    return m_virtualAddress;
}

GxDirect::FeaturSupport::FS_CrossNode& GxDirect::XFeatureSupport::getCrossNode() {
    return m_crossNode;
}
