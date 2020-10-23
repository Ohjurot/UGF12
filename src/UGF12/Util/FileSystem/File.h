#pragma once
#include "pch.h"

#include <UGF12/Util/FileSystem/FileSystem.h>
#include <UGF12/Util/FileSystem/IFileSystemImpl.h>

namespace GxUtil {
	namespace FS {
		/// <summary>
		/// Represents a file in memory
		/// </summary>
		class File {
			public:
				/// <summary>
				/// Create file from fs and name
				/// </summary>
				/// <param name="ptrFileSystm">Input file system</param>
				/// <param name="fileName">Input file name</param>
				File(GxUtil::FS::FileSystem* ptrFileSystm, LPCWSTR fileName);

				// Destruct
				~File();

				/// <summary>
				/// Checks weather the file exists
				/// </summary>
				/// <returns>If the file exists</returns>
				BOOL exists();

				/// <summary>
				/// Open the file
				/// </summary>
				/// <param name="read">File read access</param>
				/// <param name="write">File write access</param>
				/// <param name="create">Create file if not existing</param>
				BOOL open(BOOL read = TRUE, BOOL write = FALSE, BOOL create = FALSE);

				/// <summary>
				/// Open the file
				/// </summary>
				/// <param name="flags">Input flags</param>
				/// <returns></returns>
				BOOL open(DWORD flags);

				/// <summary>
				/// Create the file
				/// </summary>
				/// <param name="keepOpen">Indicates if the file should be keept open</param>
				/// <returns>Result</returns>
				BOOL create(BOOL keepOpen = TRUE);

				/// <summary>
				/// Close the file
				/// </summary>
				void close();

				/// <summary>
				/// Check if the file is open
				/// </summary>
				/// <returns>If the file is open</returns>
				BOOL isOpen();

			private:
				/// <summary>
				/// File path buffer
				/// </summary>
				WCHAR m_filePathComplete[MAX_PATH];

				/// <summary>
				/// Pointer to the files filesystem impl
				/// </summary>
				GxUtil::FS::IFileSystemImpl* m_ptrImpl;

				/// <summary>
				/// Indicates if the file is open
				/// </summary>
				BOOL m_bFileOpen = FALSE;

				/// <summary>
				/// File Handle
				/// </summary>
				::UGF12::HFile m_hFile = 0x0;
		};
	}
}