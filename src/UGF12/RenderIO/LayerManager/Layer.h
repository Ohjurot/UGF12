#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>
#include <UGF12/Util/Concurent/WorkerPayload.h>

#include <UGF12/RenderIO/LayerManager/ILayerImpl.h>
#include <UGF12/RenderIO/Framebuffer.h>
#include <UGF12/RenderIO/Executing/CmdListManager.h>

namespace GxRenderIO {
	namespace LayerStack {

		/// <summary>
		/// Struck for storing the frame info
		/// </summary>
		struct LayerFrameInfo {
			/// <summary>
			/// Index of the current frame
			/// </summary>
			UINT64 frameIndex = 0;

			/// <summary>
			/// Start time of the Frame
			/// </summary>
			INT64 frameStartTime = 0;

			/// <summary>
			/// Delta time since last frame
			/// </summary>
			FLOAT deltaTMs = 0.0;
		};

		/// <summary>
		/// Represents a layer
		/// </summary>
		class Layer {
			public:
				/// <summary>
				/// Create layer manger
				/// </summary>
				/// <param name="ptrImpl">Pointer to layer implementation</param>
				/// <param name="affinityMask">Thread affinity mask</param>
				Layer(GxDirect::XContext* ptrContext, UINT width, UINT height, GxRenderIO::LayerStack::ILayerImpl* ptrImpl, DWORD affinityMask = 0xFFFF);

				// Destructor 
				~Layer();

				/// <summary>
				/// Create Shader resource view for a buffer
				/// </summary>
				/// <param name="srvHandle">Shader resource view handle</param>
				/// <param name="bufferIndex">Index of the buffer to be set the handle to</param>
				/// <returns>If action completed successfully</returns>
				BOOL getResourceViewForBuffer(D3D12_CPU_DESCRIPTOR_HANDLE srvHandle, UINT bufferIndex);

				/// <summary>
				/// Dispatch a frame for the worker thread (Blocks only when called multiple times without a call to waitForFrame(...) )
				/// </summary>
				void dispatchFrame();

				/// <summary>
				/// Wait for the worker threads execution (Does block)
				/// </summary>
				void waitForFrame();

			private:
				/// <summary>
				/// Thread proc
				/// </summary>
				/// <param name="threadParam">Pointer to the active Layer</param>
				/// <returns>Thread return code</returns>
				static DWORD WINAPI ThreadProc(LPVOID threadParam);

				/// <summary>
				/// Internal thread proc
				/// </summary>
				/// <returns>Return code</returns>
				DWORD _internalThreadProc();
			private:
				/// <summary>
				/// Pointer to thread implementation
				/// </summary>
				GxRenderIO::LayerStack::ILayerImpl* m_ptrImpl;

				/// <summary>
				/// Handle to the thread
				/// </summary>
				HANDLE m_hThread = NULL;

				/// <summary>
				/// Id of the thread
				/// </summary>
				DWORD m_dwThreadId = 0;

				/// <summary>
				/// Flag that indicates if the thread should exit
				/// </summary>
				BOOL m_bThreadShouldExit = FALSE;

				/// <summary>
				/// Payload of the thread worker
				/// </summary>
				GxUtil::WorkerPayload<LayerFrameInfo> m_workerPayload;

				/// <summary>
				/// Double buffering framebuffers (RTV)
				/// </summary>
				GxRenderIO::FrameBuffer m_framebuffers[2];
		};
	}
}