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

	// Create device
	if (FAILED(hr = D3D12CreateDevice(ptrTargetAdapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_ptrDevice)))) {
		throw EXEPTION_HR(L"D3D12CreateDevice(...)", hr);
	}

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
