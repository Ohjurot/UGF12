#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>
#include <UGF12/Util/FileSystem/IFileSystemImpl.h>

namespace GxUtil {
	namespace FS {
		/// <summary>
		/// File system container
		/// </summary>
		class FileSystem {
			public:
				/// <summary>
				/// Default constructor (defect file system)
				/// </summary>
				FileSystem();

				/// <summary>
				/// File system for initial path and implementation
				/// </summary>
				/// <param name="ptrFileSystemImpl">Pointer to implementation</param>
				/// <param name="basePath">Base directory in iplementation</param>
				FileSystem(GxUtil::FS::IFileSystemImpl* ptrFileSystemImpl, LPCWSTR basePath);

				/// <summary>
				/// File system ontop on aditional filesystem
				/// </summary>
				/// <param name="ptrParentFileSystem">Pointer to old file sytem</param>
				/// <param name="subPath">Sub path</param>
				FileSystem(GxUtil::FS::FileSystem* ptrParentFileSystem, LPCWSTR subPath);

				/// <summary>
				/// Get the file systems impementation
				/// </summary>
				/// <returns>Implementation</returns>
				GxUtil::FS::IFileSystemImpl* getImpl();

				/// <summary>
				/// Create path for this filesystem.
				/// </summary>
				/// <param name="file">File starting without a slash and uses general backslashen</param>
				/// <returns>Combined path for fs root directory. Only valid until next call to this function</returns>
				LPCWSTR createPath(LPCWSTR file);

			private:
				/// <summary>
				/// File system implementation
				/// </summary>
				GxUtil::FS::IFileSystemImpl* m_ptrImpl;

				/// <summary>
				/// Buffer for base directory
				/// </summary>
				WCHAR m_strBasePath[MAX_PATH];

				/// <summary>
				/// Length of base path
				/// </summary>
				SIZE_T m_uiPathLength;
		};
	}
}