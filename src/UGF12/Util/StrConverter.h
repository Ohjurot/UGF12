#pragma once
#include "pch.h"

namespace GxUtil {
	class StrConverter {
		public:
			/// <summary>
			/// Convert a narraw to wide string
			/// </summary>
			/// <param name="input">Input narrow string</param>
			/// <returns>New wide string</returns>
			static std::wstring ConvertN2W(std::string input);

			/// <summary>
			/// Convert wide to narrow string
			/// </summary>
			/// <param name="input">Input wide string</param>
			/// <param name="errorChar">Character to use when no narraw char is avalible</param>
			/// <returns>New narrow string</returns>
			static std::string ConvertW2N(std::wstring input);

			/// <summary>
			/// Translate a hresult to a wide string
			/// </summary>
			/// <param name="hr">Input hresult</param>
			/// <returns>HRESULTs message as wide string</returns>
			static std::wstring TranslateHRESULT(HRESULT hr);

			/// <summary>
			/// Convert a memory integer (base 1024) to a string
			/// </summary>
			/// <param name="memory">Input amount of memory</param>
			/// <returns>Ouptut string</returns>
			static std::string memoryToString(UINT64 memory);
	};
}