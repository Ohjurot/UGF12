#include "pch.h"
#include "Framebuffer.h"

GxRenderIO::FrameBuffer::FrameBuffer(GxDirect::XContext* ptrContext, UINT width, UINT height, DXGI_FORMAT formate, GxRenderIO::FrameBufferUsage usage, ClearValue clearValue, UINT msaaCount, UINT msaaQuality) :
	m_uiWidth(width),
	m_uiHeight(height),
	m_formate(formate),
	m_usage(usage),
	m_uiMsaaCount(msaaCount),
	m_uiMsaaQuality(msaaQuality),
	m_clearValue(clearValue)
{
	// Get device
	ptrContext->getDevice(&m_ptrDevice);

	// If quality is null set count to one
	if (m_uiMsaaCount == 1) {
		m_uiMsaaQuality = 0;
	}

	// Create buffer
	createBuffer();
}

GxRenderIO::FrameBuffer::~FrameBuffer() {
	// Release buffer
	COM_RELEASE(m_ptrBuffer);

	// Release device reference
	COM_RELEASE(m_ptrDevice);
}

void GxRenderIO::FrameBuffer::resize(UINT width, UINT height){
	// Store new values
	m_uiWidth = width;
	m_uiHeight = height;

	// Recreate buffer
	createBuffer();
}

BOOL GxRenderIO::FrameBuffer::createRTV(D3D12_CPU_DESCRIPTOR_HANDLE heapHandleRtv){
	// Return false if rt is not allowed
	if (m_usage != FrameBufferUsage::BUFFER_TEXTURE_RENDER_TARGET) {
		return FALSE;
	}

	// Create RTV descriptor
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesk;
	ZeroMemory(&rtvDesk, sizeof(D3D12_RENDER_TARGET_VIEW_DESC));

	// Describe RTV
	if (m_uiMsaaCount == 1) {
		rtvDesk.Format = m_formate;
		rtvDesk.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvDesk.Texture2D.MipSlice = 0; // No MipMaping or planeslicing
		rtvDesk.Texture2D.PlaneSlice = 0;
	}
	else {
		rtvDesk.Format = m_formate;
		rtvDesk.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
	}

	// Create RTV
	m_ptrDevice->CreateRenderTargetView(m_ptrBuffer, &rtvDesk, heapHandleRtv);

	// Seems ok return true
	return TRUE;
}

BOOL GxRenderIO::FrameBuffer::createDSV(D3D12_CPU_DESCRIPTOR_HANDLE heapHandleDsv, D3D12_DSV_FLAGS dsvFlags) {
	// Return false if depth is not allowed
	if (m_usage != FrameBufferUsage::BUFFER_TEXTURE_DEPTH) {
		return FALSE;
	}

	// Create DSV descriptor
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesk;
	ZeroMemory(&dsvDesk, sizeof(D3D12_DEPTH_STENCIL_VIEW_DESC));

	// Desribe DSV
	if (m_uiMsaaCount == 1) {
		dsvDesk.Format = m_formate;
		dsvDesk.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesk.Flags = dsvFlags;
		dsvDesk.Texture2D.MipSlice = 0; // No mip maping
	}
	else {
		dsvDesk.Format = m_formate;
		dsvDesk.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
	}

	// Create DSV
	m_ptrDevice->CreateDepthStencilView(m_ptrBuffer, &dsvDesk, heapHandleDsv);

	// Seems ok return true
	return TRUE;
}

void GxRenderIO::FrameBuffer::createSRV(D3D12_CPU_DESCRIPTOR_HANDLE heapHandleSrv) {
	// Create SRV descriptor
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesk;
	ZeroMemory(&srvDesk, sizeof(D3D12_SHADER_RESOURCE_VIEW_DESC));

	// Describe SRV
	if (m_uiMsaaCount == 1) {
		srvDesk.Format = m_formate;
		srvDesk.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesk.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING; // Mapping can be default size it is not packed specialy or loaded from disk
		srvDesk.Texture2D.MostDetailedMip = 0;
		srvDesk.Texture2D.MipLevels = 1;
		srvDesk.Texture2D.PlaneSlice = 0;
		srvDesk.Texture2D.ResourceMinLODClamp = 0.0f;
	}
	else {
		srvDesk.Format = m_formate;
		srvDesk.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
		srvDesk.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING; // Mapping can be default size it is not packed specialy or loaded from disk
	}

	// Create SRV
	m_ptrDevice->CreateShaderResourceView(m_ptrBuffer, &srvDesk, heapHandleSrv);
}

D3D12_RESOURCE_STATES GxRenderIO::FrameBuffer::getState() {
	// Return next lates state of this resource
	return m_resState;
}

ID3D12Resource* GxRenderIO::FrameBuffer::getResourcePointer() {
	// Return this reouse pointer
	return m_ptrBuffer;
}

D3D12_GPU_VIRTUAL_ADDRESS GxRenderIO::FrameBuffer::getGpuAddress() {
	// Return GPU address of the buffer
	return m_ptrBuffer->GetGPUVirtualAddress();
}

void GxRenderIO::FrameBuffer::barrier(D3D12_RESOURCE_STATES newState, ID3D12GraphicsCommandList* ptrCmdList){
	// Check for fast return
	if (newState == m_resState) {
		return;
	}
	
	// Create barrier descriptor
	D3D12_RESOURCE_BARRIER barrier;
	ZeroMemory(&barrier, sizeof(D3D12_RESOURCE_BARRIER));

	// Describe barrier (Resource transsition)
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_ptrBuffer;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = m_resState;
	barrier.Transition.StateAfter = newState;

	// Add barrier to list
	ptrCmdList->ResourceBarrier(1, &barrier);

	// Update internal state
	m_resState = newState;
}

void GxRenderIO::FrameBuffer::setName(LPCWSTR name){
	#ifdef _DEBUG
	// Stream name
	std::wstringstream wss;
	wss << L"Framebuffer: " << name;

	// Safe name
	m_strName = name;

	// Set name
	m_ptrBuffer->SetName(wss.str().c_str());
	#endif
}

void GxRenderIO::FrameBuffer::createBuffer() {
	// Free buffer if exists
	COM_RELEASE(m_ptrBuffer);

	// Create descriptor for heap properties
	D3D12_HEAP_PROPERTIES heapProDesc;
	ZeroMemory(&heapProDesc, sizeof(D3D12_HEAP_PROPERTIES));

	// Create descriptor for resource description
	D3D12_RESOURCE_DESC resDesc;
	ZeroMemory(&resDesc, sizeof(D3D12_RESOURCE_DESC));

	// Create descriptor for clear value
	D3D12_CLEAR_VALUE clearValue;
	ZeroMemory(&clearValue, sizeof(D3D12_CLEAR_VALUE));

	// Describe heap properties
	heapProDesc.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProDesc.CreationNodeMask = NULL;
	heapProDesc.VisibleNodeMask = NULL;

	// Describe resource
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Alignment = NULL;
	resDesc.Width = m_uiWidth;
	resDesc.Height = m_uiHeight;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;	// No mip maping
	resDesc.Format = m_formate;
	resDesc.SampleDesc.Count = m_uiMsaaCount;
	resDesc.SampleDesc.Quality = m_uiMsaaQuality;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN; // Never access by anyone elese than gpu -> let the gpu specify the layout
	
	// Set flag for resource
	switch (m_usage) {
		// Default no flag
		case FrameBufferUsage::BUFFER_TEXTURE:
			{
				resDesc.Flags = D3D12_RESOURCE_FLAG_NONE; 
			}
			break;

		// Render target -> allow render target
		case FrameBufferUsage::BUFFER_TEXTURE_RENDER_TARGET:
			{
				resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

				// Clear values for render targets
				memcpy(clearValue.Color, m_clearValue.color, sizeof(FLOAT) * 4);
			}
			break;

		// Depth buffer -> allow depth stencil
		case FrameBufferUsage::BUFFER_TEXTURE_DEPTH:
			{
				resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

				// Clear values for depth
				clearValue.DepthStencil = m_clearValue.depthValue;
			}
			break;
	}

	// Describe clear value formate
	clearValue.Format = m_formate;

	// Create buffer
	HRESULT hr;
	if (FAILED(hr = m_ptrDevice->CreateCommittedResource(
		&heapProDesc,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&clearValue,
		IID_PPV_ARGS(&m_ptrBuffer)
	))) {
		throw EXEPTION_HR(L"ID3D12Device->CreateCommittedResource(...)", hr);
	}

	// Name buffer on debug
	#ifdef _DEBUG
		// Stream Name
		std::wstringstream wss;
		wss << L"Framebuffer: " << m_strName;
		
		// Set name
		m_ptrBuffer->SetName(wss.str().c_str());
	#endif

	// Set state
	m_resState = D3D12_RESOURCE_STATE_COMMON;
}
