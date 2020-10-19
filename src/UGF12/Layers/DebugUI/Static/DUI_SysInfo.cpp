#include "pch.h"
#include "DUI_SysInfo.h"

UGF12::DebugUI::UI_SysInfo::UI_SysInfo(GxDirect::XContext* ptrContext) :
	m_ptrContext(ptrContext),
	m_supportObject(ptrContext)
{
	// Inrement ref
	m_ptrContext->IncRef();

	// String builder
	std::stringstream ss;

	// Query CPUID
	GxUtil::CPUID_Factory::queryCpu(&m_cpuId);

	// Instuction list
	SSTREAM_OUT(m_cpuId.ABM(), ss, "ABM");
	SSTREAM_OUT(m_cpuId.ADX(), ss, ", ADX");
	SSTREAM_OUT(m_cpuId.AES(), ss, ", AES");
	SSTREAM_OUT(m_cpuId.AVX(), ss, ", AVX");
	SSTREAM_OUT(m_cpuId.AVX2(), ss, ", AVX2");
	SSTREAM_OUT(m_cpuId.AVX512CD(), ss, ", AVX512CD");
	SSTREAM_OUT(m_cpuId.AVX512ER(), ss, ", AVX512ER");
	SSTREAM_OUT(m_cpuId.AVX512F(), ss, ", AVX512F");
	SSTREAM_OUT(m_cpuId.AVX512PF(), ss, ", AVX512PF");
	SSTREAM_OUT(m_cpuId.BMI1(), ss, ", BMI1");
	SSTREAM_OUT(m_cpuId.BMI2(), ss, ", BMI2");
	SSTREAM_OUT(m_cpuId.CLFSH(), ss, ", CLFSH");
	SSTREAM_OUT(m_cpuId.CMOV(), ss, ", CMOV");
	SSTREAM_OUT(m_cpuId.CMPXCHG16B(), ss, ", CMPXCHG16B");
	SSTREAM_OUT(m_cpuId.CX8(), ss, ", CX8");
	SSTREAM_OUT(m_cpuId.ERMS(), ss, ", ERMS");
	SSTREAM_OUT(m_cpuId.F16C(), ss, ", F16C");
	SSTREAM_OUT(m_cpuId.FMA(), ss, ", FMA");
	SSTREAM_OUT(m_cpuId.FSGSBASE(), ss, ", FSGSBASE");
	SSTREAM_OUT(m_cpuId.FXSR(), ss, ", FXSR");
	SSTREAM_OUT(m_cpuId.HLE(), ss, ", HLE");
	SSTREAM_OUT(m_cpuId.INVPCID(), ss, ", INVPCID");
	SSTREAM_OUT(m_cpuId.LAHF(), ss, ", LAHF");
	SSTREAM_OUT(m_cpuId.LZCNT(), ss, ", LZCNT");
	SSTREAM_OUT(m_cpuId.MMX(), ss, ", MMX");
	SSTREAM_OUT(m_cpuId.MMXEXT(), ss, ", MMXEXT");
	SSTREAM_OUT(m_cpuId.MONITOR(), ss, ", MONITOR");
	SSTREAM_OUT(m_cpuId.MOVBE(), ss, ", MOVBE");
	SSTREAM_OUT(m_cpuId.MSR(), ss, ", MSR");
	SSTREAM_OUT(m_cpuId.OSXSAVE(), ss, ", OSXSAVE");
	SSTREAM_OUT(m_cpuId.PCLMULQDQ(), ss, ", PCLMULQDQ");
	SSTREAM_OUT(m_cpuId.POPCNT(), ss, ", POPCNT");
	SSTREAM_OUT(m_cpuId.PREFETCHWT1(), ss, ", PREFETCHWT1");
	SSTREAM_OUT(m_cpuId.RDRAND(), ss, ", RDRAND");
	SSTREAM_OUT(m_cpuId.RDSEED(), ss, ", RDSEED");
	SSTREAM_OUT(m_cpuId.RDTSCP(), ss, ", RDTSCP");
	SSTREAM_OUT(m_cpuId.RTM(), ss, ", RTM");
	SSTREAM_OUT(m_cpuId.SEP(), ss, ", SEP");
	SSTREAM_OUT(m_cpuId.SHA(), ss, ", SHA");
	SSTREAM_OUT(m_cpuId.SSE(), ss, ", SSE");
	SSTREAM_OUT(m_cpuId.SSE2(), ss, ", SSE2");
	SSTREAM_OUT(m_cpuId.SSE3(), ss, ", SSE3");
	SSTREAM_OUT(m_cpuId.SSE41(), ss, ", SSE41");
	SSTREAM_OUT(m_cpuId.SSE42(), ss, ", SSE42");
	SSTREAM_OUT(m_cpuId.SSE4a(), ss, ", SSE4a");
	SSTREAM_OUT(m_cpuId.SSSE3(), ss, ", SSSE3");
	SSTREAM_OUT(m_cpuId.SYSCALL(), ss, ", SYSCALL");
	SSTREAM_OUT(m_cpuId.TBM(), ss, ", TBM");
	SSTREAM_OUT(m_cpuId.XOP(), ss, ", XOP");
	SSTREAM_OUT(m_cpuId.XSAVE(), ss, ", XSAVE");
	SSTREAM_OUT(m_cpuId._3DNOW(), ss, ", _3DNOW");
	SSTREAM_OUT(m_cpuId._3DNOWEXT(), ss, ", _3DNOWEXT");

	m_cpuInstuctionSets = ss.str();

	// Get memory
	ss.str("");
	ss << GxUtil::SystemMetrics::getSystemMemory() / 1024 / 1024 << " GB";
	m_strSystemMemory = ss.str();

	// Get device
	ID3D12Device* ptrDeivce;
	m_ptrContext->getDevice(&ptrDeivce);

	// Get GPU Info
	m_gpuAdapterDesc = m_ptrContext->getGpuInfo();

	// Retrive VRAM
	UINT64 vram = m_gpuAdapterDesc.DedicatedVideoMemory;

	// Try to get acurate vram from registry
	HKEY regKey = NULL;
	if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, GPU_REG_KEY, NULL, STANDARD_RIGHTS_READ | KEY_QUERY_VALUE, &regKey) == ERROR_SUCCESS) {
		// Try to read value
		UINT64 memory;
		DWORD len = sizeof(UINT64);
		if (RegGetValueW(regKey, NULL, GPU_REG_NAME, GPU_REG_TYPE, NULL, &memory, &len) == ERROR_SUCCESS) {
			vram = memory;
		}

		// Close registry
		RegCloseKey(regKey);
	}	

	// Convert VRAM
	ss.str("");
	ss << (vram / 1024.0 / 1024.0 / 1024.0) << "GB";
	m_strVram = ss.str();

	// Convert Shadred memory
	ss.str("");
	ss << (m_gpuAdapterDesc.SharedSystemMemory / 1024.0 / 1024.0 / 1024.0) << "GB";
	m_strShared = ss.str();

	// GPU Core count
	D3D12_FEATURE_DATA_D3D12_OPTIONS1 op1;
	ZeroMemory(&op1, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS1));
	if (FAILED(ptrDeivce->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS1, &op1, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS1)))) {
		// Set not supprted text
		m_strCoreCount = "Query not supported!";
	}
	else {
		ss.str("");
		ss << op1.TotalLaneCount << " (Wave lane min: " << op1.WaveLaneCountMin << ")";
		m_strCoreCount = ss.str();
	}

	// Get feature level
	switch (m_ptrContext->getFeatureLevel()) {
		case D3D_FEATURE_LEVEL_11_0:
			m_strFeatureLevel = "D3D_FEATURE_LEVEL_11_0";
			break;
		case D3D_FEATURE_LEVEL_11_1:
			m_strFeatureLevel = "D3D_FEATURE_LEVEL_11_1";
			break;
		case D3D_FEATURE_LEVEL_12_0:
			m_strFeatureLevel = "D3D_FEATURE_LEVEL_12_0";
			break;
		case D3D_FEATURE_LEVEL_12_1:
			m_strFeatureLevel = "D3D_FEATURE_LEVEL_12_1";
			break;
	}

	// Release device
	COM_RELEASE(ptrDeivce);

	// Get OS Caption
	VARIANT osCaption;
	if (FAILED(GxUtil::IWebmProvider::getInstance()->queryValue(L"Win32_OperatingSystem", L"Caption", &osCaption))) {
		m_strOsDescription = "Unknown OS";
	} else {
		m_strOsDescription = GxUtil::StrConverter::ConvertW2N(osCaption.bstrVal);
		VariantClear(&osCaption);
	}

	// Get OS Computer
	VARIANT osComputer;
	if (FAILED(GxUtil::IWebmProvider::getInstance()->queryValue(L"Win32_OperatingSystem", L"CSName", &osComputer))) {
		m_strOsComputer = "Local computer";
	}
	else {
		m_strOsComputer = GxUtil::StrConverter::ConvertW2N(osComputer.bstrVal);
		VariantClear(&osComputer);
	}

	// Get OS Root Dir
	VARIANT osRootDir;
	if (FAILED(GxUtil::IWebmProvider::getInstance()->queryValue(L"Win32_OperatingSystem", L"WindowsDirectory", &osRootDir))) {
		m_strOsRootDir = "Unknown";
	}
	else {
		m_strOsRootDir = GxUtil::StrConverter::ConvertW2N(osRootDir.bstrVal);
		VariantClear(&osRootDir);
	}

	// Get OS Build number
	VARIANT osBuildNumber;
	if (FAILED(GxUtil::IWebmProvider::getInstance()->queryValue(L"Win32_OperatingSystem", L"BuildNumber", &osBuildNumber))) {
		m_strOsBuildNumber = "Unknown";
	}
	else {
		m_strOsBuildNumber = GxUtil::StrConverter::ConvertW2N(osBuildNumber.bstrVal);
		VariantClear(&osBuildNumber);
	}

	// Get OS Version
	VARIANT osVersion;
	if (FAILED(GxUtil::IWebmProvider::getInstance()->queryValue(L"Win32_OperatingSystem", L"Version", &osVersion))) {
		m_strOsVersion = "Unknown";
	}
	else {
		m_strOsVersion = GxUtil::StrConverter::ConvertW2N(osVersion.bstrVal);
		VariantClear(&osVersion);
	}

	// Get OS Serial
	VARIANT osSerial;
	if (FAILED(GxUtil::IWebmProvider::getInstance()->queryValue(L"Win32_OperatingSystem", L"SerialNumber", &osSerial))) {
		m_strOsSerial = "?????-?????-?????-?????";
	}
	else {
		m_strOsSerial = GxUtil::StrConverter::ConvertW2N(osSerial.bstrVal);
		VariantClear(&osRootDir);
	}
}

UGF12::DebugUI::UI_SysInfo::~UI_SysInfo(){
	// Decrement ref
	m_ptrContext->DecRef();
}

void UGF12::DebugUI::UI_SysInfo::draw(GxRenderIO::LayerStack::LayerFrameInfo* ptrFrameInfo, ImGuiIO* ptrImGuiIo) {
	#if !defined(SHIPPING)
	// If visible
	if (m_bVisible) {
		// Metric window
		if (ImGui::Begin("System Info", (bool*)&m_bVisible), ImGuiWindowFlags_NoCollapse) {
            if (ImGui::BeginTabBar("SystemInfo")) {				
				// TAB CPU
                if (ImGui::BeginTabItem("System & CPU")) {
					// === Metrics ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "System Memory:");

					// One row has 2 colums
					ImGui::Columns(2);

					// Row #1
					ImGui::Separator();
					// System memory : TITLE
					ImGui::Text("System Memory");

					ImGui::NextColumn();
					// System memory : VALUE
					ImGui::Text(m_strSystemMemory.c_str());

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// === Metrics ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nCPU Metrics:");

					// One row has 2 colums
					ImGui::Columns(2);

					// Row #1
					ImGui::Separator();
					// CPU Count : TITLE
					ImGui::Text("CPU / Processor count");

					ImGui::NextColumn();
					// CPU Count : VALUE
					ImGui::Text(std::to_string(GxUtil::SystemMetrics::getCpuCount()).c_str());

					// Row #2
					ImGui::NextColumn();
					ImGui::Separator();
					// CPU Core Count : TITLE
					ImGui::Text("Physical core count");

					ImGui::NextColumn();
					// CPU Core Count : VALUE
					ImGui::Text(std::to_string(GxUtil::SystemMetrics::getCpuCoresCount()).c_str());

					// Row #3
					ImGui::NextColumn();
					ImGui::Separator();
					// CPU Core Count Logical : TITLE
					ImGui::Text("Logical core count");

					ImGui::NextColumn();
					// CPU Core Count Logical : VALUE
					ImGui::Text(std::to_string(GxUtil::SystemMetrics::getCpuLogicalCoreCount()).c_str());

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// === Cache ==
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nCPU Caches:");

					// One row has 2 colums
					ImGui::Columns(2);

					// For each cache
					for (UINT i = 0; i < GxUtil::SystemMetrics::getCacheCount(); i++) {
						// Get cache at index
						GxUtil::SystemMetrics::CpuCache cache = GxUtil::SystemMetrics::getCpuCache(i);

						// Build string
						std::stringstream ss;
						ss << "L" << cache.level;

						switch (cache.type) {
							case CacheTrace:
								ss << " Trace Cache";
								break;
							case CacheData:
								ss << " Data Cache";
								break;
							case CacheInstruction:
								ss << " Instruction Cache";
								break;
							case CacheUnified:
								ss << " Unified Cache";
								break;
						}

						ImGui::Separator();
						ImGui::Text(ss.str().c_str());
						ImGui::NextColumn();

						ss.str("");
						ss << cache.count << "x " << cache.translatedString;

						ImGui::Text(ss.str().c_str());
						ImGui::NextColumn();
					}

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// === CPUID ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nCPUID:");

					// One row has 2 colums
					ImGui::Columns(2);

					// Row #1
					ImGui::Separator();
					// CPU Vendor : TITLE
					ImGui::Text("CPU Vendor");

					ImGui::NextColumn();
					// CPU Vendor : VALUE
					switch (m_cpuId.vendor) {
						case GxUtil::CPU_VENDOR::AMD:
							ImGui::Text("AMD");
							break;
						case GxUtil::CPU_VENDOR::INTEL:
							ImGui::Text("Intel");
							break;
						case GxUtil::CPU_VENDOR::VM_OR_UNKNOWN:
							ImGui::Text("Unknown");
							break;
					}
					

					// Row #2
					ImGui::NextColumn();
					ImGui::Separator();
					// CPU Name : TITLE
					ImGui::Text("CPU Description");

					ImGui::NextColumn();
					// CPU Name: VALUE
					ImGui::TextWrapped(m_cpuId.brandString.c_str());

					// Row #3
					ImGui::NextColumn();
					ImGui::Separator();
					// CPU Name : TITLE
					ImGui::Text("CPU Instructions");

					ImGui::NextColumn();
					// CPU Name: VALUE
					ImGui::TextWrapped(m_cpuInstuctionSets.c_str());

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// End item
                    ImGui::EndTabItem();
                }

                // TAB GPU
                if (ImGui::BeginTabItem("GPU")) {
					// === Basic GPU ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Basic GPU informations:");
					
					// One row has 2 colums
					ImGui::Columns(2);
					
					// Row #1
					ImGui::Separator();
					// GPU Name : TITLE
					ImGui::Text("GPU Name / Description");
					
					ImGui::NextColumn();
					// GPU Name : VALUE
					ImGui::Text(GxUtil::StrConverter::ConvertW2N(m_gpuAdapterDesc.Description).c_str());

					// Row #2
					ImGui::NextColumn();
					ImGui::Separator();
					// GPU Core : TITLE
					ImGui::Text("Shader / Core count");

					ImGui::NextColumn();
					// GPU Core : VALUE
					ImGui::Text(m_strCoreCount.c_str());

					// Row #3
					ImGui::NextColumn();
					ImGui::Separator();
					// GPU VRAM : TITLE
					ImGui::Text("GPU VRAM");

					ImGui::NextColumn();
					// GPU VRAM : VALUE
					ImGui::Text(m_strVram.c_str());

					// Row #4
					ImGui::NextColumn();
					ImGui::Separator();
					// GPU SYS-RAM : TITLE
					ImGui::Text("GPU shared system memory");

					ImGui::NextColumn();
					// GPU SYS-RAM : VALUE
					ImGui::Text(m_strShared.c_str());

					// Row #5
					ImGui::NextColumn();
					ImGui::Separator();
					// GPU FL : TITLE
					ImGui::Text("Feature level");

					ImGui::NextColumn();
					// GPU FL: VALUE
					ImGui::Text(m_strFeatureLevel.c_str());

					// Row #6
					ImGui::NextColumn();
					ImGui::Separator();
					// GPU FL : TITLE
					ImGui::Text(m_supportObject.getShaderModel().strName.c_str());

					ImGui::NextColumn();
					// GPU FL: VALUE
					ImGui::Text(m_supportObject.getShaderModel().strValue.c_str());

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// === D3D12_FEATURE_DATA_D3D12_OPTIONS  ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nD3D12_FEATURE_DATA_D3D12_OPTIONS:");

					// Start Colums
					ImGui::Columns(2);

					// Draw elements
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions0().getDoublePrecisionFloatShaderOps());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions0().getOutputMergerLogicOp());
					imGuiDrawTableEntry<D3D12_SHADER_MIN_PRECISION_SUPPORT>(m_supportObject.getOptions0().getMinPrecisionSupport());
					imGuiDrawTableEntry<D3D12_TILED_RESOURCES_TIER>(m_supportObject.getOptions0().getTiledResourcesTier());
					imGuiDrawTableEntry<D3D12_RESOURCE_BINDING_TIER>(m_supportObject.getOptions0().getResourceBindingTier());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions0().getPSSpecifiedStencilRefSupported());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions0().getTypedUAVLoadAdditionalFormats());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions0().getROVsSupported());
					imGuiDrawTableEntry<D3D12_CONSERVATIVE_RASTERIZATION_TIER>(m_supportObject.getOptions0().getConservativeRasterizationTier());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions0().getStandardSwizzle64KBSupported());
					imGuiDrawTableEntry<D3D12_CROSS_NODE_SHARING_TIER>(m_supportObject.getOptions0().getCrossNodeSharingTier());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions0().getCrossAdapterRowMajorTextureSupported());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions0().getVPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation());
					imGuiDrawTableEntry<D3D12_RESOURCE_HEAP_TIER>(m_supportObject.getOptions0().getResourceHeapTier());

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// === nD3D12_FEATURE_DATA_D3D12_OPTIONS1  ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nD3D12_FEATURE_DATA_D3D12_OPTIONS1:");

					// Start Colums
					ImGui::Columns(2);

					// Draw elements
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions1().getWaveOps());
					imGuiDrawTableEntry<UINT>(m_supportObject.getOptions1().getWaveLaneCount());
					imGuiDrawTableEntry<UINT>(m_supportObject.getOptions1().getTotalLaneCount());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions1().getInt64ShaderOps());

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// === D3D12_FEATURE_DATA_D3D12_OPTIONS2  ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nD3D12_FEATURE_DATA_D3D12_OPTIONS2:");

					// Start Colums
					ImGui::Columns(2);

					// Draw elements
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions2().getDepthBoundsTestSupported());
					imGuiDrawTableEntry<D3D12_PROGRAMMABLE_SAMPLE_POSITIONS_TIER>(m_supportObject.getOptions2().getProgrammableSamplePositionsTier());

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// === D3D12_FEATURE_DATA_D3D12_OPTIONS3  ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nD3D12_FEATURE_DATA_D3D12_OPTIONS3:");

					// Start Colums
					ImGui::Columns(2);

					// Draw elements
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions3().getCopyQueueTimestampQueriesSupported());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions3().getCastingFullyTypedFormatSupported());
					imGuiDrawTableEntry<D3D12_VIEW_INSTANCING_TIER>(m_supportObject.getOptions3().getViewInstancingTier());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions3().getBarycentricsSupported());

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// === D3D12_FEATURE_DATA_D3D12_OPTIONS4  ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nD3D12_FEATURE_DATA_D3D12_OPTIONS4:");

					// Start Colums
					ImGui::Columns(2);

					// Draw elements
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions4().getMSAA64KBAlignedTextureSupported());
					imGuiDrawTableEntry<D3D12_SHARED_RESOURCE_COMPATIBILITY_TIER>(m_supportObject.getOptions4().getSharedResourceCompatibilityTier());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions4().getNative16BitShaderOpsSupported());

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// === D3D12_FEATURE_DATA_D3D12_OPTIONS5  ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nD3D12_FEATURE_DATA_D3D12_OPTIONS5:");

					// Start Colums
					ImGui::Columns(2);

					// Draw elements
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions5().getSRVOnlyTiledResourceTier3());
					imGuiDrawTableEntry<D3D12_RENDER_PASS_TIER>(m_supportObject.getOptions5().getRenderPassesTier());
					imGuiDrawTableEntry<D3D12_RAYTRACING_TIER >(m_supportObject.getOptions5().getRaytracingTier());

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// === D3D12_FEATURE_DATA_D3D12_OPTIONS6  ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nD3D12_FEATURE_DATA_D3D12_OPTIONS6:");

					// Start Colums
					ImGui::Columns(2);

					// Draw elements
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions6().getAdditionalShadingRatesSupported());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions6().getPerPrimitiveShadingRateSupportedWithViewportIndexing());
					imGuiDrawTableEntry<D3D12_VARIABLE_SHADING_RATE_TIER>(m_supportObject.getOptions6().getVariableShadingRateTier());
					imGuiDrawTableEntry<UINT>(m_supportObject.getOptions6().getShadingRateImageTileSize());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getOptions6().getBackgroundProcessingSupported());

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// === D3D12_FEATURE_DATA_D3D12_OPTIONS7  ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nD3D12_FEATURE_DATA_D3D12_OPTIONS7:");

					// Start Colums
					ImGui::Columns(2);

					// Draw elements
					imGuiDrawTableEntry<D3D12_MESH_SHADER_TIER>(m_supportObject.getOptions7().getMeshShaderTier());
					imGuiDrawTableEntry<D3D12_SAMPLER_FEEDBACK_TIER>(m_supportObject.getOptions7().getSamplerFeedbackTier());

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// === D3D12_FEATURE_DATA_ARCHITECTURE1   ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nD3D12_FEATURE_DATA_ARCHITECTURE1:");

					// Start Colums
					ImGui::Columns(2);

					// Draw elements
					imGuiDrawTableEntry<UINT>(m_supportObject.getArchitecture1().getNodeIndex());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getArchitecture1().getTileBasedRenderer());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getArchitecture1().getUMA());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getArchitecture1().getCacheCoherentUMA());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getArchitecture1().getIsolatedMMU());

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// === D3D12_FEATURE_DATA_CROSS_NODE   ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nD3D12_FEATURE_DATA_CROSS_NODE:");

					// Start Colums
					ImGui::Columns(2);

					// Draw elements
					imGuiDrawTableEntry<D3D12_CROSS_NODE_SHARING_TIER>(m_supportObject.getCrossNode().getSharingTier());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getCrossNode().getAtomicShaderInstructions());

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// === D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT   ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nD3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT:");

					// Start Colums
					ImGui::Columns(2);

					// Draw elements
					imGuiDrawTableEntry<UINT>(m_supportObject.getVirtualAddress().getMaxGPUVirtualAddressBitsPerResource());
					imGuiDrawTableEntry<UINT>(m_supportObject.getVirtualAddress().getMaxGPUVirtualAddressBitsPerProcess());

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// === Additional   ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nAdditional:");

					// Start Colums
					ImGui::Columns(2);

					// Draw elements
					imGuiDrawTableEntry<D3D_ROOT_SIGNATURE_VERSION>(m_supportObject.getRootSignatureVersion());
					imGuiDrawTableEntry<D3D12_SHADER_CACHE_SUPPORT_FLAGS>(m_supportObject.getShaderCacheSupportFlags());
					imGuiDrawTableEntry<BOOL>(m_supportObject.getExisitingHeapSupport());

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// End of tab 
					ImGui::EndTabItem();
                }

				// TAB Windows
				if (ImGui::BeginTabItem("OS")) {
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Windows OS Informations:");

					// One row has 2 colums
					ImGui::Columns(2);

					// Row #1
					ImGui::Separator();
					// OS Computer : TITLE
					ImGui::Text("Computer name");

					ImGui::NextColumn();
					// OS Computer : VALUE
					ImGui::Text(m_strOsComputer.c_str());

					// Row #2
					ImGui::NextColumn();
					ImGui::Separator();
					// OS Name : TITLE
					ImGui::Text("Operating system");

					ImGui::NextColumn();
					// OS Name : VALUE
					ImGui::Text(m_strOsDescription.c_str());

					// Row #3
					ImGui::NextColumn();
					ImGui::Separator();
					// OS Version : TITLE
					ImGui::Text("OS Version");

					ImGui::NextColumn();
					// OS Version : VALUE
					ImGui::Text(m_strOsVersion.c_str());

					// Row #4
					ImGui::NextColumn();
					ImGui::Separator();
					// OS Build : TITLE
					ImGui::Text("OS Build");

					ImGui::NextColumn();
					// OS Build : VALUE
					ImGui::Text(m_strOsBuildNumber.c_str());

					// Row #5
					ImGui::NextColumn();
					ImGui::Separator();
					// OS Name : TITLE
					ImGui::Text("OS home directory");

					ImGui::NextColumn();
					// OS Name : VALUE
					ImGui::Text(m_strOsRootDir.c_str());

					// End Items
					ImGui::Separator();
					ImGui::Columns(1);

					// Serial number
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nWindows serial number:");
					ImGui::TextWrapped("UGF12 is able to retrive your windows serial number. The serial number itself will not be included in any logs.\n\t");
					if (m_bShowSerial) {
						ImGui::Text(m_strOsSerial.c_str());

						if (m_swShowTime.getElapsedMs() >= 10000.0f) {
							m_bShowSerial = FALSE;
						}
					}
					else {
						if (ImGui::Button("Ctrl + Click to show serial number") && (((1 << 15)) & GetAsyncKeyState(VK_CONTROL))) {
							m_bShowSerial = TRUE;
							m_swShowTime.reset();
							m_swShowTime.start();
						}
					}

					

					// End of tab 
					ImGui::EndTabItem();
				} else {
					// Clear serial
					m_bShowSerial = FALSE;
				}

				if (ImGui::BeginTabItem("DXGI_FORMATE")) {
					const char* formatesStrings[] = {
						"DXGI_FORMAT_UNKNOWN",
						"DXGI_FORMAT_R32G32B32A32_TYPELESS",
						"DXGI_FORMAT_R32G32B32A32_FLOAT",
						"DXGI_FORMAT_R32G32B32A32_UINT",
						"DXGI_FORMAT_R32G32B32A32_SINT",
						"DXGI_FORMAT_R32G32B32_TYPELESS",
						"DXGI_FORMAT_R32G32B32_FLOAT",
						"DXGI_FORMAT_R32G32B32_UINT",
						"DXGI_FORMAT_R32G32B32_SINT",
						"DXGI_FORMAT_R16G16B16A16_TYPELESS",
						"DXGI_FORMAT_R16G16B16A16_FLOAT",
						"DXGI_FORMAT_R16G16B16A16_UNORM",
						"DXGI_FORMAT_R16G16B16A16_UINT",
						"DXGI_FORMAT_R16G16B16A16_SNORM",
						"DXGI_FORMAT_R16G16B16A16_SINT",
						"DXGI_FORMAT_R32G32_TYPELESS",
						"DXGI_FORMAT_R32G32_FLOAT",
						"DXGI_FORMAT_R32G32_UINT",
						"DXGI_FORMAT_R32G32_SINT",
						"DXGI_FORMAT_R32G8X24_TYPELESS",
						"DXGI_FORMAT_D32_FLOAT_S8X24_UINT",
						"DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS",
						"DXGI_FORMAT_X32_TYPELESS_G8X24_UINT",
						"DXGI_FORMAT_R10G10B10A2_TYPELESS",
						"DXGI_FORMAT_R10G10B10A2_UNORM",
						"DXGI_FORMAT_R10G10B10A2_UINT",
						"DXGI_FORMAT_R11G11B10_FLOAT",
						"DXGI_FORMAT_R8G8B8A8_TYPELESS",
						"DXGI_FORMAT_R8G8B8A8_UNORM",
						"DXGI_FORMAT_R8G8B8A8_UNORM_SRGB",
						"DXGI_FORMAT_R8G8B8A8_UINT",
						"DXGI_FORMAT_R8G8B8A8_SNORM",
						"DXGI_FORMAT_R8G8B8A8_SINT",
						"DXGI_FORMAT_R16G16_TYPELESS",
						"DXGI_FORMAT_R16G16_FLOAT",
						"DXGI_FORMAT_R16G16_UNORM",
						"DXGI_FORMAT_R16G16_UINT",
						"DXGI_FORMAT_R16G16_SNORM",
						"DXGI_FORMAT_R16G16_SINT",
						"DXGI_FORMAT_R32_TYPELESS",
						"DXGI_FORMAT_D32_FLOAT",
						"DXGI_FORMAT_R32_FLOAT",
						"DXGI_FORMAT_R32_UINT",
						"DXGI_FORMAT_R32_SINT",
						"DXGI_FORMAT_R24G8_TYPELESS",
						"DXGI_FORMAT_D24_UNORM_S8_UINT",
						"DXGI_FORMAT_R24_UNORM_X8_TYPELESS",
						"DXGI_FORMAT_X24_TYPELESS_G8_UINT",
						"DXGI_FORMAT_R8G8_TYPELESS",
						"DXGI_FORMAT_R8G8_UNORM",
						"DXGI_FORMAT_R8G8_UINT",
						"DXGI_FORMAT_R8G8_SNORM",
						"DXGI_FORMAT_R8G8_SINT",
						"DXGI_FORMAT_R16_TYPELESS",
						"DXGI_FORMAT_R16_FLOAT",
						"DXGI_FORMAT_D16_UNORM",
						"DXGI_FORMAT_R16_UNORM",
						"DXGI_FORMAT_R16_UINT",
						"DXGI_FORMAT_R16_SNORM",
						"DXGI_FORMAT_R16_SINT",
						"DXGI_FORMAT_R8_TYPELESS",
						"DXGI_FORMAT_R8_UNORM",
						"DXGI_FORMAT_R8_UINT",
						"DXGI_FORMAT_R8_SNORM",
						"DXGI_FORMAT_R8_SINT",
						"DXGI_FORMAT_A8_UNORM",
						"DXGI_FORMAT_R1_UNORM",
						"DXGI_FORMAT_R9G9B9E5_SHAREDEXP",
						"DXGI_FORMAT_R8G8_B8G8_UNORM",
						"DXGI_FORMAT_G8R8_G8B8_UNORM",
						"DXGI_FORMAT_BC1_TYPELESS",
						"DXGI_FORMAT_BC1_UNORM",
						"DXGI_FORMAT_BC1_UNORM_SRGB",
						"DXGI_FORMAT_BC2_TYPELESS",
						"DXGI_FORMAT_BC2_UNORM",
						"DXGI_FORMAT_BC2_UNORM_SRGB",
						"DXGI_FORMAT_BC3_TYPELESS",
						"DXGI_FORMAT_BC3_UNORM",
						"DXGI_FORMAT_BC3_UNORM_SRGB",
						"DXGI_FORMAT_BC4_TYPELESS",
						"DXGI_FORMAT_BC4_UNORM",
						"DXGI_FORMAT_BC4_SNORM",
						"DXGI_FORMAT_BC5_TYPELESS",
						"DXGI_FORMAT_BC5_UNORM",
						"DXGI_FORMAT_BC5_SNORM",
						"DXGI_FORMAT_B5G6R5_UNORM",
						"DXGI_FORMAT_B5G5R5A1_UNORM",
						"DXGI_FORMAT_B8G8R8A8_UNORM",
						"DXGI_FORMAT_B8G8R8X8_UNORM",
						"DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM",
						"DXGI_FORMAT_B8G8R8A8_TYPELESS",
						"DXGI_FORMAT_B8G8R8A8_UNORM_SRGB",
						"DXGI_FORMAT_B8G8R8X8_TYPELESS",
						"DXGI_FORMAT_B8G8R8X8_UNORM_SRGB",
						"DXGI_FORMAT_BC6H_TYPELESS",
						"DXGI_FORMAT_BC6H_UF16",
						"DXGI_FORMAT_BC6H_SF16",
						"DXGI_FORMAT_BC7_TYPELESS",
						"DXGI_FORMAT_BC7_UNORM",
						"DXGI_FORMAT_BC7_UNORM_SRGB",
						"DXGI_FORMAT_AYUV",
						"DXGI_FORMAT_Y410",
						"DXGI_FORMAT_Y416",
						"DXGI_FORMAT_NV12",
						"DXGI_FORMAT_P010",
						"DXGI_FORMAT_P016",
						"DXGI_FORMAT_420_OPAQUE",
						"DXGI_FORMAT_YUY2",
						"DXGI_FORMAT_Y210",
						"DXGI_FORMAT_Y216",
						"DXGI_FORMAT_NV11",
						"DXGI_FORMAT_AI44",
						"DXGI_FORMAT_IA44",
						"DXGI_FORMAT_P8",
						"DXGI_FORMAT_A8P8",
						"DXGI_FORMAT_B4G4R4A4_UNORM",
					};
					
					// === Select formate ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Select DXGI Formate:");
					if (ImGui::Combo("DXGI_FORMATE", &m_uiSelectedFormate, formatesStrings, 116)) {
						// --> Change event
						// Check support for format
						m_supportObject.checkFormatSupport((DXGI_FORMAT)m_uiSelectedFormate, &m_fsSupport1, &m_fsSupport2);

						// Check msaa suppoert
						std::stringstream ss;
						UINT msaaQuality = 1;
						UINT msaaCount = 2;
						while (msaaQuality > 0) {
							// Query
							msaaQuality = m_supportObject.checkMsaaSupport((DXGI_FORMAT)m_uiSelectedFormate, msaaCount);

							if (msaaQuality) {
								ss << msaaCount << "X (Quality: " << msaaQuality << ")\n";
							}

							msaaCount *= 2;
						}

						// Check if has length
						if (ss.str().length() > 0) {
							m_strFsMsaa = ss.str();
						}
						else {
							m_strFsMsaa = "1X (Quality 0)";
						}
					}

					// === Support MSAA ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nFormat MSAA Support:");
					ImGui::Text(m_strFsMsaa.c_str());

					// === Support types ===
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nFormat support:");

					// D3D12_FORMAT_SUPPORT1_BUFFER
					bool flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_BUFFER);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_BUFFER", &flag);

					// D3D12_FORMAT_SUPPORT1_IA_VERTEX_BUFFER
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_IA_VERTEX_BUFFER);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_IA_VERTEX_BUFFER", &flag);

					// D3D12_FORMAT_SUPPORT1_IA_INDEX_BUFFER
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_IA_INDEX_BUFFER);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_IA_INDEX_BUFFER", &flag);

					// D3D12_FORMAT_SUPPORT1_SO_BUFFER
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_SO_BUFFER);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_SO_BUFFER", &flag);

					// D3D12_FORMAT_SUPPORT1_TEXTURE1D
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_TEXTURE1D);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_TEXTURE1D", &flag);

					// D3D12_FORMAT_SUPPORT1_TEXTURE2D
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_TEXTURE2D);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_TEXTURE2D", &flag);

					// D3D12_FORMAT_SUPPORT1_TEXTURE3D
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_TEXTURE3D);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_TEXTURE3D", &flag);

					// D3D12_FORMAT_SUPPORT1_TEXTURECUBE
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_TEXTURECUBE);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_TEXTURECUBE", &flag);

					// D3D12_FORMAT_SUPPORT1_SHADER_LOAD
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_SHADER_LOAD);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_SHADER_LOAD", &flag);

					// D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE", &flag);

					// D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE_COMPARISON
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE_COMPARISON);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE_COMPARISON", &flag);

					// D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE_MONO_TEXT
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE_MONO_TEXT);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE_MONO_TEXT", &flag);
					
					// D3D12_FORMAT_SUPPORT1_MIP
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_MIP);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_MIP", &flag);

					// D3D12_FORMAT_SUPPORT1_RENDER_TARGET
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_RENDER_TARGET);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_RENDER_TARGET", &flag);

					// D3D12_FORMAT_SUPPORT1_BLENDABLE
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_BLENDABLE);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_BLENDABLE", &flag);

					// D3D12_FORMAT_SUPPORT1_DEPTH_STENCIL
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_DEPTH_STENCIL);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_DEPTH_STENCIL", &flag);

					// D3D12_FORMAT_SUPPORT1_MULTISAMPLE_RESOLVE
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_MULTISAMPLE_RESOLVE);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_MULTISAMPLE_RESOLVE", &flag);

					// D3D12_FORMAT_SUPPORT1_DISPLAY
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_DISPLAY);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_DISPLAY", &flag);

					// D3D12_FORMAT_SUPPORT1_CAST_WITHIN_BIT_LAYOUT
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_CAST_WITHIN_BIT_LAYOUT);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_CAST_WITHIN_BIT_LAYOUT", &flag);

					// D3D12_FORMAT_SUPPORT1_MULTISAMPLE_RENDERTARGET
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_MULTISAMPLE_RENDERTARGET);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_MULTISAMPLE_RENDERTARGET", &flag);

					// D3D12_FORMAT_SUPPORT1_MULTISAMPLE_LOAD
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_MULTISAMPLE_LOAD);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_MULTISAMPLE_LOAD", &flag);

					// D3D12_FORMAT_SUPPORT1_SHADER_GATHER
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_SHADER_GATHER);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_SHADER_GATHER", &flag);

					// D3D12_FORMAT_SUPPORT1_BACK_BUFFER_CAST
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_BACK_BUFFER_CAST);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_BACK_BUFFER_CAST", &flag);

					// D3D12_FORMAT_SUPPORT1_TYPED_UNORDERED_ACCESS_VIEW
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_TYPED_UNORDERED_ACCESS_VIEW);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_TYPED_UNORDERED_ACCESS_VIEW", &flag);

					// D3D12_FORMAT_SUPPORT1_SHADER_GATHER_COMPARISON
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_SHADER_GATHER_COMPARISON);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_SHADER_GATHER_COMPARISON", &flag);

					// D3D12_FORMAT_SUPPORT1_DECODER_OUTPUT
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_DECODER_OUTPUT);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_DECODER_OUTPUT", &flag);

					// D3D12_FORMAT_SUPPORT1_DECODER_OUTPUT
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_VIDEO_PROCESSOR_OUTPUT);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_VIDEO_PROCESSOR_OUTPUT", &flag);

					// D3D12_FORMAT_SUPPORT1_DECODER_OUTPUT
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_VIDEO_PROCESSOR_INPUT);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_VIDEO_PROCESSOR_INPUT", &flag);

					// D3D12_FORMAT_SUPPORT1_DECODER_OUTPUT
					flag = (m_fsSupport1 & D3D12_FORMAT_SUPPORT1_VIDEO_ENCODER);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT1_VIDEO_ENCODER", &flag);

					// D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_ADD
					flag = (m_fsSupport2 & D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_ADD);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_ADD", &flag);

					// D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_BITWISE_OPS
					flag = (m_fsSupport2 & D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_BITWISE_OPS);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_BITWISE_OPS", &flag);

					// D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_COMPARE_STORE_OR_COMPARE_EXCHANGE
					flag = (m_fsSupport2 & D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_COMPARE_STORE_OR_COMPARE_EXCHANGE);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_COMPARE_STORE_OR_COMPARE_EXCHANGE", &flag);

					// D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_EXCHANGE
					flag = (m_fsSupport2 & D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_EXCHANGE);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_EXCHANGE", &flag);

					// D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_SIGNED_MIN_OR_MAX
					flag = (m_fsSupport2 & D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_SIGNED_MIN_OR_MAX);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_SIGNED_MIN_OR_MAX", &flag);

					// D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_UNSIGNED_MIN_OR_MAX
					flag = (m_fsSupport2 & D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_UNSIGNED_MIN_OR_MAX);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_UNSIGNED_MIN_OR_MAX", &flag);

					// D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD
					flag = (m_fsSupport2 & D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD", &flag);

					// D3D12_FORMAT_SUPPORT2_UAV_TYPED_STORE
					flag = (m_fsSupport2 & D3D12_FORMAT_SUPPORT2_UAV_TYPED_STORE);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT2_UAV_TYPED_STORE", &flag);

					// D3D12_FORMAT_SUPPORT2_OUTPUT_MERGER_LOGIC_OP
					flag = (m_fsSupport2 & D3D12_FORMAT_SUPPORT2_OUTPUT_MERGER_LOGIC_OP);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT2_OUTPUT_MERGER_LOGIC_OP", &flag);

					// D3D12_FORMAT_SUPPORT2_TILED
					flag = (m_fsSupport2 & D3D12_FORMAT_SUPPORT2_TILED);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT2_TILED", &flag);

					// D3D12_FORMAT_SUPPORT2_MULTIPLANE_OVERLAY
					flag = (m_fsSupport2 & D3D12_FORMAT_SUPPORT2_MULTIPLANE_OVERLAY);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT2_MULTIPLANE_OVERLAY", &flag);

					// D3D12_FORMAT_SUPPORT2_SAMPLER_FEEDBACK
					flag = (m_fsSupport2 & D3D12_FORMAT_SUPPORT2_SAMPLER_FEEDBACK);
					ImGui::Checkbox("D3D12_FORMAT_SUPPORT2_SAMPLER_FEEDBACK", &flag);

					// End of tab 
					ImGui::EndTabItem();
				}
				
				// End Tab
                ImGui::EndTabBar();
            }

			// End window
			ImGui::End();
		} 
	} else {
		m_bShowSerial = FALSE;
	}
	#endif
}

void UGF12::DebugUI::UI_SysInfo::setEnable(BOOL enabled) {
	m_bVisible = enabled;
}

BOOL UGF12::DebugUI::UI_SysInfo::getEnable() {
	return m_bVisible;
}
