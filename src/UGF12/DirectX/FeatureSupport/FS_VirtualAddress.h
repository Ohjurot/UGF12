#pragma once
#include "pch.h"

#include <UGF12/Util/TSysInfo.h>
#include <UGF12/Util/Exeption.h>

#include <UGF12/DirectX/XContext.h>

namespace GxDirect {
	namespace FeaturSupport {
		/// <summary>
		/// Container for D3D12_FEATURE_GPU_VIRTUAL_ADDRESS_SUPPORT
		/// </summary>
		class FS_VirtualAddress {
			public:
				/// <summary>
				/// Create options based on a context
				/// </summary>
				/// <param name="ptrContext">context to query</param>
				FS_VirtualAddress(GxDirect::XContext* ptrContext);

				/// <summary>
				/// Retrive the result of the query
				/// </summary>
				/// <returns>Result of query</returns>
				HRESULT getQueryResult();

				/// <summary>
				/// Gennerates the text representation for feature support (Will fail on Shipping configuration!)
				/// </summary>
				void gennerateTextRepresentation();

			public:
				/// <summary>
				/// https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_feature_data_gpu_virtual_address_support
				/// </summary>
				/// <returns></returns>
				const TSysInfo(UINT)& getMaxGPUVirtualAddressBitsPerResource();

				/// <summary>
				/// https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_feature_data_gpu_virtual_address_support
				/// </summary>
				/// <returns></returns>
				const TSysInfo(UINT)& getMaxGPUVirtualAddressBitsPerProcess();

			private:
				/// <summary>
				/// Query result
				/// </summary>
				HRESULT m_hrQueryResult;

				/// <summary>
				/// https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_feature_data_gpu_virtual_address_support
				/// </summary>
				TSysInfo(UINT) m_siMaxGPUVirtualAddressBitsPerResource = 0;

				/// <summary>
				/// https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_feature_data_gpu_virtual_address_support
				/// </summary>
				TSysInfo(UINT) m_siMaxGPUVirtualAddressBitsPerProcess = 0;
		};
	}
}