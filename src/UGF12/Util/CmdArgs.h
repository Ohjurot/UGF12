#pragma once
#include "pch.h"

namespace GxUtil {
	/// <summary>
	/// Class witch stores cmd args with the format -cmd=value or -cmd="Value with space" to an key value pair of wstrings
	/// </summary>
	class CmdArgs {
		public:
			/// <summary>
			/// Create a CmdArgs object by passing in the winapi PWSTR pointer
			/// </summary>
			/// <param name="cmdLine">Application commaned line unicode</param>
			CmdArgs(LPWSTR cmdLine);

			/// <summary>
			/// Get a value for key
			/// </summary>
			/// <param name="key">Retrive a value for a given key</param>
			/// <returns></returns>
			std::wstring operator[](std::wstring key);

			/// <summary>
			/// Check if a key exists in the arguments
			/// </summary>
			/// <param name="key">Input key</param>
			/// <returns>If the key exists</returns>
			BOOL exitst(std::wstring key);

			// Delete unused
			void operator=(const CmdArgs&) = delete;
			CmdArgs(const CmdArgs&) = delete;
		private:
			/// <summary>
			/// Map witch stores the key value pairs
			/// </summary>
			std::map<std::wstring, std::wstring> m_mapKeyValue;

	};
}