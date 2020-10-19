#include "pch.h"
#include "CPUID.h"

bool GxUtil::CPUID_Factory::queryCpu(CPUID_RESULT* ptrResultStruct) {
    // Check for nullpointer
    if (ptrResultStruct == nullptr)
        return false;

    // Start of CPUID call process

    // Create buffer for CPUID / CPUIDEX calls
    int cpuInfoBuffer[4];

    // Read cpuid 0x0 for information about number of function ids
    CPUID(cpuInfoBuffer, 0x0);
    const int idsCount = cpuInfoBuffer[0];

    // Create big / variable buffer for storing result data
    int* cpuInfoBufferVar = (int*)malloc((sizeof(int) * 4) * (idsCount + 1));

    // Query cpuid for every id
    for (__int32 i = 0; i <= idsCount; i++) {
        CPUIDEX(&cpuInfoBufferVar[i * 4], i, 0);
    }

    // Create buffer for vendor
    char vendor[0x20];
    ZeroMemory(vendor, 0x20);

    // Read vendor from cpuid query
    *reinterpret_cast<int*>(vendor + 0) = cpuInfoBufferVar[(0 * 4) + 1];
    *reinterpret_cast<int*>(vendor + 4) = cpuInfoBufferVar[(0 * 4) + 3];
    *reinterpret_cast<int*>(vendor + 8) = cpuInfoBufferVar[(0 * 4) + 2];
    ptrResultStruct->vendorString = std::string(vendor);

    // Check if vendor matched AMD or Intel
    if (ptrResultStruct->vendorString == "GenuineIntel") {
        ptrResultStruct->vendor = CPU_VENDOR::INTEL;
    }
    else if (ptrResultStruct->vendorString == "AuthenticAMD") {
        ptrResultStruct->vendor = CPU_VENDOR::AMD;
    }
    // No else needed because the pointers date should have been cleared to zero bevore this methods call

    // Load F1 register if exists
    if (idsCount >= 1) {
        ptrResultStruct->regF1_ECX = cpuInfoBufferVar[(1 * 4) + 2];
        ptrResultStruct->regF1_EDX = cpuInfoBufferVar[(1 * 4) + 3];
    }

    // Load F7 register if exists
    if (idsCount >= 7) {
        ptrResultStruct->regF7_EBX = cpuInfoBufferVar[(7 * 4) + 1];
        ptrResultStruct->regF7_ECX = cpuInfoBufferVar[(7 * 4) + 2];
    }

    // Read cpuid 0x80000000 for information about number of extended function ids 
    CPUID(cpuInfoBuffer, 0x80000000);
    const int idsExMax = cpuInfoBuffer[0];

    // Reallocate cpuInfoBufferVar for use with ex query
    cpuInfoBufferVar = (int*)realloc(cpuInfoBufferVar, (sizeof(int) * 4) * ((unsigned __int32)idsExMax - (unsigned __int32)0x80000000));

    // Query cpu ex info
    for (int i = 0x80000000; i < idsExMax; i++) {
        CPUIDEX(&cpuInfoBufferVar[(i - 0x80000000) * 4], i, 0);
    }

    // Create buffer for brand name
    char brand[0x40];
    ZeroMemory(brand, 0x40);

    // Fill Brand buffer if brand name exists
    if (idsExMax >= 0x80000004) {
        memcpy(brand + 0, reinterpret_cast<void*>(&cpuInfoBufferVar[(2 * 4)]), 0x10);
        memcpy(brand + 16, reinterpret_cast<void*>(&cpuInfoBufferVar[(3 * 4)]), 0x10);
        memcpy(brand + 32, reinterpret_cast<void*>(&cpuInfoBufferVar[(4 * 4)]), 0x10);
        ptrResultStruct->brandString = std::string(brand);
    }

    // Load F81 register if exists
    if (idsExMax >= 0x80000001) {
        ptrResultStruct->regF81_ECX = cpuInfoBufferVar[(1 * 4) + 2];
        ptrResultStruct->regF81_EDX = cpuInfoBufferVar[(1 * 4) + 3];
    }

    // Free Buffer
    free(cpuInfoBufferVar);

    return TRUE;
}
