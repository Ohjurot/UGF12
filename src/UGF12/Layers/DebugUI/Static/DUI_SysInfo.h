#pragma once
#include "pch.h"

#include <UGF12/Util/StrConverter.h>
#include <UGF12/Util/Time/StopWatch.h>
#include <UGF12/Util/SystemMetrics.h>
#include <UGF12/Util/CPUID.h>
#include <UGF12/Util/IWbem/IWbemProvider.h>

#include <UGF12/DirectX/XContext.h>
#include <UGF12/DirectX/XFeatureSupport.h>

#include <UGF12/Layers/DebugUI/IGuiDrawable.h>

#include <imgui.h>

#define GPU_REG_KEY		L"SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000"
#define GPU_REG_NAME	L"HardwareInformation.qwMemorySize"
#define GPU_REG_TYPE	RRF_RT_REG_QWORD

#define	SSTREAM_OUT(condition, stream, text) if(condition) {stream << text;}

namespace UGF12 {
	namespace DebugUI {
		/// <summary>
		/// System Info
		/// </summary>
		class UI_SysInfo : public UGF12::DebugUI::IGuiDrawable {
			public:
				/// <summary>
				/// Create systeminfo
				/// </summary>
				/// <param name="ptrContext"></param>
				UI_SysInfo(GxDirect::XContext* ptrContext);

				// Destruct
				~UI_SysInfo();

				/// <summary>
				/// Draw GUI part
				/// </summary>
				/// <param name="ptrFrameInfo">Frame info</param>
				/// <param name="ptrImGuiIo">ImGuiIO</param>
				void draw(GxRenderIO::LayerStack::LayerFrameInfo* ptrFrameInfo, ImGuiIO* ptrImGuiIo);

				/// <summary>
				/// Set if the UI Element should be enabled
				/// </summary>
				/// <param name="enabled">Enable flag</param>
				void setEnable(BOOL enabled);

				/// <summary>
				/// Retrive the enable state
				/// </summary>
				/// <returns>If enabled</returns>
				BOOL getEnable();

				// Delete unsupported
				UI_SysInfo(const UI_SysInfo&) = delete;
				void operator==(const UI_SysInfo&) = delete;
			private:
				template<typename T>
				void imGuiDrawTableEntry(const TSysInfo(T)& pInfo) {
					// Row seperator
					ImGui::Separator();

					// TITLE
					ImGui::Text(pInfo.strName.c_str());
					ImGui::NextColumn();

					// VALUE
					ImGui::Text(pInfo.strValue.c_str());
					ImGui::NextColumn();
				}

			private:
				/// <summary>
				/// Gpu adapter info
				/// </summary>
				DXGI_ADAPTER_DESC m_gpuAdapterDesc;

				/// <summary>
				/// GPU Feature support
				/// </summary>
				GxDirect::XFeatureSupport m_supportObject;

				/// <summary>
				/// Selected DXGI_FORMATE to query info
				/// </summary>
				INT m_uiSelectedFormate = 0;

				/// <summary>
				/// Format support flags 1
				/// </summary>
				D3D12_FORMAT_SUPPORT1 m_fsSupport1 = D3D12_FORMAT_SUPPORT1_NONE;

				/// <summary>
				/// Format support flags 2
				/// </summary>
				D3D12_FORMAT_SUPPORT2 m_fsSupport2 = D3D12_FORMAT_SUPPORT2_NONE;

				std::string m_strFsMsaa = "1X (Quality 0)";
				
				/// <summary>
				/// CPUID 
				/// </summary>
				GxUtil::CPUID_RESULT m_cpuId;

				/// <summary>
				/// Supported instruction sets
				/// </summary>
				std::string m_cpuInstuctionSets;

				/// <summary>
				/// GPU Core count
				/// </summary>
				std::string m_strSystemMemory;

				/// <summary>
				/// GPU Core count
				/// </summary>
				std::string m_strCoreCount;

				/// <summary>
				/// Vram string buffer
				/// </summary>
				std::string m_strVram;

				/// <summary>
				/// Shared memory
				/// </summary>
				std::string m_strShared;

				/// <summary>
				/// String for feature level
				/// </summary>
				std::string m_strFeatureLevel;

				/// <summary>
				/// Name of the Computer
				/// </summary>
				std::string m_strOsComputer;

				/// <summary>
				/// Name of the operating system
				/// </summary>
				std::string m_strOsDescription;

				/// <summary>
				/// Windows directory
				/// </summary>
				std::string m_strOsRootDir;

				/// <summary>
				/// Build number of the OS
				/// </summary>
				std::string m_strOsBuildNumber;

				/// <summary>
				/// Version of the OS
				/// </summary>
				std::string m_strOsVersion;

				/// <summary>
				/// String for windows serial number
				/// </summary>
				std::string m_strOsSerial;

				/// <summary>
				/// Inidcates if the serial should be shown
				/// </summary>
				BOOL m_bShowSerial = FALSE;

				/// <summary>
				/// Show time of the serial number
				/// </summary>
				GxUtil::StopWatch m_swShowTime;


				/// <summary>
				/// Pointer to context
				/// </summary>
				GxDirect::XContext* m_ptrContext;

				/// <summary>
				/// Indicates if UI element should be visible
				/// </summary>
				BOOL m_bVisible = FALSE;
		};
	}
}