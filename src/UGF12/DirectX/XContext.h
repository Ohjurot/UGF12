#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>

namespace GxDirect {
	class XContext {
		public:
			/// <summary>
			/// Create context on prefered gpu
			/// </summary>
			/// <param name="strPreferedGpu">Name of the preverd gpu (fallback if index and name dont match)</param>
			/// <param name="idxPreveredGpu">Primery filter (if idx and name don't match) can be -1</param>
			/// <returns></returns>
			XContext(std::wstring strPreferedGpu, INT idxPreveredGpu, BOOL debugLayer);

			// Destructor to free resources
			~XContext();

			/// <summary>
			/// Set a pointer to the context device (dont forget to call relase on your new pointer when no longer required)
			/// </summary>
			/// <param name="ppDevice">Pointer to the new device pointer</param>
			void getDevice(ID3D12Device** ppDevice);

			/// <summary>
			/// Get a pointer to the current gpu
			/// </summary>
			/// <returns>Pointer to gpu info struct</returns>
			DXGI_ADAPTER_DESC getGpuInfo();

			// Delete unuse
			void operator=(const XContext&) = delete;
			XContext(const XContext&) = delete;
		private:
			/// <summary>
			/// Default D3D12 Device
			/// </summary>
			ID3D12Device* m_ptrDevice = NULL;

			/// <summary>
			/// Debug device create if debug layer is enabled
			/// </summary>
			ID3D12DebugDevice* m_ptrDebugDevice = NULL;

			/// <summary>
			/// Debug for enabling debug layer
			/// </summary>
			ID3D12Debug* m_ptrDebug = NULL;

			/// <summary>
			/// Info about the running gpu
			/// </summary>
			DXGI_ADAPTER_DESC m_adapterInfo;
	};
}