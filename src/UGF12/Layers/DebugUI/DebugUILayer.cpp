#include "pch.h"
#include "DebugUILayer.h"

UGF12::DebugUI::DebugUILayer::DebugUILayer(GxDirect::XContext* ptrContext, GxDirect::XWindow* ptrWindow, BOOL* vsyncPtr, BOOL visible) :
	#if !defined(SHIPPING)	
		GxRenderIO::LayerStack::ILayerImpl(L"DebugUI", visible),
	#else
		GxRenderIO::LayerStack::ILayerImpl(L"DebugUI", FALSE),
	#endif
	m_ptrContext(ptrContext),
	m_ptrWindow(ptrWindow),
	m_ptrVsyncBool(vsyncPtr)
{ }

void UGF12::DebugUI::DebugUILayer::Init() {
	// Refcounting
	m_ptrContext->IncRef();

	// Get Device
	ID3D12Device* ptrDevice;
	m_ptrContext->getDevice(&ptrDevice);

	// Describe imgui heap
	D3D12_DESCRIPTOR_HEAP_DESC heapDesk;
	ZeroMemory(&heapDesk, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));

	heapDesk.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesk.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesk.NumDescriptors = 1;
	heapDesk.NodeMask = NULL;

	// Create ImGui Heap
	HRESULT hr;
	if (FAILED(hr = ptrDevice->CreateDescriptorHeap(&heapDesk, IID_PPV_ARGS(&m_ptrImGuiHeap)))){
		throw EXEPTION_HR(L"ID3D12Device->CreateDescriptorHeap(...)", hr);
	}

	// Init ImGui
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	// Init ImGui impl
	ImGui_ImplWin32_Init(m_ptrWindow->getHandle());
	ImGui_ImplDX12_Init(ptrDevice, 3, DXGI_FORMAT_R8G8B8A8_UNORM, m_ptrImGuiHeap, m_ptrImGuiHeap->GetCPUDescriptorHandleForHeapStart(), m_ptrImGuiHeap->GetGPUDescriptorHandleForHeapStart());

	// Describe RTV Heap
	heapDesk.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesk.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesk.NumDescriptors = 1;
	heapDesk.NodeMask = NULL;

	// Create RTV Heap
	if (FAILED(hr = ptrDevice->CreateDescriptorHeap(&heapDesk, IID_PPV_ARGS(&m_ptrHeapRtv)))) {
		throw EXEPTION_HR(L"ID3D12Device->CreateDescriptorHeap(...)", hr);
	}

	// Release device
	COM_RELEASE(ptrDevice);

	// Create UI
	m_ptrInfoHint = new UGF12::DebugUI::UI_InfoHint(m_ptrContext, TRUE);
	m_ptrMainMenue = new UGF12::DebugUI::UI_MainMenue(m_ptrContext, m_ptrWindow, m_ptrInfoHint, m_ptrVsyncBool);
}

void UGF12::DebugUI::DebugUILayer::Destroy() {
	// Destroy UI
	delete m_ptrInfoHint;
	delete m_ptrMainMenue;
	
	// Shutdown ImGui
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// Release heap
	COM_RELEASE(m_ptrHeapRtv);
	COM_RELEASE(m_ptrImGuiHeap);

	// Ref counting
	m_ptrContext->DecRef();
}

void UGF12::DebugUI::DebugUILayer::draw(GxRenderIO::LayerStack::LayerFrameInfo* ptrFrameInfo, GxRenderIO::CmdListProxy* ptrCmdListProxy, GxRenderIO::FrameBuffer* ptrFrameBuffer) {
	// Set buffer state to render target
	ptrFrameBuffer->barrier(D3D12_RESOURCE_STATE_RENDER_TARGET, ptrCmdListProxy->get());
	
	// Imgui new frame
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Get ImGui IO
	ImGuiIO* ptrIo = &ImGui::GetIO();
	m_ptrInfoHint->draw(ptrFrameInfo, ptrIo);
	m_ptrMainMenue->draw(ptrFrameInfo, ptrIo);

	// Create RTV Handle
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_ptrHeapRtv->GetCPUDescriptorHandleForHeapStart();

	// Set and clear RTV
	ptrCmdListProxy->get()->OMSetRenderTargets(1, &handle, 0, NULL);
	ptrCmdListProxy->get()->ClearRenderTargetView(handle, ptrFrameBuffer->getClearValue().color, 0, NULL);

	// Set viewport
	ptrCmdListProxy->get()->RSSetViewports(1, &m_viewPort);
	ptrCmdListProxy->get()->RSSetScissorRects(1, &m_sicRect);

	// Set Descritor Heap for ImGui
	ptrCmdListProxy->get()->SetDescriptorHeaps(1, &m_ptrImGuiHeap);

	// Render Imgui and render date to cmd list
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), ptrCmdListProxy->get());

	// Set buffer state to read
	ptrFrameBuffer->barrier(D3D12_RESOURCE_STATE_COMMON, ptrCmdListProxy->get());
}

void UGF12::DebugUI::DebugUILayer::onResourceChange(UINT resource, UINT index, void* ptrResource) {
	switch (resource) {
		case UGF12_RESOURCE_TYPE_LAYER_FRAMEBUFFER:
			// Cast to buffer
			GxRenderIO::FrameBuffer* ptrBuffer = (GxRenderIO::FrameBuffer*)ptrResource;

			// Create handle
			D3D12_CPU_DESCRIPTOR_HANDLE hRtv = m_ptrHeapRtv->GetCPUDescriptorHandleForHeapStart();

			// Create View
			ptrBuffer->createRTV(hRtv);
			break;
	}
}

void UGF12::DebugUI::DebugUILayer::onResize(UINT width, UINT height) {
	// Set view port
	m_viewPort.TopLeftX = 0.0f;
	m_viewPort.TopLeftY = 0.0f;
	m_viewPort.Width = (FLOAT)width;
	m_viewPort.Height = (FLOAT)height;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;

	// Set rect
	m_sicRect = { 0, 0, (LONG)width, (LONG)height };
}

BOOL UGF12::DebugUI::DebugUILayer::onEnable() {
	#if !defined(SHIPPING)	
		return TRUE;
	#else
		return FALSE;
	#endif
}

BOOL UGF12::DebugUI::DebugUILayer::handleWindowMessage(LRESULT* ptrResult, HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(wnd, msg, wParam, lParam)) {
		// Return true if handled
		*ptrResult = NULL;
		return TRUE;
	}
	return FALSE;
}
