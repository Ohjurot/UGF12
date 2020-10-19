#pragma once
#include "pch.h"

#include <UGF12/DirectX/XContext.h>

#include <UGF12/DirectX/FeatureSupport/FS_Options0.h>
#include <UGF12/DirectX/FeatureSupport/FS_Options1.h>
#include <UGF12/DirectX/FeatureSupport/FS_Options2.h>
#include <UGF12/DirectX/FeatureSupport/FS_Options3.h>
#include <UGF12/DirectX/FeatureSupport/FS_Options4.h>
#include <UGF12/DirectX/FeatureSupport/FS_Options5.h>
#include <UGF12/DirectX/FeatureSupport/FS_Options6.h>
#include <UGF12/DirectX/FeatureSupport/FS_Options7.h>

#include <UGF12/DirectX/FeatureSupport/FS_Architecture1.h>
#include <UGF12/DirectX/FeatureSupport/FS_VirtualAddress.h>
#include <UGF12/DirectX/FeatureSupport/FS_CrossNode.h>



namespace GxDirect {
	class XFeatureSupport {
		public:
			/// <summary>
			/// Create feature support from contewxt
			/// </summary>
			/// <param name="ptrContext"></param>
			XFeatureSupport(GxDirect::XContext* ptrContext);

			// Destruct
			~XFeatureSupport();

			/// <summary>
			/// Check MSAA qualit level for formate and smaple count
			/// </summary>
			/// <param name="formate">Formate to check MSAA support on</param>
			/// <param name="sampleCount">Samples count</param>
			/// <returns>Sample quality</returns>
			UINT checkMsaaSupport(DXGI_FORMAT formate, UINT sampleCount);

			/// <summary>
			/// Check if a format is supported
			/// </summary>
			/// <param name="format">Input format</param>
			/// <param name="ptrOutputFormat1">Pointer to FORMAT_SUPPORT1</param>
			/// <param name="ptrOutputFormat2">Pointer to FORMAT_SUPPORT2</param>
			/// <returns>If query return successfully</returns>
			BOOL checkFormatSupport(DXGI_FORMAT format, D3D12_FORMAT_SUPPORT1* ptrOutputFormat1, D3D12_FORMAT_SUPPORT2* ptrOutputFormat2);

			/// <summary>
			/// Check if the device supports an specific command que priprity
			/// </summary>
			/// <param name="listType">Type of command list to que priority on</param>
			/// <param name="priority">Requested priority</param>
			/// <returns>If priprity is supported</returns>
			BOOL checkCommandQueuePriority(D3D12_COMMAND_LIST_TYPE listType, D3D12_COMMAND_QUEUE_PRIORITY priority);

			/// <summary>
			/// Query heap serialisation tier for a node index
			/// </summary>
			/// <param name="nodeIndex">Index of the node</param>
			/// <returns>Heap serialisation tier</returns>
			D3D12_HEAP_SERIALIZATION_TIER getHeapSerializationTierForNode(UINT nodeIndex);

			/// <summary>
			/// Get highst shader model availible
			/// </summary>
			/// <returns></returns>
			const TSysInfo(D3D_SHADER_MODEL)& getShaderModel();

			/// <summary>
			/// Get the root signature version supported
			/// </summary>
			/// <returns></returns>
			const TSysInfo(D3D_ROOT_SIGNATURE_VERSION)& getRootSignatureVersion();

			/// <summary>
			/// Get the shader cache support flags
			/// </summary>
			/// <returns></returns>
			const TSysInfo(D3D12_SHADER_CACHE_SUPPORT_FLAGS)& getShaderCacheSupportFlags();

			/// <summary>
			/// Indicates that the an adapter can create an heap from existing system memory
			/// </summary>
			/// <returns></returns>
			const TSysInfo(BOOL)& getExisitingHeapSupport();

			/// <summary>
			/// Retrive Options0
			/// </summary>
			/// <returns>const ref</returns>
			GxDirect::FeaturSupport::FS_Options0& getOptions0();

			/// <summary>
			/// Retrive Options1
			/// </summary>
			/// <returns>const ref</returns>
			GxDirect::FeaturSupport::FS_Options1& getOptions1();

			/// <summary>
			/// Retrive Options2
			/// </summary>
			/// <returns>const ref</returns>
			GxDirect::FeaturSupport::FS_Options2& getOptions2();
			
			/// <summary>
			/// Retrive Options3
			/// </summary>
			/// <returns>const ref</returns>
			GxDirect::FeaturSupport::FS_Options3& getOptions3();

			/// <summary>
			/// Retrive Options4
			/// </summary>
			/// <returns>const ref</returns>
			GxDirect::FeaturSupport::FS_Options4& getOptions4();

			/// <summary>
			/// Retrive Options2
			/// </summary>
			/// <returns>const ref</returns>
			GxDirect::FeaturSupport::FS_Options5& getOptions5();

			/// <summary>
			/// Retrive Options3
			/// </summary>
			/// <returns>const ref</returns>
			GxDirect::FeaturSupport::FS_Options6& getOptions6();

			/// <summary>
			/// Retrive Options7
			/// </summary>
			/// <returns>const ref</returns>
			GxDirect::FeaturSupport::FS_Options7& getOptions7();

			/// <summary>
			/// Get architecture 1
			/// </summary>
			/// <returns>const ref</returns>
			GxDirect::FeaturSupport::FS_Architecture1& getArchitecture1();

			/// <summary>
			/// Get VirtualAddress object
			/// </summary>
			/// <returns>const ref</returns>
			GxDirect::FeaturSupport::FS_VirtualAddress& getVirtualAddress();

			/// <summary>
			/// Get CrossNode
			/// </summary>
			/// <returns>const ref</returns>
			GxDirect::FeaturSupport::FS_CrossNode& getCrossNode();

		private:
			/// <summary>
			/// Device reference
			/// </summary>
			ID3D12Device* m_ptrDevice;

			/// <summary>
			/// Highst shader modell availible
			/// </summary>
			TSysInfo(D3D_SHADER_MODEL) m_siHighstShaderModell = D3D_SHADER_MODEL_5_1;

			/// <summary>
			/// Highst root signature version supported
			/// </summary>
			TSysInfo(D3D_ROOT_SIGNATURE_VERSION) m_siRootSignatureVersion = D3D_ROOT_SIGNATURE_VERSION_1;

			/// <summary>
			/// Shader cache flags
			/// </summary>
			TSysInfo(D3D12_SHADER_CACHE_SUPPORT_FLAGS) m_siShaderCache = D3D12_SHADER_CACHE_SUPPORT_NONE;

			/// <summary>
			/// Indicates that the an adapter can create an heap from existing system memory
			/// </summary>
			TSysInfo(BOOL) m_siExistingHeapSupport = FALSE;
			
			/// <summary>
			/// D3D12_FEATURE_DATA_D3D12_OPTIONS
			/// </summary>
			GxDirect::FeaturSupport::FS_Options0 m_options0;

			/// <summary>
			/// D3D12_FEATURE_DATA_D3D12_OPTIONS1
			/// </summary>
			GxDirect::FeaturSupport::FS_Options1 m_options1;
			
			/// <summary>
			/// D3D12_FEATURE_DATA_D3D12_OPTIONS2
			/// </summary>
			GxDirect::FeaturSupport::FS_Options2 m_options2;

			/// <summary>
			/// D3D12_FEATURE_DATA_D3D12_OPTIONS3
			/// </summary>
			GxDirect::FeaturSupport::FS_Options3 m_options3;

			/// <summary>
			/// D3D12_FEATURE_DATA_D3D12_OPTIONS4
			/// </summary>
			GxDirect::FeaturSupport::FS_Options4 m_options4;

			/// <summary>
			/// D3D12_FEATURE_DATA_D3D12_OPTIONS5
			/// </summary>
			GxDirect::FeaturSupport::FS_Options5 m_options5;

			/// <summary>
			/// D3D12_FEATURE_DATA_D3D12_OPTIONS6
			/// </summary>
			GxDirect::FeaturSupport::FS_Options6 m_options6;

			/// <summary>
			/// D3D12_FEATURE_DATA_D3D12_OPTIONS7
			/// </summary>
			GxDirect::FeaturSupport::FS_Options7 m_options7;

			/// <summary>
			/// D3D12_FEATURE_DATA_ARCHITECTURE1 
			/// </summary>
			GxDirect::FeaturSupport::FS_Architecture1 m_architecture1;

			/// <summary>
			/// D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT
			/// </summary>
			GxDirect::FeaturSupport::FS_VirtualAddress m_virtualAddress;

			/// <summary>
			/// D3D12_FEATURE_DATA_CROSS_NODE 
			/// </summary>
			GxDirect::FeaturSupport::FS_CrossNode m_crossNode;
	};
}
