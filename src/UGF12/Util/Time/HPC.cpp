#include "pch.h"
#include "HPC.h"

// Declare s_instance
GxUtil::HPC GxUtil::HPC::s_instance;

GxUtil::TIMESTAMP GxUtil::HPC::queryCounter() {
    // Query current time
    GxUtil::TIMESTAMP now;
    if (!QueryPerformanceCounter((LARGE_INTEGER*)&now)) {
        throw EXEPTION_HR(L"QueryPerformanceCounter(...)", GetLastError());
    }

    // Calculate away freqeuncy
    now *= 1000000;
    now /= s_instance.m_iTimerFrequency;

    // Return calculated time
    return now;
}

GxUtil::HPC::HPC() {
    // Query frequency
    if (!QueryPerformanceFrequency((LARGE_INTEGER*)&m_iTimerFrequency)) {
        throw EXEPTION_HR(L"QueryPerformanceFrequency(...)", GetLastError());
    }
}
