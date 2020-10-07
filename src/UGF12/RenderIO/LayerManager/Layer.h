#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>
#include <UGF12/Util/StrConverter.h>
#include <UGF12/Util/Concurent/WorkerPayload.h>

#include <UGF12/RenderIO/LayerManager/ILayerImpl.h>
#include <UGF12/RenderIO/Framebuffer.h>
#include <UGF12/RenderIO/Executing/CmdListProxy.h>

namespace GxRenderIO {
	namespace LayerStack {
		/// <summary>
		/// Represents a layer
		/// </summary>
		class Layer {
			public:
				/// <summary>
				/// Create layer manger
				/// </summary>
				/// <param name="ptrContext">Pointer to context</param>
				/// <param name="width">Initial width</param>
				/// <param name="height">Initial height</param>
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
				/// <param name="frameInfo">Information about current frame</param>
				/// <param name="ptrCmdManager">Pointer to manager to be used</param>
				/// <param name="bufferIndex">Index of the resource to use (0/1)</param>
				void dispatchFrame(LayerFrameInfo* frameInfo, GxRenderIO::CmdListManger* ptrCmdManager, UINT bufferIndex);

				/// <summary>
				/// Wait for the worker threads execution (Does block)
				/// </summary>
				void waitForFrame();

			private:
				/// <summary>
				/// Struckt for the worker
				/// </summary>
				struct LayerWorkerPayload {
					LayerFrameInfo frameInfo;
					GxRenderIO::CmdListManger* ptrCmdManager;
					GxRenderIO::FrameBuffer* ptrFrameBuffer;
				};

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
				/// Encapsulation for exeption (if start work == true -> exeption encountered)
				/// </summary>
				GxUtil::WorkerPayload<GxExeption> m_pyThreadExeption;

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
				GxUtil::WorkerPayload<LayerWorkerPayload> m_workerPayload;

				/// <summary>
				/// Double buffering framebuffers (RTV)
				/// </summary>
				GxRenderIO::FrameBuffer m_framebuffers[2];
		};
	}
}