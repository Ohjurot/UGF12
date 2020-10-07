#include "pch.h"
#include "CmdListProxy.h"

GxRenderIO::CmdListProxy::CmdListProxy(GxRenderIO::CmdListManger* ptrManger) :
	m_ptrManager(ptrManger)
{
	// Increment ref count
	m_ptrManager->IncRef();

	// Get first command list
	if (!m_ptrManager->getFirstList(&m_ptrCmdList)) {
		throw EXEPTION(L"Error retriving required command list from manager");
	}
}

GxRenderIO::CmdListProxy::~CmdListProxy() noexcept(false) {
	// If required execute command list
	if (m_bDirty) {
		execute();
	}
	
	// Release command list
	if (!m_ptrManager->returnList(m_ptrCmdList)) {
		throw EXEPTION(L"Was not able to return command list to manager!");
	}

	// Decrement ref count
	m_ptrManager->DecRef();
}

BOOL GxRenderIO::CmdListProxy::execute(GxRenderIO::CmdListManger::DispatchMode disptachMode, GxRenderIO::CmdListExecutionFlag* ptrFlag) {
	// Only if dirty
	if (m_bDirty) {
		if (!m_ptrManager->swapExecute(&m_ptrCmdList, disptachMode, ptrFlag)) {
			// Execute now
			m_ptrCmdList->execute();
			m_ptrCmdList->wait();

			// Return FALSE (unespected finish)
			return FALSE;
		}

		// Return TRUE (expected result)
		return TRUE;
	}

	// Return FALSE (unexpected result)
	return FALSE;
}

ID3D12GraphicsCommandList* GxRenderIO::CmdListProxy::get() {
	m_bDirty = TRUE;
	return m_ptrCmdList->get(FALSE);
}

BOOL GxRenderIO::CmdListProxy::isDirty() {
	return m_bDirty;
}
