#include "pch.h"
#include "LayerStackManager.h"

GxRenderIO::LayerStack::Manager::Manager(GxDirect::XContext* ptrContext, UINT cmdListCount, UINT width, UINT height) :
	m_uiWidth(width),
	m_uiHeight(height),
	m_ptrContext(ptrContext)
{
	HRESULT hr;
	
	// Increment ref count
	m_ptrContext->IncRef();

	// Create que and manager
	m_ptrCmdQues = new GxDirect::XCmdQueue(ptrContext);
	m_ptrCmdListManger = new GxRenderIO::CmdListManger(ptrContext, m_ptrCmdQues, cmdListCount);

	// Compile shader
	GxRenderIO::LayerStack::Shader::compile();

	// Get device
	ID3D12Device* ptrDevice;
	ptrContext->getDevice(&ptrDevice);

	// Heap descritor default
	D3D12_HEAP_PROPERTIES heapPropDesk_default;
	ZeroMemory(&heapPropDesk_default, sizeof(D3D12_HEAP_PROPERTIES));

	heapPropDesk_default.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapPropDesk_default.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapPropDesk_default.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapPropDesk_default.CreationNodeMask = NULL;
	heapPropDesk_default.VisibleNodeMask = NULL;

	// Heap descritor upload
	D3D12_HEAP_PROPERTIES heapPropDesk_upload;
	ZeroMemory(&heapPropDesk_upload, sizeof(D3D12_HEAP_PROPERTIES));

	heapPropDesk_upload.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapPropDesk_upload.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapPropDesk_upload.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapPropDesk_upload.CreationNodeMask = NULL;
	heapPropDesk_upload.VisibleNodeMask = NULL;

	// Resource descrition for const buffer
	D3D12_RESOURCE_DESC resDesk_cbuf;
	ZeroMemory(&resDesk_cbuf, sizeof(D3D12_RESOURCE_DESC));

	resDesk_cbuf.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesk_cbuf.Alignment = 0;
	resDesk_cbuf.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resDesk_cbuf.Width = sizeof(GxRenderIO::LayerStack::Manager::MetaCBuffer);
	resDesk_cbuf.Height = 1;
	resDesk_cbuf.DepthOrArraySize = 1;
	resDesk_cbuf.MipLevels = 1;
	resDesk_cbuf.SampleDesc.Count = 1;
	resDesk_cbuf.SampleDesc.Quality = 0;
	resDesk_cbuf.Flags = D3D12_RESOURCE_FLAG_NONE;

	// Create Constant buffer
	if (FAILED(hr = ptrDevice->CreateCommittedResource(&heapPropDesk_default, D3D12_HEAP_FLAG_NONE, &resDesk_cbuf, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&m_ptrConstBuffer)))) {
		throw EXEPTION_HR(L"ID3D12Device->CreateCommittedResource(...)", hr);
	}

	// Create Upload buffer
	if (FAILED(hr = ptrDevice->CreateCommittedResource(&heapPropDesk_upload, D3D12_HEAP_FLAG_NONE, &resDesk_cbuf, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&m_ptrUploadBuffer)))) {
		throw EXEPTION_HR(L"ID3D12Device->CreateCommittedResource(...)", hr);
	}

	// Name buffers
	#if defined(_DEBUG)
	m_ptrConstBuffer->SetName(L"LayerStack Const Buffer");
	m_ptrUploadBuffer->SetName(L"LayerStack Upload Buffer");
	#endif

	// Create Root Signature
	void* ptrVoidData;
	SIZE_T dataLength;
	GxRenderIO::LayerStack::Shader::getVertexShader(&ptrVoidData, &dataLength);
	if (FAILED(hr = ptrDevice->CreateRootSignature(NULL, ptrVoidData, dataLength, IID_PPV_ARGS(&m_ptrRootSignature)))) {
		throw EXEPTION_HR(L"ID3D12Device->CreateRootSignature(...)", hr);
	}

	// Describe PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesk;
	ZeroMemory(&psoDesk, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	psoDesk.pRootSignature = m_ptrRootSignature;
	GxRenderIO::LayerStack::Shader::getVertexShader((void**)&psoDesk.VS.pShaderBytecode, &psoDesk.VS.BytecodeLength);
	GxRenderIO::LayerStack::Shader::getPixelShader((void**)&psoDesk.PS.pShaderBytecode, &psoDesk.PS.BytecodeLength);
	psoDesk.StreamOutput = {
		NULL, 
		0, 
		NULL, 
		0, 
		0
	};
	psoDesk.BlendState.AlphaToCoverageEnable = FALSE;
	psoDesk.BlendState.IndependentBlendEnable = FALSE;
	psoDesk.BlendState.RenderTarget[0].BlendEnable = TRUE;
	psoDesk.BlendState.RenderTarget[0].LogicOpEnable = FALSE;
	psoDesk.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesk.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	psoDesk.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	psoDesk.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
	psoDesk.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	psoDesk.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	psoDesk.BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	psoDesk.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	psoDesk.RasterizerState = {
		D3D12_FILL_MODE_SOLID,
		D3D12_CULL_MODE_NONE,
		FALSE,
		D3D12_DEFAULT_DEPTH_BIAS,
		D3D12_DEFAULT_DEPTH_BIAS_CLAMP,
		D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
		FALSE,
		FALSE,
		FALSE,
		0,
		D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
	};
	psoDesk.DepthStencilState.DepthEnable = FALSE;
	psoDesk.DepthStencilState.StencilEnable = FALSE;
	psoDesk.SampleMask = UINT_MAX;
	psoDesk.InputLayout.NumElements = 2;
	psoDesk.InputLayout.pInputElementDescs = c_inputLayout;
	psoDesk.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	psoDesk.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesk.NumRenderTargets = 1;
	psoDesk.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesk.SampleDesc.Count = 1;
	psoDesk.SampleDesc.Quality = 0;
	psoDesk.NodeMask = NULL;
	psoDesk.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	// Create PSO
	if (FAILED(hr = ptrDevice->CreateGraphicsPipelineState(&psoDesk, IID_PPV_ARGS(&m_ptrPipelineState)))) {
		throw EXEPTION_HR(L"ID3D12Device->CreateGraphicsPipelineState(...)", hr);
	}

	// Set view port
	m_viewPort.TopLeftX = 0.0f;
	m_viewPort.TopLeftY = 0.0f;
	m_viewPort.Width = (FLOAT)width;
	m_viewPort.Height = (FLOAT)height;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;

	// Set rect
	m_sicRect = { 0, 0, (LONG)width, (LONG)height };

	// Release device
	COM_RELEASE(ptrDevice);
}

GxRenderIO::LayerStack::Manager::~Manager() {	
	// Wait and Delete layers
	for (UINT i = 0; i < m_uiLayersUsed; i++) {
		m_ptrsLayers[i]->waitForFrame();
		delete m_ptrsLayers[i];
	}

	// Delete managers and ques
	m_ptrCmdListManger->flush();
	delete m_ptrCmdListManger;
	delete m_ptrCmdQues;

	// Rlease root and pso
	COM_RELEASE(m_ptrPipelineState);
	COM_RELEASE(m_ptrRootSignature);

	// Release Vertex buffer
	COM_RELEASE(m_ptrVertexBuffer);

	// Release cBuf and upload buffer
	COM_RELEASE(m_ptrUploadBuffer);
	COM_RELEASE(m_ptrConstBuffer);

	// Release Decriptor heap
	COM_RELEASE(m_ptrDecriptorHeap);

	// Decrement ref
	m_ptrContext->DecRef();
}

void GxRenderIO::LayerStack::Manager::insertLayer(GxRenderIO::LayerStack::ILayerImpl* ptrImpl){
	// Check bounds
	if (m_uiLayersUsed == 16) {
		throw EXEPTION(L"LayerStack overflow");
	}

	// Calculate affinity mask
	DWORD affintyMask = (1 << ( m_uiLayersUsed % GxUtil::SystemMetrics::getCpuCoresCount() ));

	// Create layer
	m_ptrsLayers[m_uiLayersUsed] = new GxRenderIO::LayerStack::Layer(m_ptrContext, m_uiWidth, m_uiHeight, ptrImpl, affintyMask);

	// Increment layer
	m_uiLayersUsed++;
}

void GxRenderIO::LayerStack::Manager::init(GxDirect::XCmdList* ptrCmdList) {
	// Get Devide
	ID3D12Device* ptrDevice;
	m_ptrContext->getDevice(&ptrDevice);

	// Describe heap
	D3D12_DESCRIPTOR_HEAP_DESC deskHeapDesk;
	ZeroMemory(&deskHeapDesk, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	deskHeapDesk.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	deskHeapDesk.NumDescriptors = m_uiLayersUsed;
	deskHeapDesk.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	deskHeapDesk.NodeMask = NULL;

	// Create heap
	HRESULT hr = S_OK;
	if (FAILED(hr = ptrDevice->CreateDescriptorHeap(&deskHeapDesk, IID_PPV_ARGS(&m_ptrDecriptorHeap)))) {
		throw EXEPTION_HR(L"ID3D12Device->CreateDescriptorHeap(...)", hr);
	}

	// Release device
	COM_RELEASE(ptrDevice);

	// Fill descritor heaps
	fillDescHeaps();

	// Upload vertex buffer
	uploadVertex(ptrCmdList);
}

void GxRenderIO::LayerStack::Manager::flushAndResize(UINT width, UINT height) {
	// Flush manager
	m_ptrCmdListManger->flush();

	// Resize all layers
	for (UINT i = 0; i < m_uiLayersUsed; i++) {
		m_ptrsLayers[i]->resize(width, height);
	}

	// Update width and height
	m_viewPort.Width = (FLOAT)width;
	m_viewPort.Height = (FLOAT)height;

	// Set rect
	m_sicRect = { 0, 0, (LONG)width, (LONG)height };

	// Update internal width and height
	m_uiWidth = width;
	m_uiHeight = height;

	// Recreate heap
	fillDescHeaps();
}

void GxRenderIO::LayerStack::Manager::execute(GxDirect::XWindow* ptrWindow, GxDirect::XCmdList* ptrWindowList, BOOL vsync){
	m_stopWatch.stop();
	m_frameInfo.deltaTMs = m_stopWatch.getElapsedMs();
	
	// Dispatch layers
	for (UINT i = 0; i < m_uiLayersUsed; i++) {
		// Check if layer is enabled
		if (m_ptrsLayers[i]->getEnabled()) {
			// Start execution
			m_ptrsLayers[i]->dispatchFrame(&m_frameInfo, m_ptrCmdListManger);
		}
	}

	m_stopWatch.reset();
	m_stopWatch.start();

	// Wait for execution
	for (UINT i = 0; i < m_uiLayersUsed; i++) {
		m_ptrsLayers[i]->waitForFrame();
	}

	// Execute (LAST) frame and wait for it
	m_ptrCmdListManger->executeCommandLists();
	m_ptrCmdListManger->waitForCommandLists();

	// Copy Buffer if required
	updateConstBuffer(ptrWindowList);

	// Set states
	ptrWindowList->get()->SetPipelineState(m_ptrPipelineState);
	ptrWindowList->get()->SetGraphicsRootSignature(m_ptrRootSignature);

	// Set CBuffer
	ptrWindowList->get()->SetGraphicsRootConstantBufferView(0, m_ptrConstBuffer->GetGPUVirtualAddress());

	// Set Framebuffers
	ID3D12DescriptorHeap* arr[] = {
		m_ptrDecriptorHeap
	};
	ptrWindowList->get()->SetDescriptorHeaps(1, arr);
	ptrWindowList->get()->SetGraphicsRootDescriptorTable(1, m_ptrDecriptorHeap->GetGPUDescriptorHandleForHeapStart());

	// Begin frame on window
	ptrWindow->beginFrame(ptrWindowList->get());
	
	// Set viewport and rect
	ptrWindowList->get()->RSSetViewports(1, &m_viewPort);
	ptrWindowList->get()->RSSetScissorRects(1, &m_sicRect);

	// Set Input type
	static D3D12_VERTEX_BUFFER_VIEW view = {m_ptrVertexBuffer->GetGPUVirtualAddress(), sizeof(float) * 4 * 4, sizeof(float) * 4 };
	ptrWindowList->get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	ptrWindowList->get()->IASetVertexBuffers(0, 1, &view);

	// Draw
	ptrWindowList->get()->DrawInstanced(4, m_uiLayersUsed, 0, 0);

	// End prame
	ptrWindow->endFrame(ptrWindowList->get());

	// Execute and wait
	ptrWindowList->execute();
	ptrWindowList->wait();

	// Present
	ptrWindow->present(vsync);
}

BOOL GxRenderIO::LayerStack::Manager::setLayerEnabled(UINT index, BOOL enabled){
	// check bounds
	if (index >= m_uiLayersUsed) {
		throw EXEPTION(L"GxRenderIO::LayerStack::Manager::setLayerEnabled(...) Index out of range");
	}
	
	// Call layer
	return m_ptrsLayers[index]->setEnable(enabled);
}

void GxRenderIO::LayerStack::Manager::fillDescHeaps(){
	// For all layers
	for (UINT layerNumber = 0; layerNumber < m_uiLayersUsed; layerNumber++) {
		// Create handle to heap start
		D3D12_CPU_DESCRIPTOR_HANDLE handle = m_ptrDecriptorHeap->GetCPUDescriptorHandleForHeapStart();

		// Increment handle
		handle.ptr += (SIZE_T)m_ptrContext->getIncrmentSrv() * layerNumber;

		// Create view
		m_ptrsLayers[layerNumber]->getResourceViewForBuffer(handle);
	}
}

void GxRenderIO::LayerStack::Manager::uploadVertex(GxDirect::XCmdList* ptrCmdList) {
	HRESULT hr;

	// Heap descritor default
	D3D12_HEAP_PROPERTIES heapPropDesk_default;
	ZeroMemory(&heapPropDesk_default, sizeof(D3D12_HEAP_PROPERTIES));

	heapPropDesk_default.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapPropDesk_default.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapPropDesk_default.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapPropDesk_default.CreationNodeMask = NULL;
	heapPropDesk_default.VisibleNodeMask = NULL;

	// Heap descritor upload
	D3D12_HEAP_PROPERTIES heapPropDesk_upload;
	ZeroMemory(&heapPropDesk_upload, sizeof(D3D12_HEAP_PROPERTIES));

	heapPropDesk_upload.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapPropDesk_upload.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapPropDesk_upload.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapPropDesk_upload.CreationNodeMask = NULL;
	heapPropDesk_upload.VisibleNodeMask = NULL;
	
	// Resource descrition for vertex buffer
	D3D12_RESOURCE_DESC resDesk_vtx;
	ZeroMemory(&resDesk_vtx, sizeof(D3D12_RESOURCE_DESC));

	resDesk_vtx.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesk_vtx.Alignment = 0;
	resDesk_vtx.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resDesk_vtx.Width = sizeof(c_vertexBuffer);
	resDesk_vtx.Height = 1;
	resDesk_vtx.DepthOrArraySize = 1;
	resDesk_vtx.MipLevels = 1;
	resDesk_vtx.SampleDesc.Count = 1;
	resDesk_vtx.SampleDesc.Quality = 0;
	resDesk_vtx.Flags = D3D12_RESOURCE_FLAG_NONE;

	// Get Deive
	ID3D12Device* ptrDevice;
	m_ptrContext->getDevice(&ptrDevice);

	// Create Vertex buffer
	if (FAILED(hr = ptrDevice->CreateCommittedResource(&heapPropDesk_default, D3D12_HEAP_FLAG_NONE, &resDesk_vtx, D3D12_RESOURCE_STATE_COPY_DEST, NULL, IID_PPV_ARGS(&m_ptrVertexBuffer)))) {
		throw EXEPTION_HR(L"ID3D12Deive->CreateCommittedResource(...)", hr);
	}

	// Create Upload buffer
	ID3D12Resource* ptrUpload;
	if (FAILED(hr = ptrDevice->CreateCommittedResource(&heapPropDesk_upload, D3D12_HEAP_FLAG_NONE, &resDesk_vtx, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&ptrUpload)))) {
		throw EXEPTION_HR(L"ID3D12Deive->CreateCommittedResource(...)", hr);
	}

	COM_RELEASE(ptrDevice);

	// Map upload buffer into memroy
	void* ptrUploadData;
	if (FAILED(hr = ptrUpload->Map(0, NULL, &ptrUploadData))) {
		throw EXEPTION_HR(L"ID3D12Resource->Map(...)", hr);
	}

	// Memory copy
	memcpy(ptrUploadData, c_vertexBuffer, sizeof(c_vertexBuffer));

	// Unmap buffer
	ptrUpload->Unmap(0, NULL);

	// Copy 
	ptrCmdList->get()->CopyResource(m_ptrVertexBuffer, ptrUpload);

	// Barrier
	D3D12_RESOURCE_BARRIER barrToCpy;
	ZeroMemory(&barrToCpy, sizeof(D3D12_RESOURCE_BARRIER));
	
	// Vertex is read
	barrToCpy.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrToCpy.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrToCpy.Transition.pResource = m_ptrVertexBuffer;
	barrToCpy.Transition.Subresource = 0;
	barrToCpy.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrToCpy.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	// Que barrier
	ptrCmdList->get()->ResourceBarrier(1, &barrToCpy);

	// Execute
	ptrCmdList->execute();
	ptrCmdList->wait();

	// Delete upload buffer
	COM_RELEASE(ptrUpload);

	// Set name for buffer
	#if defined(_DEBUG)
	m_ptrVertexBuffer->SetName(L"LayerStack Vertex Buffer");
	#endif
}

void GxRenderIO::LayerStack::Manager::updateConstBuffer(GxDirect::XCmdList* ptrCmdList) {
	HRESULT hr;

	// Buil bit mask
	UINT mask = 0x0;
	for (UINT i = 0; i < m_uiLayersUsed; i++) {
		if (m_ptrsLayers[i]->getEnabled()) {
			mask |= (1UL << i);
		}
	}

	// check if changed
	MetaCBuffer cb = m_cbLastFrame;
	cb.layersEnabledMask = mask;
	if (memcmp(&cb, &m_cbLastFrame, sizeof(MetaCBuffer)) != 0) {
		// Update internal
		m_cbLastFrame = cb;

		// Map upload to memory
		void* ptrUploadData = NULL;
		if (FAILED(hr = m_ptrUploadBuffer->Map(0, NULL, &ptrUploadData))) {
			throw EXEPTION_HR(L"ID3D12Resource->Map(...)", hr);
		}

		// Copy to upload buffer
		memcpy(ptrUploadData, &m_cbLastFrame, sizeof(MetaCBuffer));

		// Unmap buffer
		m_ptrUploadBuffer->Unmap(0, NULL);
		
		// Barrier to copy dest
		D3D12_RESOURCE_BARRIER barrToCpy;
		ZeroMemory(&barrToCpy, sizeof(D3D12_RESOURCE_BARRIER));

		barrToCpy.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrToCpy.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrToCpy.Transition.pResource = m_ptrConstBuffer;
		barrToCpy.Transition.Subresource = 0;
		barrToCpy.Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
		barrToCpy.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;

		// Que barrier
		ptrCmdList->get()->ResourceBarrier(1, &barrToCpy);

		// Que Copy buffer
		ptrCmdList->get()->CopyResource(m_ptrConstBuffer, m_ptrUploadBuffer);

		// Change barrier
		barrToCpy.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		barrToCpy.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

		// Que change barrier
		ptrCmdList->get()->ResourceBarrier(1, &barrToCpy);
	}
}
