#include "pch.h"
#include "IWbemProvider.h"

// Singelton instance
GxUtil::IWebmProvider* GxUtil::IWebmProvider::s_ptrInstance = NULL;

GxUtil::IWebmProvider* GxUtil::IWebmProvider::getInstance() {
    // Check pointer
    if (!s_ptrInstance) {
        throw EXEPTION(L"GxUtil::IWebmProvider has not been initialized!");
    }

    // Return instance
    return s_ptrInstance;
}

void GxUtil::IWebmProvider::Init() {
    // Destroy if existing
    Destroy();

    // Create new
    s_ptrInstance = new GxUtil::IWebmProvider();
}

void GxUtil::IWebmProvider::Destroy() {
    // Check if existing
    if (s_ptrInstance) {
        delete s_ptrInstance;
    }
}

HRESULT GxUtil::IWebmProvider::queryValue(std::wstring className, std::wstring valueName, VARIANT* ptrValue) {
    HRESULT hr;

    // Check if cache miss
    if (wcscmp(valueName.c_str(), m_strRootCim2CacheClass.c_str()) != 0) {
        // Enum object
        IEnumWbemClassObject* ptrEnum = NULL;
        
        // Build WQL query
        std::wstringstream wss;
        wss << "SELECT * FROM " << className;

        // Enum for query
        if (FAILED(hr = m_ptrService->ExecQuery(bstr_t(L"WQL"), bstr_t(wss.str().c_str()), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &ptrEnum))) {
            return hr;
        }

        // Get instance
        ULONG returnValue = 0;
        if (FAILED(hr = ptrEnum->Next(WBEM_INFINITE, 1, &m_ptrRootCim2Cache, &returnValue))) {
            COM_RELEASE(ptrEnum);
            return hr;
        }

        // Check if exists
        if (returnValue == 0) {
            return E_FAIL;
        }

        // Set cache
        m_strRootCim2CacheClass = className;

        // Release objects
        COM_RELEASE(ptrEnum);
    }

    // Get object
    if (FAILED(hr = m_ptrRootCim2Cache->Get(valueName.c_str(), 0, ptrValue, 0, 0))) {
        return hr;
    }

    // OK
    return S_OK;
}

GxUtil::IWebmProvider::IWebmProvider() {
    HRESULT hr;

    // Get Locator for WMI
    if (FAILED(hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_ptrLocator)))) {
        throw EXEPTION_HR(L"CoCreateInstance(...) for CLSID_WbemLocator", hr);
    }

    // Connect to service
    if (FAILED(hr = m_ptrLocator->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &m_ptrService))) {
        throw EXEPTION_HR(L"IWbemLocator->ConnectServer(...) on ROOT\\CIMV2", hr);
    }

    // Set proxy
    if(FAILED(hr = CoSetProxyBlanket(m_ptrService, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_DEFAULT))) {
        throw EXEPTION_HR(L"CoSetProxyBlanket(...) for IWbemServices", hr);
    }
}

GxUtil::IWebmProvider::~IWebmProvider() {
    // Release cache
    COM_RELEASE(m_ptrRootCim2Cache);

    // Release Service
    COM_RELEASE(m_ptrService);

    // Release Locator
    COM_RELEASE(m_ptrLocator);
}
