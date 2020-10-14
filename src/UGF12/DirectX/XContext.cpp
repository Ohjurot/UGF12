#include "pch.h"
#include "XContext.h"

GxDirect::XContext::XContext(std::wstring strPreferedGpu, INT idxPreveredGpu, BOOL debugLayer) {
	HRESULT hr;
	IDXGIAdapter* ptrTargetAdapter = NULL;
	BOOL gpuFound = FALSE;

	// Create Factory
	IDXGIFactory* ptrFactory = NULL;
	if (FAILED(hr = CreateDXGIFactory(IID_PPV_ARGS(&ptrFactory)))) {
		throw EXEPTION_HR(L"CreateDXGIFactory(...)", hr);
	}

	// Find Adapter by index and name
	if (idxPreveredGpu >= 0 && ptrFactory->EnumAdapters(idxPreveredGpu, &ptrTargetAdapter) == S_OK) {
		// Extract info
		DXGI_ADAPTER_DESC gpuDesk;
		ZeroMemory(&gpuDesk, sizeof(DXGI_ADAPTER_DESC));
		if (FAILED(hr = ptrTargetAdapter->GetDesc(&gpuDesk))) {
			throw EXEPTION_HR(L"IDXGIAdapter->GetDesc(...)", hr);
		}

		// Check if names match
		if (wcscmp(gpuDesk.Description, strPreferedGpu.c_str()) == 0) {
			gpuFound = TRUE;
		}
		else {
			COM_RELEASE(ptrTargetAdapter);
		}
	}

	// Try to find by name
	if (!gpuFound && strPreferedGpu.length() > 0) {
		// For every adaper
		UINT idx = 0;
		while (ptrFactory->EnumAdapters(idx, &ptrTargetAdapter) == S_OK) {
			// Extract info
			DXGI_ADAPTER_DESC gpuDesk;
			ZeroMemory(&gpuDesk, sizeof(DXGI_ADAPTER_DESC));
			if (FAILED(hr = ptrTargetAdapter->GetDesc(&gpuDesk))) {
				throw EXEPTION_HR(L"IDXGIAdapter->GetDesc(...)", hr);
			}

			// Check if names match
			if (wcscmp(gpuDesk.Description, strPreferedGpu.c_str()) == 0) {
				gpuFound = TRUE;
				break;
			}

			// Increment index
			COM_RELEASE(ptrTargetAdapter);
			idx++;
		}
	}

	// Get adpater zero if no is found
	if (!gpuFound) {
		if (FAILED(ptrFactory->EnumAdapters(0, &ptrTargetAdapter))) {
			throw EXEPTION(L"In order to use the programm a hardware gpu is required!");
		}

		// Set gpu found for convinience
		gpuFound = TRUE;
	}

	// Factory is no longer required
	COM_RELEASE(ptrFactory);

	if (debugLayer) {
		// Create D3D Debug 
		if (FAILED(hr = D3D12GetDebugInterface(IID_PPV_ARGS(&m_ptrDebug)))) {
			throw EXEPTION_HR(L"D3D12GetDebugInterface(...)", hr);
		}

		// Enable debug layer
		m_ptrDebug->EnableDebugLayer();
	}

	// Create device low featureset (FL 11_0; Min Dx12)
	if (FAILED(hr = D3D12CreateDevice(ptrTargetAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_ptrDevice)))) {
		throw EXEPTION_HR(L"D3D12CreateDevice(...)", hr);
	}

	// Build fd descriptor
	D3D12_FEATURE_DATA_FEATURE_LEVELS fsD;
	ZeroMemory(&fsD, sizeof(D3D12_FEATURE_DATA_FEATURE_LEVELS));

	// Fill infos
	D3D_FEATURE_LEVEL featureList[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_12_1,
		//D3D_FEATURE_LEVEL_12_2, <--- coming soon
	};
	fsD.NumFeatureLevels = 4; // 5
	fsD.pFeatureLevelsRequested = featureList;

	// Check feature support
	if (FAILED(hr = m_ptrDevice->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &fsD, sizeof(D3D12_FEATURE_DATA_FEATURE_LEVELS)))) {
		throw EXEPTION_HR(L"ID3D12Device->CheckFeatureSupport(...)", hr);
	}

	// Rlease low feature
	COM_RELEASE(m_ptrDevice);

	// Create device (max supported feature level)
	if (FAILED(hr = D3D12CreateDevice(ptrTargetAdapter, fsD.MaxSupportedFeatureLevel, IID_PPV_ARGS(&m_ptrDevice)))) {
		throw EXEPTION_HR(L"D3D12CreateDevice(...)", hr);
	}

	// Set creation level
	m_flCreate = fsD.MaxSupportedFeatureLevel;

	// Create debug device if required
	if (debugLayer) {
		// Get debug device
		if (FAILED(hr = m_ptrDevice->QueryInterface(IID_PPV_ARGS(&m_ptrDebugDevice)))) {
			throw EXEPTION_HR(L"ID3D12Device->QueryInterface(...) for ID3D12DebugDevice", hr);
		}
	}

	// Get info about current gpu
	ZeroMemory(&m_adapterInfo, sizeof(DXGI_ADAPTER_DESC));
	if (FAILED(hr = ptrTargetAdapter->GetDesc(&m_adapterInfo))) {
		throw EXEPTION_HR(L"IDXGIAdapter->GetDesc(...)", hr);
	}

	// Adapter is no longer required
	COM_RELEASE(ptrTargetAdapter);

	// Read incrments
	m_uiIncrementRtv = m_ptrDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_uiIncrementDsv = m_ptrDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_uiIncrementSrv = m_ptrDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Build String
	std::wstringstream wss;
	
	// DX Feature level
	wss << L"DX12 (FL";
	switch (m_flCreate) {
		case D3D_FEATURE_LEVEL_11_0:
			wss << L"11_0)";
			break;
		case D3D_FEATURE_LEVEL_11_1:
			wss << L"11_1)";
			break;
		case D3D_FEATURE_LEVEL_12_0:
			wss << L"12_0)";
			break;
		case D3D_FEATURE_LEVEL_12_1:
			wss << L"12_1)";
			break;
	}

	// Windows sdk
	wss << L" SDK Version: ";
	switch (WDK_NTDDI_VERSION) {
		case NTDDI_WIN10:
			wss << L"Win10";
			break;
		case NTDDI_WIN10_TH2:
			wss << L"Win10 TH2";
			break;
		case NTDDI_WIN10_RS1:
			wss << L"Win10 RS1";
			break;
		case NTDDI_WIN10_RS2:
			wss << L"Win10 RS2";
			break;
		case NTDDI_WIN10_RS3:
			wss << L"Win10 RS3";
			break;
		case NTDDI_WIN10_RS4:
			wss << L"Win10 RS4";
			break;
		case NTDDI_WIN10_RS5:
			wss << L"Win10 RS5";
			break;
		case NTDDI_WIN10_19H1:
			wss << L"Win10 19H1";
			break;
		case NTDDI_WIN10_VB:
			wss << L"Win10 VB";
			break;
	}

	m_wstrFeatures = wss.str();
}

GxDirect::XContext::~XContext() {
	// Report livetime object
	if (m_ptrDebugDevice) {
		m_ptrDebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
	}

	// Release debug stuff first
	COM_RELEASE(m_ptrDebugDevice);
	COM_RELEASE(m_ptrDebug);

	// Finaly release devie
	COM_RELEASE(m_ptrDevice);
}

void GxDirect::XContext::getDevice(ID3D12Device** ppDevice){
	// Set pointer
	*ppDevice = m_ptrDevice;

	// Add ref
	m_ptrDevice->AddRef();
}

DXGI_ADAPTER_DESC GxDirect::XContext::getGpuInfo() {
	// Return pointer to internal info
	return m_adapterInfo;
}

D3D_FEATURE_LEVEL GxDirect::XContext::getFeatureLevel() {
	return m_flCreate;
}

std::wstring& GxDirect::XContext::getFeatureString() {
	return m_wstrFeatures;
}

UINT GxDirect::XContext::getIncrmentSrv() {
	return m_uiIncrementSrv;
}

UINT GxDirect::XContext::getIncrmentDsv() {
	return m_uiIncrementDsv;
}

UINT GxDirect::XContext::getIncrmentRtv() {
	return m_uiIncrementRtv;
}
