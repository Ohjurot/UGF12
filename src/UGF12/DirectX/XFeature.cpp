#include "pch.h"
#include "XFeature.h"

UINT GxDirect::XFeature::msaaMaxQuality(GxDirect::XContext* ptrContext, UINT msaaCount, DXGI_FORMAT fbFormat) {
    // Get device
    ID3D12Device* ptrDevice;
    ptrContext->getDevice(&ptrDevice);

    // Value to return
    UINT quality = 0;

    // Create data struct for query
    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msaaQData;
    ZeroMemory(&msaaQData, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));

    // Setup init values
    msaaQData.Format = fbFormat;
    msaaQData.SampleCount = msaaCount;
    msaaQData.NumQualityLevels = 0;
    msaaQData.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;

    // Do Feature check
    if (SUCCEEDED(ptrDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msaaQData, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS)))) {
        // Check if support
        if (msaaQData.NumQualityLevels > 0) {
            quality = msaaQData.NumQualityLevels;
        }
    }

    // Free device 
    COM_RELEASE(ptrDevice);

    // Return value
    return quality;
}
