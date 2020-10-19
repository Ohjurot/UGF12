#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>

namespace GxUtil {
	/// <summary>
	/// Singelton IWebmProvider
	/// </summary>
	class IWebmProvider {
		public:
			/// <summary>
			/// Get the Singelton instance
			/// </summary>
			/// <returns></returns>
			static IWebmProvider* getInstance();

			/// <summary>
			/// Init Singelton
			/// </summary>
			static void Init();
			
			/// <summary>
			/// Destroy Singelton
			/// </summary>
			static void Destroy();

		public:
			/// <summary>
			/// Query value on ROOT\CIMV2 (will always use first instance)
			/// </summary>
			/// <param name="className">Name of the class to be queryed</param>
			/// <param name="valueName">Name of value to be retrived</param>
			/// <param name="ptrValue">Pointe to variant to be set</param>
			/// <returns>Query was successfull</returns>
			HRESULT queryValue(std::wstring className, std::wstring valueName, VARIANT* ptrValue);

		private:
			/// <summary>
			/// IWbem locator
			/// </summary>
			IWbemLocator* m_ptrLocator = NULL;

			/// <summary>
			/// WMI Service at ROOT\CIMV2
			/// </summary>
			IWbemServices* m_ptrService = NULL;

			/// <summary>
			/// Cache object
			/// </summary>
			IWbemClassObject* m_ptrRootCim2Cache = NULL;

			/// <summary>
			/// Cached class name
			/// </summary>
			std::wstring m_strRootCim2CacheClass = L"";

		private:
			/// <summary>
			/// Singelton construct
			/// </summary>
			IWebmProvider();

			// Destruct
			~IWebmProvider();

			/// <summary>
			/// Singelton instance
			/// </summary>
			static IWebmProvider* s_ptrInstance;
	};
}