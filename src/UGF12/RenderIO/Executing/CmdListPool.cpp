#include "pch.h"
#include "CmdListPool.h"

GxRenderIO::CmdListPool::CmdListPool(UINT capacity, BOOL initLists, GxDirect::XContext* ptrContext, GxDirect::XCmdQueue* ptrQue) :
	m_uiCapacity(capacity)
{
	// Allocate memory
	m_ptrCommandLists = (GxDirect::XCmdList**)malloc(sizeof(GxDirect::XCmdList*) * capacity);

	// Check init pointers
	if (initLists && (!ptrContext || !ptrQue)) {
		throw EXEPTION_HR(L"If a command list pool should init the lists a context and que is required!", E_INVALIDARG);
	}

	// Init pool
	if (initLists) {
		// Call new for all command lists
		for (UINT i = 0; i < capacity; i++) {
			m_ptrCommandLists[i] = new GxDirect::XCmdList(ptrContext, ptrQue);
		}

		// Pool is full
		m_uiStock = capacity;
	}
	else {
		// Pool is empty
		m_uiStock = 0;
	}
}

GxRenderIO::CmdListPool::~CmdListPool() {
	// Delete all list currently in pool
	for (UINT i = 0; i < m_uiStock; i++) {
		delete m_ptrCommandLists[i];
	}

	// Free memory
	free(m_ptrCommandLists);
}

BOOL GxRenderIO::CmdListPool::pull(GxDirect::XCmdList** ppCmdList){
	// Guard function
	GxUtil::LockGuard janitor(&m_lock);

	// Check stock
	if (m_uiStock == 0) {
		return FALSE;
	}

	// Set pointer 
	*ppCmdList = m_ptrCommandLists[m_uiStock - 1];

	// Decrement stock
	m_uiStock--;

	// Return true
	return TRUE;
}

BOOL GxRenderIO::CmdListPool::push(GxDirect::XCmdList* ptrCmdList) {
	// Guard function
	GxUtil::LockGuard janitor(&m_lock);

	// Check capcity
	if (m_uiCapacity == m_uiStock) {
		return FALSE;
	}

	// Add to list
	m_ptrCommandLists[m_uiStock] = ptrCmdList;
	
	// Increment stock
	m_uiStock++;

	// Return true
	return TRUE;
}

UINT GxRenderIO::CmdListPool::getUsage() {
	return m_uiStock;
}

UINT GxRenderIO::CmdListPool::getPointer_lock(GxDirect::XCmdList** ppArrayBegin, UINT** ppUiStock) {
	// Check parameters
	if (!ppArrayBegin || !ppUiStock) {
		throw EXEPTION_HR(L"GxRenderIO::CmdListPool::getPointer_lock(...)", E_INVALIDARG);
	}

	// Try aquire
	if (!m_lock.tryAquire()) {
		return 0;
	}

	// Set external pointers
	*ppArrayBegin = (GxDirect::XCmdList*)m_ptrCommandLists;
	*ppUiStock = &m_uiStock;

	// Return capcity
	return m_uiCapacity;
}

void GxRenderIO::CmdListPool::getPointer_unlock() {
	// Rlease lock
	m_lock.release();
}
