#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>
#include <UGF12/DirectX/XContext.h>
#include <UGF12/DirectX/XCmdQueue.h>

namespace GxDirect {
	class XWndDriver {
		public:
			/// <summary>
			/// Create swap chain for window
			/// </summary>
			/// <param name="targetHwnd">Window to create swap chain on</param>
			/// <param name="ptrContext">Context on witch swap chain should be created</param>
			XWndDriver(HWND targetHwnd, GxDirect::XContext* ptrContext, GxDirect::XCmdQueue* ptrCmdQue);

			// Destructor to free unused
			~XWndDriver();

			/// <summary>
			/// Begin a frame on this swap chain
			/// </summary>
			/// <param name="ptrCmdList">Command list used to draw on swap chain</param>
			void beginFrame(ID3D12GraphicsCommandList* ptrCmdList);

			/// <summary>
			/// End a frame on this swap chain
			/// </summary>
			/// <param name="ptrCmdList">Command list used to draw on swap chain</param>
			void endFrame(ID3D12GraphicsCommandList* ptrCmdList);

			/// <summary>
			/// Present the frame to the user
			/// </summary>
			/// <param name="vsync">If vsync should be enabled</param>
			UINT present(BOOL vsync = FALSE);

			/// <summary>
			/// Resizes the swap chain to fit target window
			/// </summary>
			/// <param name="targetWindow">Input target window</param>
			void resizeSwapChain(UINT width, UINT height);

			/// <summary>
			/// Set the fullscreen mode of the swap chain
			/// </summary>
			/// <param name="fullscreen">If fullscreen should be activated</param>
			void setFullscreen(BOOL fullscreen);

			/// <summary>
			/// Retrives if the driver is currently operating in fullscreen mode
			/// </summary>
			/// <returns>If in fullscreen</returns>
			BOOL isFullscreen();

			// Delete unuse
			void operator=(const XWndDriver&) = delete;
			XWndDriver(const XWndDriver&) = delete;
		private:
			/// <summary>
			/// Internal function to set pointers to buffers
			/// </summary>
			void getSwapchainBuffers();

			/// <summary>
			/// Internal reference to device
			/// </summary>
			ID3D12Device* m_ptrDevice = NULL;

			/// <summary>
			/// Internal referecne to que
			/// </summary>
			ID3D12CommandQueue* m_ptrCmdQue = NULL;

			/// <summary>
			/// Swap chain driving the windows content
			/// </summary>
			IDXGISwapChain* m_ptrSwapChain = NULL;

			/// <summary>
			/// Heap for storing render target views
			/// </summary>
			ID3D12DescriptorHeap* m_ptrRtvHeap = NULL;
			
			/// <summary>
			/// Size to increment the heap
			/// </summary>
			UINT m_uiRtvIncrementSize = 0;

			/// <summary>
			/// Tripple buffering back buffers
			/// </summary>
			ID3D12Resource* m_ptrsBackBuffers[3] = { NULL, NULL, NULL };

			/// <summary>
			/// Index to keep track of current back buffer
			/// </summary>
			UINT m_uiCurrentBackBuffer = 0;
	};
}