#include "pch.h"
#include "XWndDriver.h"

GxDirect::XWndDriver::XWndDriver(HWND targetHwnd, GxDirect::XContext* ptrContext, GxDirect::XCmdQueue* ptrCmdQue) {
	HRESULT hr;
	
	// Get Device
	ptrContext->getDevice(&m_ptrDevice);

	// Get CommanQue
	ptrCmdQue->getCommandQueue(&m_ptrCmdQue);
	
	// Create factory
	IDXGIFactory* ptrFactory = NULL;
	if (FAILED(hr = CreateDXGIFactory(IID_PPV_ARGS(&ptrFactory)))) {
		throw EXEPTION_HR(L"", hr);
	}

	// Get window bounds
	RECT cr;
	if (GetClientRect(targetHwnd, &cr) == NULL) {
		throw EXEPTION_HR(L"XWndDriver(...) HWND invalid", GetLastError());
	}

	// Create swap chain descriptor
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	// Desribe swap chain
	desc.BufferDesc.Width = cr.right - cr.left;									// Width and Height of window
	desc.BufferDesc.Height = cr.bottom - cr.top;
	desc.BufferDesc.RefreshRate.Denominator = 1;								// 60 Hz Refresh rate
	desc.BufferDesc.RefreshRate.Numerator = 60;	
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;						// Default Buffer formate
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// No Scanline Ordering
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					// No Scaling
	desc.SampleDesc.Count = 1;													// MSAA Swap chain is not supprted by directx 12
	desc.SampleDesc.Quality = 0;
	desc.BufferCount = 3;														// Tripple buffering
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;							// Is a render target
	desc.OutputWindow = targetHwnd;												// Window
	desc.Windowed = TRUE;														// Initaliy not fullscreen
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;							// Swap effect flip and discard old data
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;						// Allow buffer resize

	// Create swap chain
	if (FAILED(hr = ptrFactory->CreateSwapChain(m_ptrCmdQue, &desc, &m_ptrSwapChain))) {
		throw EXEPTION_HR(L"IDXGIFactory->CreateSwapChain(...)", hr);
	}

	// Get rtv desc heap increment size
	m_uiRtvIncrementSize = m_ptrDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// Create heap descriptor
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	ZeroMemory(&heapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));

	// Describe descriptor heap
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = 3;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.NodeMask = NULL; 

	// Create descriptor heap
	if (FAILED(hr = m_ptrDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_ptrRtvHeap)))) {
		throw EXEPTION_HR(L"ID3D12Device->CreateDescriptorHeap(...)", hr);
	}

	// Get bufers
	getSwapchainBuffers();

	// Release factory and device (local ref)
	COM_RELEASE(ptrFactory);
}

GxDirect::XWndDriver::~XWndDriver() {
	// Release back buffers
	for (UINT i = 0; i < 3; i++) {
		COM_RELEASE(m_ptrsBackBuffers[i]);
	}

	// Release heap
	COM_RELEASE(m_ptrRtvHeap);

	// Release swap chain
	COM_RELEASE(m_ptrSwapChain);

	// Release que
	COM_RELEASE(m_ptrCmdQue);

	// Release device
	COM_RELEASE(m_ptrDevice);
}

void GxDirect::XWndDriver::beginFrame(ID3D12GraphicsCommandList* ptrCmdList) {
	// Create Barrier
	D3D12_RESOURCE_BARRIER baPresentToRt;
	ZeroMemory(&baPresentToRt, sizeof(D3D12_RESOURCE_BARRIER));

	// Resource barrier for current rt from present to render target
	baPresentToRt.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	baPresentToRt.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	baPresentToRt.Transition.pResource = m_ptrsBackBuffers[m_uiCurrentBackBuffer];
	baPresentToRt.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	baPresentToRt.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	baPresentToRt.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	// Que barrier
	ptrCmdList->ResourceBarrier(1, &baPresentToRt);

	// Get handle to current rtv descriptor
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_ptrRtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += m_uiRtvIncrementSize * (SIZE_T)m_uiCurrentBackBuffer;

	// Set render target
	ptrCmdList->OMSetRenderTargets(1, &rtvHandle, TRUE, NULL);

	// Clear rtv
	static FLOAT color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ptrCmdList->ClearRenderTargetView(rtvHandle, color, 0, NULL);
}

void GxDirect::XWndDriver::endFrame(ID3D12GraphicsCommandList* ptrCmdList) {
	// Create Barrier
	D3D12_RESOURCE_BARRIER baRtToPresent;
	ZeroMemory(&baRtToPresent, sizeof(D3D12_RESOURCE_BARRIER));

	// Resource barrier for current rt from render target to present
	baRtToPresent.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	baRtToPresent.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	baRtToPresent.Transition.pResource = m_ptrsBackBuffers[m_uiCurrentBackBuffer];
	baRtToPresent.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	baRtToPresent.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	baRtToPresent.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	// Que barrier
	ptrCmdList->ResourceBarrier(1, &baRtToPresent);
}

UINT GxDirect::XWndDriver::present(BOOL vsync){
	// Increment index
	m_uiCurrentBackBuffer = (m_uiCurrentBackBuffer + 1) % 3;

	// Present frame
	m_ptrSwapChain->Present(vsync, NULL);

	// Return new back buffer index
	return m_uiCurrentBackBuffer;
}

void GxDirect::XWndDriver::resizeSwapChain(UINT width, UINT height) {
	HRESULT hr;

	// Release resources
	for (UINT i = 0; i < 3; i++) {
		COM_RELEASE(m_ptrsBackBuffers[i]);
	}
	
	// Resize swap chain and preserver buffer settings
	if (FAILED(hr = m_ptrSwapChain->ResizeBuffers(3, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH))) {
		throw EXEPTION_HR(L"IDXGISwapChain->ResizeBuffers(...)", hr);
	}

	// Recreate buffers
	getSwapchainBuffers();
}

void GxDirect::XWndDriver::setFullscreen(BOOL fullscreen) {
	HRESULT hr;
	
	// Set fullscreen mode to input and chose output automaticly
	if (FAILED(hr = m_ptrSwapChain->SetFullscreenState(fullscreen, NULL))) {
		throw EXEPTION_HR(L"IDXGISwapChain->SetFullscreenState(...)", hr);
	}
}

BOOL GxDirect::XWndDriver::isFullscreen(){
	HRESULT hr;

	// Bool for return 
	BOOL fScreen;

	// Get fullscreen state
	if (FAILED(hr = m_ptrSwapChain->GetFullscreenState(&fScreen, NULL))) {
		throw EXEPTION_HR(L"IDXGISwapChain->GetFullscreenState(...)", hr);
	}

	// Return state
	return fScreen;
}

void GxDirect::XWndDriver::getSwapchainBuffers(){
	HRESULT hr;
	
	// Get all render target views
	for (UINT i = 0; i < 3; i++) {
		// Get buffer
		if (FAILED(hr = m_ptrSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_ptrsBackBuffers[i])))) {
			throw EXEPTION_HR(L"IDXGISwapChain->GetBuffer(...)", hr);
		}

		// Create heap handle
		D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = m_ptrRtvHeap->GetCPUDescriptorHandleForHeapStart();
		heapHandle.ptr += m_uiRtvIncrementSize * (SIZE_T)i;

		// Create rtv
		m_ptrDevice->CreateRenderTargetView(m_ptrsBackBuffers[i], NULL, heapHandle);

		#ifdef _DEBUG
			// Stream back buffer name
			std::wstringstream wss;
			wss << L"Back Buffer #" << i;
			// Name back buffer
			m_ptrsBackBuffers[i]->SetName(wss.str().c_str());
		#endif
	}

	// Reset current back buffer index
	m_uiCurrentBackBuffer = 0;
}
