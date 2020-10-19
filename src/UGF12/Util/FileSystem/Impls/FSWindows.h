#pragma once
#include "pch.h"

#include <UGF12/Util/FileSystem/IFileSystemImpl.h>

namespace GxUtil {
	namespace FS {
		/// <summary>
		/// Windows file system
		/// </summary>
		class FS_Windows : public GxUtil::FS::IFileSystemImpl {
			public:
				/// <summary>
				/// Get the instance of the windows filesystem
				/// </summary>
				/// <returns></returns>
				static GxUtil::FS::IFileSystemImpl* getInstance();

			public:
				/// <summary>
				/// Open a file
				/// </summary>
				/// <param name="path"Path to the file></param>
				/// <param name="openFlags">Flags for opening the file UGF12_FILE_OPEN_XXXXX</param>
				/// <returns>Handle to file</returns>
				UGF12::File openFile(LPCWSTR path, WORD openFlags);

				/// <summary>
				/// Close a file
				/// </summary>
				/// <param name="file">Handle to file</param>
				void closeFile(UGF12::File file);

				/// <summary>
				/// Retrive the size of a file
				/// </summary>
				/// <param name="file">Handle to file</param>
				/// <returns>Size of the file</returns>
				SIZE_T getFileSize(UGF12::File file);

				/// <summary>
				/// Set the file pointer
				/// </summary>
				/// <param name="file">Handle to file</param>
				/// <param name="mode">Set mode UGF12_FILE_SET_XXXX</param>
				/// <param name="value">Index to offset file</param>
				/// <returns></returns>
				void setFilePointer(UGF12::File file, WORD mode, SIZE_T value);

				/// <summary>
				/// Read a file
				/// </summary>
				/// <param name="file">Handle to file</param>
				/// <param name="numberOfBytesToRead">Number of bytes desired to read</param>
				/// <param name="ptrBuffer">Pointer to read file to</param>
				/// <returns>Bytes actualy read</returns>
				DWORD readFile(UGF12::File file, DWORD numberOfBytesToRead, void* ptrBuffer);

				/// <summary>
				/// Write to a file
				/// </summary>
				/// <param name="file">Handle to file</param>
				/// <param name="numerOfByteToWrite">Number of byte to be written</param>
				/// <param name="ptrBuffer">Pointer to buffer</param>
				/// <returns>Number of bytes written</returns>
				DWORD writeFile(UGF12::File file, DWORD numerOfByteToWrite, void* ptrBuffer);

			private:
				/// <summary>
				/// Empty singelton constructor
				/// </summary>
				FS_Windows() {};

				/// <summary>
				/// Static instance
				/// </summary>
				static FS_Windows s_instance;
		};
	}
}