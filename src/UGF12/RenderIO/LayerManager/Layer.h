#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>
#include <UGF12/RenderIO/LayerManager/ILayerImpl.h>

namespace GxRenderIO {
	namespace LayerStack {

		/// <summary>
		/// Represents a layer
		/// </summary>
		class Layer {
			public:
				Layer(GxRenderIO::LayerStack::ILayerImpl* ptrImpl, DWORD affinityMask = 0xFFFF);

				// Destructor 
				~Layer();

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
		};
	}
}