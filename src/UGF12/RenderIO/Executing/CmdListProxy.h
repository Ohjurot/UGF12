#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>

#include <UGF12/DirectX/XCmdList.h>

#include <UGF12/RenderIO/Executing/CmdListExecutionFlag.h>
#include <UGF12/RenderIO/Executing/CmdListManager.h>

namespace GxRenderIO {

	/// <summary>
	/// Proxy for accessing a command list
	/// </summary>
	class CmdListProxy {
		public:
			/// <summary>
			/// Create proxy
			/// </summary>
			/// <param name="ptrManger">Pointer to manager</param>
			CmdListProxy(GxRenderIO::CmdListManger* ptrManger);

			// Destructor to free resources
			~CmdListProxy() noexcept(false);

			/// <summary>
			/// Execute the current command list (Objects remains valid!)
			/// </summary>
			/// <param name="disptachMode">Dispatch mode (default the operation is required this frame)</param>
			/// <param name="ptrFlag">Pointer to a flag to be set on completion (only valid on DisptachMode: RESULT_REQUIRED_LATER)</param>
			/// <returns>If function executed like expected (will also succeed on false - but has done execute in line or nothing)</returns>
			BOOL execute(GxRenderIO::CmdListManger::DispatchMode disptachMode = GxRenderIO::CmdListManger::DispatchMode::RESULT_REQUIRED_THIS_FRAME, GxRenderIO::CmdListExecutionFlag* ptrFlag = nullptr);

			/// <summary>
			/// Retrive the command list pointer
			/// </summary>
			/// <returns>Pointer to command list (D3D)</returns>
			ID3D12GraphicsCommandList* get();

			/// <summary>
			/// Returns if the proxy is dirty
			/// </summary>
			/// <returns></returns>
			BOOL isDirty();

			// Delete unused
			CmdListProxy(const CmdListProxy&) = delete;
			void operator=(const CmdListProxy&) = delete;
		private:
			/// <summary>
			/// Manger who is responsible for this command list
			/// </summary>
			GxRenderIO::CmdListManger* m_ptrManager;

			/// <summary>
			/// Current command list
			/// </summary>
			GxDirect::XCmdList* m_ptrCmdList;

			/// <summary>
			/// Bool to store if command is dirty (has commands on it)
			/// </summary>
			BOOL m_bDirty = FALSE;
	};
}