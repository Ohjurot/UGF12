#pragma once
#include "pch.h"

#define CPUID(cpuInfo, function_id)						__cpuid(cpuInfo, function_id)
#define CPUIDEX(cpuInfo, function_id, subfunction_id)	__cpuidex(cpuInfo, function_id, subfunction_id)
#define CHECK_BIT(reg, bit)                             ((reg) & (1 << (bit)))

/****************************************************************************************
 * These files (CPUID.h and CPUID.cpp) where written using the Mircosoft dokumentation  *
 * This code here follows the microsoft example but has been adapted "to my c++ style"  *
 * The Microsoft dokumentation is release under CC BY 4.0                               *
 * https://docs.microsoft.com/de-de/cpp/intrinsics/cpuid-cpuidex?view=vs-2019           *
 * https://creativecommons.org/licenses/by/4.0/deed.de                                  *
 ****************************************************************************************/

namespace GxUtil {
    /// <summary>
    /// CPU Vendor
    /// </summary>
    enum class CPU_VENDOR {
        VM_OR_UNKNOWN = 0,
        INTEL = 1,
        AMD = 2
    };

    /// <summary>
    /// CPUID Query result
    /// </summary>
    struct CPUID_RESULT {
        /// <summary>
        /// CPU Vendor string
        /// </summary>
        std::string	vendorString;
        
        /// <summary>
        /// CPU Brand string
        /// </summary>
        std::string	brandString;
        
        /// <summary>
        /// CPU Vendor
        /// </summary>
        CPU_VENDOR vendor;

        unsigned __int32 regF1_ECX;
        unsigned __int32 regF1_EDX;
        unsigned __int32 regF7_EBX;
        unsigned __int32 regF7_ECX;
        unsigned __int32 regF81_ECX;
        unsigned __int32 regF81_EDX;

        bool SSE3() { return CHECK_BIT(regF1_ECX, 0); }
        bool PCLMULQDQ() { return CHECK_BIT(regF1_ECX, 1); }
        bool MONITOR() { return CHECK_BIT(regF1_ECX, 3); }
        bool SSSE3() { return CHECK_BIT(regF1_ECX, 9); }
        bool FMA() { return CHECK_BIT(regF1_ECX, 12); }
        bool CMPXCHG16B() { return CHECK_BIT(regF1_ECX, 13); }
        bool SSE41() { return CHECK_BIT(regF1_ECX, 19); }
        bool SSE42() { return CHECK_BIT(regF1_ECX, 20); }
        bool MOVBE() { return CHECK_BIT(regF1_ECX, 22); }
        bool POPCNT() { return CHECK_BIT(regF1_ECX, 23); }
        bool AES() { return CHECK_BIT(regF1_ECX, 25); }
        bool XSAVE() { return CHECK_BIT(regF1_ECX, 26); }
        bool OSXSAVE() { return CHECK_BIT(regF1_ECX, 27); }
        bool AVX() { return CHECK_BIT(regF1_ECX, 28); }
        bool F16C() { return CHECK_BIT(regF1_ECX, 29); }
        bool RDRAND() { return CHECK_BIT(regF1_ECX, 30); }

        bool MSR() { return CHECK_BIT(regF1_EDX, 5); }
        bool CX8() { return CHECK_BIT(regF1_EDX, 8); }
        bool SEP() { return CHECK_BIT(regF1_EDX, 11); }
        bool CMOV() { return CHECK_BIT(regF1_EDX, 15); }
        bool CLFSH() { return CHECK_BIT(regF1_EDX, 19); }
        bool MMX() { return CHECK_BIT(regF1_EDX, 23); }
        bool FXSR() { return CHECK_BIT(regF1_EDX, 24); }
        bool SSE() { return CHECK_BIT(regF1_EDX, 25); }
        bool SSE2() { return CHECK_BIT(regF1_EDX, 26); }

        bool FSGSBASE() { return CHECK_BIT(regF7_EBX, 0); }
        bool BMI1() { return CHECK_BIT(regF7_EBX, 3); }
        bool HLE() { return CHECK_BIT(regF7_EBX, 4) && vendor == CPU_VENDOR::INTEL; }
        bool AVX2() { return CHECK_BIT(regF7_EBX, 5); }
        bool BMI2() { return CHECK_BIT(regF7_EBX, 8); }
        bool ERMS() { return CHECK_BIT(regF7_EBX, 9); }
        bool INVPCID() { return CHECK_BIT(regF7_EBX, 10); }
        bool RTM() { return CHECK_BIT(regF7_EBX, 11) && vendor == CPU_VENDOR::INTEL; }
        bool AVX512F() { return CHECK_BIT(regF7_EBX, 16); }
        bool RDSEED() { return CHECK_BIT(regF7_EBX, 18); }
        bool ADX() { return CHECK_BIT(regF7_EBX, 19); }
        bool AVX512PF() { return CHECK_BIT(regF7_EBX, 26); }
        bool AVX512ER() { return CHECK_BIT(regF7_EBX, 27); }
        bool AVX512CD() { return CHECK_BIT(regF7_EBX, 28); }
        bool SHA() { return CHECK_BIT(regF7_EBX, 29); }

        bool PREFETCHWT1() { return CHECK_BIT(regF7_ECX, 0); }

        bool LAHF() { return CHECK_BIT(regF81_ECX, 0); }
        bool LZCNT() { return CHECK_BIT(regF81_ECX, 5) && vendor == CPU_VENDOR::INTEL; }
        bool ABM() { return CHECK_BIT(regF81_ECX, 5) && vendor == CPU_VENDOR::AMD; }
        bool SSE4a() { return CHECK_BIT(regF81_ECX, 6) && vendor == CPU_VENDOR::AMD; }
        bool XOP() { return CHECK_BIT(regF81_ECX, 11) && vendor == CPU_VENDOR::AMD; }
        bool TBM() { return CHECK_BIT(regF81_ECX, 21) && vendor == CPU_VENDOR::AMD; }

        bool SYSCALL() { return CHECK_BIT(regF81_EDX, 11) && vendor == CPU_VENDOR::INTEL; }
        bool MMXEXT() { return CHECK_BIT(regF81_EDX, 22) && vendor == CPU_VENDOR::AMD; }
        bool RDTSCP() { return CHECK_BIT(regF81_EDX, 27) && vendor == CPU_VENDOR::INTEL; }
        bool _3DNOWEXT() { return CHECK_BIT(regF81_EDX, 30) && vendor == CPU_VENDOR::AMD; }
        bool _3DNOW() { return CHECK_BIT(regF81_EDX, 31) && vendor == CPU_VENDOR::AMD; }
    };

    /// <summary>
    /// Static class for creating CPU ID Querys
    /// </summary>
    class CPUID_Factory {
        public:
            /// <summary>
            /// Function to create CPUID query
            /// </summary>
            /// <param name="ptrResultStruct">Pointer to resulting struct</param>
            /// <returns>if query was successfully</returns>
            static bool queryCpu(CPUID_RESULT* ptrResultStruct);
    };
}