#pragma once
#include "pch.h"

#include <UGF12/DirectX/XContext.h>
#include <UGF12/DirectX/XFeature.h>

namespace GxRenderIO {
	/// <summary>
	/// Enum to indicate framebuffer usage
	/// </summary>
	enum class FrameBufferUsage : UINT {
		BUFFER_TEXTURE = 0,
		BUFFER_TEXTURE_RENDER_TARGET = 1,
		BUFFER_TEXTURE_DEPTH = 2,
	};

	/// <summary>
	/// Struck to store color / depth cleat value
	/// </summary>
	struct ClearValue {
		union {
			FLOAT						color[4];
			D3D12_DEPTH_STENCIL_VALUE	depthValue;
		};
	};

	/// <summary>
	/// Class for storing a framebuffer
	/// </summary>
	class FrameBuffer {
		public:
			/// <summary>
			/// Creates a framebufer on the gpu
			/// </summary>
			/// <param name="ptrContext">Pointer to context</param>
			/// <param name="width">Width of the framebuffer</param>
			/// <param name="height">Height of the framebuffer</param>
			/// <param name="formate">Formate of the framebuffer</param>
			/// <param name="usage">Usage of the buffer</param>
			/// <param name="msaaCount">MSAA Count</param>
			/// <param name="maaaQuality">MSAA Quality (if set to -1 MSAA Quality will be resolved automaticly)</param>
			FrameBuffer(GxDirect::XContext* ptrContext, UINT width, UINT height, DXGI_FORMAT formate = DXGI_FORMAT_R8G8B8A8_UNORM, GxRenderIO::FrameBufferUsage usage = FrameBufferUsage::BUFFER_TEXTURE, ClearValue clearValue = {0.0f, 0.0f, 0.0f, 0.0f}, UINT msaaCount = 1, UINT msaaQuality = 0);

			// Destructor to free resources
			~FrameBuffer();

			/// <summary>
			/// Resizes the buffer to given bounds (recreate)
			/// </summary>
			/// <param name="width">New width</param>
			/// <param name="height">New height</param>
			void resize(UINT width, UINT height);

			/// <summary>
			/// Create render target view to this resource
			/// </summary>
			/// <param name="heapHandleDsv">Handle to apropriate heap</param>
			/// <returns>If RTV was created (If FALSE the framebuffer is probaly no render target)</returns>
			BOOL createRTV(D3D12_CPU_DESCRIPTOR_HANDLE heapHandleRtv);

			/// <summary>
			/// Create a depth stenicl view to this resource
			/// </summary>
			/// <param name="heapHandleDsv">Handle to apropriate heap</param>
			/// <param name="dsvFlags">Optional flags witch indicate that the depth and or the stencil is read only (Non by default)</param>
			/// <returns>If DSV was created (If FALSE the framebuffer is probaly no depth framebuffer)</returns>
			BOOL createDSV(D3D12_CPU_DESCRIPTOR_HANDLE heapHandleDsv, D3D12_DSV_FLAGS dsvFlags = D3D12_DSV_FLAG_NONE);

			/// <summary>
			/// Create a shader resource view to this resource (Texture2D)
			/// </summary>
			/// <param name="heapHandleSrv">Handle to apropriate heap</param>
			void createSRV(D3D12_CPU_DESCRIPTOR_HANDLE heapHandleSrv);

			/// <summary>
			/// Get the state of this resource (After all barrier have execute; THIS FUNCTION IS TO BE USED WITH CARE SINZE IT DOES NOT REPRESET THE REAL STATE OF THE RESOURCE)
			/// </summary>
			/// <returns>Curren / Next latest resource state</returns>
			D3D12_RESOURCE_STATES getState();

			/// <summary>
			/// Retrive the resource pointer for external operation with the resource
			/// </summary>
			/// <returns>Pointer to this resource</returns>
			ID3D12Resource* getResourcePointer();

			/// <summary>
			/// Get the GPU Adress of the framebuffer
			/// </summary>
			/// <returns>Handle to the buffer for the GPU</returns>
			D3D12_GPU_VIRTUAL_ADDRESS getGpuAddress();

			/// <summary>
			/// Initiate a resource barrier from current latest state to a new state on command list
			/// </summary>
			/// <param name="newState">The desired resource state</param>
			/// <param name="ptrCmdList">Command list to create barrier on</param>
			void barrier(D3D12_RESOURCE_STATES newState, ID3D12GraphicsCommandList* ptrCmdList);

			/// <summary>
			/// Set the name of the Framebuffer
			/// </summary>
			/// <param name="name">Name of buffer</param>
			void setName(LPCWSTR name);

			/// <summary>
			/// Retrive the buffers clear value
			/// </summary>
			/// <returns>Clear Value</returns>
			GxRenderIO::ClearValue getClearValue();

			// Delete unused
			FrameBuffer(const FrameBuffer&) = delete;
			void operator=(const FrameBuffer&) = delete;
		private:
			/// <summary>
			/// Internal function for recreation of the buffer
			/// </summary>
			void createBuffer();

		private:
			/// <summary>
			/// Internal pointer to device
			/// </summary>
			ID3D12Device* m_ptrDevice = NULL;

			/// <summary>
			/// Actual framebuffer object
			/// </summary>
			ID3D12Resource* m_ptrBuffer = NULL;

			/// <summary>
			/// State of the resource on the gpu
			/// </summary>
			D3D12_RESOURCE_STATES m_resState = D3D12_RESOURCE_STATE_COMMON;
			
			/// <summary>
			/// Height of this buffer
			/// </summary>
			UINT m_uiWidth;

			/// <summary>
			/// Width of this buffer
			/// </summary>
			UINT m_uiHeight;

			/// <summary>
			/// Formate of the buffer
			/// </summary>
			DXGI_FORMAT m_formate;

			/// <summary>
			/// Clear value for buffer
			/// </summary>
			ClearValue m_clearValue;

			/// <summary>
			/// Usage of the buffer
			/// </summary>
			GxRenderIO::FrameBufferUsage m_usage = FrameBufferUsage::BUFFER_TEXTURE;

			/// <summary>
			/// MSAA sample count 
			/// </summary>
			UINT m_uiMsaaCount;

			/// <summary>
			/// MSAA Quality
			/// </summary>
			UINT m_uiMsaaQuality;

			#ifdef _DEBUG

			/// <summary>
			/// Name of the buffer
			/// </summary>
			std::wstring m_strName = L"Unnamed";

			#endif
	};
}