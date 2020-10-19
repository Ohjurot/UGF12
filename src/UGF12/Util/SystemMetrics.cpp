#include "pch.h"
#include "SystemMetrics.h"

GxUtil::SystemMetrics GxUtil::SystemMetrics::s_instance = GxUtil::SystemMetrics();

GxUtil::SystemMetrics::CpuCache GxUtil::SystemMetrics::getCpuCache(UINT index) {
    // Check bounds
    if (index >= s_instance.m_vecCaches.size()) {
        throw EXEPTION(L"Index out of range!");
    }

    // Return catch
    return s_instance.m_vecCaches[index];
}

UINT64 GxUtil::SystemMetrics::getSystemMemory() {
    return s_instance.m_uiSystemMemory;
}

GxUtil::SystemMetrics::SystemMetrics() {
    
    
    // Default values
    m_uiCpuCoresCount = 0;
    m_uiCpuLogicalCoresCount = 0;
    m_uiCpuCount = 1;
    m_uiSystemMemory = 0;

    // Get system memory
    GetPhysicallyInstalledSystemMemory(&m_uiSystemMemory);
    
    // Create buffer
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION* ptrProcessorInfos = NULL;
    DWORD dwProcessorInfosSize = 0;
    
    // Get required buffer size
    if (GetLogicalProcessorInformation(ptrProcessorInfos, &dwProcessorInfosSize) == FALSE) {
        // Allocate buffer
        UINT infoCount = dwProcessorInfosSize / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        ptrProcessorInfos = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*)malloc(dwProcessorInfosSize);

        // Fill buffer
        if (ptrProcessorInfos && GetLogicalProcessorInformation(ptrProcessorInfos, &dwProcessorInfosSize) == TRUE) {
            // Buffers for for loop
            UINT uiNumaNodes = 0;
            UINT uiPackageCount = 0;

            UINT uiProcesorCount = 0;
            UINT uiLogicalProcessorCount = 0;

            // For each info
            for (UINT i = 0; i < infoCount; i++) {
                // Check node type
                switch (ptrProcessorInfos[i].Relationship) {
                    // Pro processor
                    case RelationProcessorCore:
                        // Increment Physical
                        uiProcesorCount++;
                        // Increment Logical by set bits in mask
                        uiLogicalProcessorCount += __popcnt64(ptrProcessorInfos[i].ProcessorMask);

                        break;
                    
                    // For cache
                    case RelationCache:
                        {
                            // Try to find matching cache
                            BOOL found = FALSE;
                            for (UINT j = 0; j < m_vecCaches.size(); j++) {
                                // If level and type match
                                if (m_vecCaches[j].level == ptrProcessorInfos[i].Cache.Level && m_vecCaches[j].type == ptrProcessorInfos[i].Cache.Type) {
                                    // Increment count
                                    m_vecCaches[j].count++;
                                    found = TRUE;
                                }
                            }

                            // Insert new if non found and has space
                            if (!found) {
                                // Setup CpuCache
                                SystemMetrics::CpuCache cc;
                                cc.level = ptrProcessorInfos[i].Cache.Level;
                                cc.type = ptrProcessorInfos[i].Cache.Type;
                                cc.size = ptrProcessorInfos[i].Cache.Size;
                                cc.count = 1;
                                
                                // Copy to array
                                m_vecCaches.push_back(cc);
                            }
                        }
                        break;

                    case RelationProcessorPackage:
                        // Increment Package
                        uiPackageCount++;
                        break;

                    default:
                        // ....
                        break;
                }
            }

            // Coyp to internal
            m_uiCpuCoresCount = uiProcesorCount;
            m_uiCpuLogicalCoresCount = uiLogicalProcessorCount;
            m_uiCpuCount = uiPackageCount;

            // Create strings
            for (UINT i = 0; i < m_vecCaches.size(); i++) {
                // Start size
                UINT size = m_vecCaches[i].size;
                UINT divisionsCount = 0;

                // While is bigger than 1024
                while (size >= 1024) {
                    // Devide down one
                    size = size / 1024;
                    // Set division count
                    divisionsCount++;
                }

                // Translate to string
                std::stringstream ss;
                ss << size;

                // Check for uint (very very future prove!)
                switch (divisionsCount) {
                    case 0:
                        ss << " Bytes";
                        break;
                    case 1:
                        ss << " KB";
                        break;
                    case 2:
                        ss << " MB";
                        break;
                    case 3:
                        ss << " GB";
                        break;
                    case 4:
                        ss << " TB";
                        break;
                    case 5:
                        ss << " PB";
                        break;
                }

                // Set text
                m_vecCaches[i].translatedString = ss.str();
            }
        }

        // Free buffer
        free(ptrProcessorInfos);
    }
}

UINT GxUtil::SystemMetrics::getCpuCount() {
    return s_instance.m_uiCpuCount;
}

UINT GxUtil::SystemMetrics::getCpuCoresCount(){
    // Return core count
    return s_instance.m_uiCpuCoresCount;
}

UINT GxUtil::SystemMetrics::getCpuLogicalCoreCount() {
    // Return logical core count
    return s_instance.m_uiCpuLogicalCoresCount;
}

UINT GxUtil::SystemMetrics::getCacheCount() {
    // Return caches count
    return s_instance.m_vecCaches.size();
}
