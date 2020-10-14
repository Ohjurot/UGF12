#include "pch.h"
#include "CmdListManager.h"

GxRenderIO::CmdListManger::CmdListManger(GxDirect::XContext* ptrContext, GxDirect::XCmdQueue* ptrCommandQue, UINT commandListCount) :
	m_clPoolIdle(commandListCount, TRUE, ptrContext, ptrCommandQue),
	m_clPoolFrame(commandListCount, FALSE),
	m_ptrCmdQue(ptrCommandQue)
{ 
	// Ref counting
	m_ptrCmdQue->IncRef();

	// Create local array
	m_ptrExecuteArray = (ID3D12CommandList**)malloc(sizeof(ID3D12CommandList*) * commandListCount);
}

GxRenderIO::CmdListManger::~CmdListManger(){
	// Ref counting
	m_ptrCmdQue->DecRef();

	// Check state
	if (m_psFramePoolState == FramePoolState::STATE_EXECUTING) {
		// Wait
		waitForCommandLists();
	}

	// Cleanup array
	free(m_ptrExecuteArray);
	
	// Cleanup of objects is done automaticly in objects destructor
}

BOOL GxRenderIO::CmdListManger::getFirstList(GxDirect::XCmdList** ppCmdList) {
	return m_clPoolIdle.pull(ppCmdList);
}

BOOL GxRenderIO::CmdListManger::swapExecute(GxDirect::XCmdList** ppCmdList, GxRenderIO::CmdListManger::DispatchMode disptachMode, GxRenderIO::CmdListExecutionFlag* ptrFlag) {
	// Check state of list
	if (m_psFramePoolState == FramePoolState::STATE_EXECUTING) {
		// If not throw exeption (Out of command lists)
		throw EXEPTION_HR(L"Call on GxRenderIO::CmdListManger::swapExecute(...) is not expected while frame is in flight", E_ABORT);
	}
	
	// Get List
	GxDirect::XCmdList* ptrList = *ppCmdList;

	// Set new list
	if (!m_clPoolIdle.pull(ppCmdList)){
		// If not throw exeption (Out of command lists)
		throw EXEPTION_HR(L"Out of CommandLists", E_ABORT);
	}

	// Switch on execution mode
	switch (disptachMode) {
		// List result is required is this frame (stage in frame pool)
		case DispatchMode::RESULT_REQUIRED_THIS_FRAME:
			{
				// Return the result of this push
				return m_clPoolFrame.push(ptrList);
			}
			break;

		// List result is required now (execute and block until finished)
		case DispatchMode::RESULT_REQUIRED_NOW:
			{
				// Execute list and wait
				ptrList->execute();
				ptrList->wait();

				// Push back on idle pool
				if (!m_clPoolIdle.push(ptrList)) {
					throw EXEPTION_HR(L"Idle Command List Pool was not able to manage own list!", E_ABORT);
				}
			}
			break;

		// Command list result is required later stage it on frame async pool
		case DispatchMode::RESULT_REQUIRED_LATER:
			{
				// Check pointer
				if (!ptrFlag) {
					throw EXEPTION_HR(L"Pointer to execution flag is invalid", E_INVALIDARG);
				}

				// Check if que is possible
				if (!GxRenderIO::AsyncCmdExecutor::queExecution(ptrList, &m_clPoolIdle, ptrFlag)) {
					// Execute now 
					ptrList->execute();
					ptrList->wait();
					ptrFlag->__setCompleted();

					// Push back on idle pool
					if (!m_clPoolIdle.push(ptrList)) {
						throw EXEPTION_HR(L"Idle Command List Pool was not able to manage own list!", E_ABORT);
					}
				}
			}
			break;
	}

	return 0;
}

BOOL GxRenderIO::CmdListManger::returnList(GxDirect::XCmdList* ptrList) {
	// Try to return a list to the idle pool
	return m_clPoolIdle.push(ptrList);
}

void GxRenderIO::CmdListManger::executeCommandLists(){
	// Check state of list
	if (m_psFramePoolState != FramePoolState::STATE_RECORD) {
		// If not throw exeption (Out of command lists)
		throw EXEPTION_HR(L"Call on GxRenderIO::CmdListManger::executeCommandLists(...) is not expected while frame is in flight", E_ABORT);
	}
	
	// Gain pointer access
	if (!m_clPoolFrame.getPointer_lock((GxDirect::XCmdList**) & m_ptrsFramesLists, &m_ptrFramesStockCount)) {
		throw EXEPTION_HR(L"Cannot lock coammnd list pool for current frame", E_ABORT);
	}

	// Build array
	for (UINT i = 0; i < *m_ptrFramesStockCount; i++) {
		m_ptrsFramesLists[i]->close();
		m_ptrExecuteArray[i] = m_ptrsFramesLists[i]->get(FALSE);
	}

	// Execute command lists
	m_uiExecutionValue = m_ptrCmdQue->execute(m_ptrExecuteArray, *m_ptrFramesStockCount);

	// Set state
	m_psFramePoolState = FramePoolState::STATE_EXECUTING;
}

void GxRenderIO::CmdListManger::waitForCommandLists(){	
	// Wait for command list
	m_ptrCmdQue->waitForValue(m_uiExecutionValue);

	if (m_psFramePoolState == FramePoolState::STATE_EXECUTING) {
		// Flush command lists and add to idle
		for (UINT i = 0; i < *m_ptrFramesStockCount; i++) {
			m_ptrsFramesLists[i]->flush();
			if (!m_clPoolIdle.push(m_ptrsFramesLists[i])) {
				throw EXEPTION_HR(L"Idle Command List Pool was not able to manage own list!", E_ABORT);
			}
		}

		// Set stock count to zero
		*m_ptrFramesStockCount = 0;

		// Give back controle
		m_clPoolFrame.getPointer_unlock();
	}

	// Set state
	m_psFramePoolState = FramePoolState::STATE_RECORD;
}

void GxRenderIO::CmdListManger::flush() {
	// If not running and has stock execute
	if (m_psFramePoolState == FramePoolState::STATE_RECORD && m_clPoolFrame.getUsage() > 0) {
		executeCommandLists();
	}
	
	// If running wait for execution
	if (m_psFramePoolState == FramePoolState::STATE_EXECUTING) {
		waitForCommandLists();
	}
}
