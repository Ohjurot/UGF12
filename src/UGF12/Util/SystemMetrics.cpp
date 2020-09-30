#include "pch.h"
#include "SystemMetrics.h"

GxUtil::SystemMetrics GxUtil::SystemMetrics::s_instance = GxUtil::SystemMetrics();

GxUtil::SystemMetrics::SystemMetrics() {
    // Get empty system info
    SYSTEM_INFO sysInfo;
    ZeroMemory(&sysInfo, sizeof(SYSTEM_INFO));

    // Query systeminfo
    GetSystemInfo(&sysInfo);

    // Get cpu count
    m_uiCpuCoresCount = sysInfo.dwNumberOfProcessors;
}

UINT GxUtil::SystemMetrics::getCpuCoresCount(){
    // Return core count
    return s_instance.m_uiCpuCoresCount;
}
