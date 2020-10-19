#pragma once
#include "pch.h"

#include <UGF12/Util/FileSystem/FileSystem.h>
#include <UGF12/Util/FileSystem/Impls/FSWindows.h>

#define UGF12_GET_APP_FS()		GxUtil::FS::FSProvider::getAppDirFs()
#define UGF12_GET_WORKING_FS()	GxUtil::FS::FSProvider::getExecDirFs()
#define UGF12_APP_FS			GxUtil::FS::FSProvider::getAppDirFs()
#define UGF12_WORKING_FS		GxUtil::FS::FSProvider::getExecDirFs()

namespace GxUtil {
	namespace FS {
		class FSProvider {
			public:	
				/// <summary>
				/// Get execution dirs file system
				/// </summary>
				/// <returns></returns>
				static GxUtil::FS::FileSystem* getExecDirFs();

				/// <summary>
				/// Get application dirs file system
				/// </summary>
				/// <returns></returns>
				static GxUtil::FS::FileSystem* getAppDirFs();

			private:
				/// <summary>
				/// Singelton empy construct
				/// </summary>
				FSProvider();
				
				/// <summary>
				/// Singelton instance
				/// </summary>
				static FSProvider s_instance;

			private:
				/// <summary>
				/// Execution dir file system
				/// </summary>
				GxUtil::FS::FileSystem m_fsExec;

				/// <summary>
				/// Application dir
				/// </summary>
				GxUtil::FS::FileSystem m_fsApp;
		};
	}
}