#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>
#include <UGF12/Util/SystemMetrics.h>
#include <UGF12/Util/Time/StopWatch.h>
#include <UGF12/Util/Time/HPC.h>
#include <UGF12/Util/Concurent/ExecutionCounter.h>

#include <UGF12/DirectX/XCmdQueue.h>
#include <UGF12/DirectX/XCmdList.h>
#include <UGF12/DirectX/XWindow.h>

#include <UGF12/RenderIO/LayerManager/ILayerImpl.h>
#include <UGF12/RenderIO/LayerManager/Layer.h>
#include <UGF12/RenderIO/LayerManager/LayerStackShader.h>

namespace GxRenderIO {
	namespace LayerStack {
		/// <summary>
		/// Layer Stack manager
		/// </summary>
		class Manager {
			private:
				/// <summary>
				/// Const Buffer for drawing info
				/// </summary>
				struct MetaCBuffer {
					/// <summary>
					/// Layers enabled mask
					/// </summary>
					UINT layersEnabledMask = 0x00;
				};

				/// <summary>
				/// Constnt vertext buffer IA-Type: D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
				/// </summary>
				const FLOAT c_vertexBuffer[16] = {
					-1.0f,	-1.0f,		0.0f, 1.0f,
					-1.0f,	1.0f,		0.0f, 0.0f,
					1.0f,	-1.0f,		1.0f, 1.0f,
					1.0f,	1.0f,		1.0f, 0.0f,
				};

				const D3D12_INPUT_ELEMENT_DESC c_inputLayout[2] = {
					{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,					D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
					{"TEXTCORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 2,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
				};

			public:
				/// <summary>
				/// Create layerstack from context
				/// </summary>
				/// <param name="ptrContext">Pointer to context</param>
				/// <param name="cmdListCount">Number of command lists</param>
				/// <param name="width">Width of the window</param>
				/// <param name="height">Height of the window</param>
				Manager(GxDirect::XContext* ptrContext, UINT cmdListCount, UINT width, UINT height);

				// Destructor
				~Manager();

				/// <summary>
				/// Create layer for implemenatation
				/// </summary>
				/// <param name="ptrImpl"></param>
				void insertLayer(GxRenderIO::LayerStack::ILayerImpl* ptrImpl);

				/// <summary>
				/// Should be called bevore the main loop
				/// </summary>
				/// <param name="ptrCmdList">Pointe to command list</param>
				void init(GxDirect::XCmdList* ptrCmdList);

				/// <summary>
				/// Flush and resize layer stack
				/// </summary>
				/// <param name="width">Width of the window</param>
				/// <param name="height">Height of the window</param>
				void flushAndResize(UINT width, UINT height);

				/// <summary>
				/// Draw last processed frame on a window
				/// </summary>
				/// <param name="ptrWindow">Pointe to window</param>
				/// <param name="ptrWindowList">Pointer to window list</param>
				/// <param name="vsync">If vsync should be enabled</param>
				void execute(GxDirect::XWindow* ptrWindow, GxDirect::XCmdList* ptrWindowList, BOOL vsync = FALSE);

				/// <summary>
				/// Set the enable state of a layer
				/// </summary>
				/// <param name="index">Index of layer</param>
				/// <param name="enabled">If the layer should be enabled</param>
				/// <returns>If layer could be en/disabled</returns>
				BOOL setLayerEnabled(UINT index, BOOL enabled);

				// Delee unsupported
				Manager(const Manager&) = delete;
				void operator==(const Manager&) = delete;
			private:
				/// <summary>
				/// Fills the descriptor heaps
				/// </summary>
				void fillDescHeaps();

				/// <summary>
				/// Upload vertex data
				/// </summary>
				/// <param name="ptrCmdList">Pointe to command list</param>
				void uploadVertex(GxDirect::XCmdList* ptrCmdList);

				/// <summary>
				/// Updates the const buffer if required
				/// </summary>
				/// <param name="ptrCmdList">Pointe to command list</param>
				void updateConstBuffer(GxDirect::XCmdList* ptrCmdList);
			private:
				/// <summary>
				/// Reference to context
				/// </summary>
				GxDirect::XContext* m_ptrContext;

				/// <summary>
				/// Current width
				/// </summary>
				UINT m_uiWidth;

				/// <summary>
				/// Current height
				/// </summary>
				UINT m_uiHeight;

				/// <summary>
				/// Stop Watch for timing 
				/// </summary>
				GxUtil::StopWatch m_stopWatch;

				/// <summary>
				/// Command Que
				/// </summary>
				GxDirect::XCmdQueue* m_ptrCmdQues;

				/// <summary>
				/// Command list manager
				/// </summary>
				GxRenderIO::CmdListManger* m_ptrCmdListManger;

				/// <summary>
				/// Array of layers (16 Max)
				/// </summary>
				GxRenderIO::LayerStack::Layer* m_ptrsLayers[16] = {};
				
				/// <summary>
				/// Meta buffer of the last frame
				/// </summary>
				MetaCBuffer m_cbLastFrame = {0xFFFFFFFF};

				/// <summary>
				/// Number of layers used
				/// </summary>
				UINT m_uiLayersUsed = 0;
				
				/// <summary>
				/// Current frame info
				/// </summary>
				GxRenderIO::LayerStack::LayerFrameInfo m_frameInfo;

			private:
				/// <summary>
				/// Descritor heap
				/// </summary>
				ID3D12DescriptorHeap* m_ptrDecriptorHeap = NULL;

				/// <summary>
				/// GPU Resource for constant buffer
				/// </summary>
				ID3D12Resource* m_ptrConstBuffer = NULL;

				/// <summary>
				/// Upload buffer for constant buffer
				/// </summary>
				ID3D12Resource* m_ptrUploadBuffer = NULL;

				/// <summary>
				/// Vertex Buffer for GPU
				/// </summary>
				ID3D12Resource* m_ptrVertexBuffer = NULL;

				/// <summary>
				/// Pipline state for layerstack manger draws
				/// </summary>
				ID3D12PipelineState* m_ptrPipelineState = NULL;

				/// <summary>
				/// Root signature of shaders
				/// </summary>
				ID3D12RootSignature* m_ptrRootSignature = NULL;

				/// <summary>
				/// D3D Viewport
				/// </summary>
				D3D12_VIEWPORT m_viewPort;

				/// <summary>
				/// Siscor recange
				/// </summary>
				RECT m_sicRect;
		};
	}
}