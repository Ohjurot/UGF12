#pragma once
#include "pch.h"

#define UGF12_FILE_OPEN_READ		1U
#define UGF12_FILE_OPEN_WRITE		2U
#define UGF12_FILE_OPEN_EXISTING	4U
#define UGF12_FILE_OPEN_NEW			8U
#define UGF12_FILE_OPEN_ALLWAYS		16U


#define UGF12_FILE_SET_FROM_START	0U
#define UGF12_FILE_SET_RELATIV		1U
#define UGF12_FILE_SET_FROM_END		2U


namespace UGF12 {
	/// <summary>
	/// A File is UINT64 id value
	/// </summary>
	typedef UINT64 HFile;
}

namespace GxUtil {
	namespace FS {
		/// <summary>
		/// Interface for filesystem implementations
		/// </summary>
		class IFileSystemImpl {
			public:
				/// <summary>
				/// Open a file
				/// </summary>
				/// <param name="path"Path to the file></param>
				/// <param name="openFlags">Flags for opening the file UGF12_FILE_OPEN_XXXXX</param>
				/// <returns>Handle to file</returns>
				virtual UGF12::HFile openFile(LPCWSTR path, DWORD openFlags) = 0;
				
				/// <summary>
				/// Close a file
				/// </summary>
				/// <param name="file">Handle to file</param>
				virtual void closeFile(UGF12::HFile file) = 0;

				/// <summary>
				/// Retrive the size of a file
				/// </summary>
				/// <param name="file">Handle to file</param>
				/// <returns>Size of the file</returns>
				virtual SIZE_T getFileSize(UGF12::HFile file) = 0;

				/// <summary>
				/// Set the file pointer
				/// </summary>
				/// <param name="file">Handle to file</param>
				/// <param name="mode">Set mode UGF12_FILE_SET_XXXX</param>
				/// <param name="value">Index to offset file</param>
				/// <returns></returns>
				virtual void setFilePointer(UGF12::HFile file, WORD mode, SIZE_T value) = 0;

				/// <summary>
				/// Read a file
				/// </summary>
				/// <param name="file">Handle to file</param>
				/// <param name="numberOfBytesToRead">Number of bytes desired to read</param>
				/// <param name="ptrBuffer">Pointer to read file to</param>
				/// <returns>Bytes actualy read</returns>
				virtual DWORD readFile(UGF12::HFile file, DWORD numberOfBytesToRead, void* ptrBuffer) = 0;

				/// <summary>
				/// Write to a file
				/// </summary>
				/// <param name="file">Handle to file</param>
				/// <param name="numerOfByteToWrite">Number of byte to be written</param>
				/// <param name="ptrBuffer">Pointer to buffer</param>
				/// <returns>Number of bytes written</returns>
				virtual DWORD writeFile(UGF12::HFile file, DWORD numerOfByteToWrite, void* ptrBuffer) = 0;
		};
	}
}