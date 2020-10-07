#include "pch.h"
#include "AsyncCmdExecutor.h"

// Async executor is null initaly
GxRenderIO::AsyncCmdExecutor* GxRenderIO::AsyncCmdExecutor::s_ptrExecutor = nullptr;

// Lock is dafault
GxUtil::SpinLock GxRenderIO::AsyncCmdExecutor::s_lock;

BOOL GxRenderIO::AsyncCmdExecutor::queExecution(GxDirect::XCmdList* ptrCmdList, GxRenderIO::CmdListPool* ptrPool, GxRenderIO::CmdListExecutionFlag* ptrFlag) {
	// Guard pointer access
	GxUtil::LockGuard janitor(&GxRenderIO::AsyncCmdExecutor::s_lock);

	// Check if needs to be thrown
	if (!GxRenderIO::AsyncCmdExecutor::s_ptrExecutor) {
		throw EXEPTION(L"Tryed to call uninistialized AsyncCommandExecutor");
	}

	// Call
	return GxRenderIO::AsyncCmdExecutor::s_ptrExecutor->queCmdList(ptrCmdList, ptrPool, ptrFlag);
}

void GxRenderIO::AsyncCmdExecutor::init(UINT capacity) {
	// Check if needs to be thrown
	if (GxRenderIO::AsyncCmdExecutor::s_ptrExecutor) {
		throw EXEPTION(L"The AsyncCommandExecutor has alredy been inited");
	}

	// Create instance
	GxRenderIO::AsyncCmdExecutor::s_ptrExecutor = new GxRenderIO::AsyncCmdExecutor(capacity);
}

void GxRenderIO::AsyncCmdExecutor::destroy() {
	// Check if needs to be thrown
	if (!GxRenderIO::AsyncCmdExecutor::s_ptrExecutor) {
		throw EXEPTION(L"Tryed to call uninistialized AsyncCommandExecutor");
	}

	// Delete executor
	delete GxRenderIO::AsyncCmdExecutor::s_ptrExecutor;
}

BOOL GxRenderIO::AsyncCmdExecutor::queCmdList(GxDirect::XCmdList* ptrCmdList, GxRenderIO::CmdListPool* ptrPool, GxRenderIO::CmdListExecutionFlag* ptrFlag) {
	// Check if space is availible
	if (m_uiCapacity == m_uiUsed) {
		return FALSE;
	}

	// Find slot
	UINT64 slot = m_uiNextFreeIndex;
	BOOL slotValid = FALSE;
	while (!slotValid) {
		// Check if current slot is valid
		UINT flagIndex = (UINT)floor((FLOAT)slot / 64.0F);
		UINT bitIndex = slot % 64;

		// Check if valid / free
		slotValid = ((m_ptrsBitset[flagIndex] & (1ULL << bitIndex)) == 0x0);

		// Increment if slot is not valid
		if (!slotValid) {
			slot = (slot + 1) % m_uiCapacity;
		}
		// Else make slot used
		else {
			m_ptrsBitset[flagIndex] |= (1ULL << bitIndex);
		}
	}

	// Setup slot params
	m_ptrsCmdLists[slot].ptrCmdList = ptrCmdList;
	m_ptrsCmdLists[slot].ptrReturnPool = ptrPool;
	m_ptrsCmdLists[slot].ptrFlag = ptrFlag;

	// Start cmd list execution
	ptrCmdList->execute();

	// Increment usage and set next hint
	m_uiUsed++;
	m_uiNextFreeIndex = (slot + 1) % m_uiCapacity;

	// OK Return true
	return TRUE;
}

GxRenderIO::AsyncCmdExecutor::AsyncCmdExecutor(UINT capacity) :
	m_uiCapacity(capacity)
{
	// Allocate memory
	m_ptrsCmdLists = (_AsyncPair*)malloc(sizeof(_AsyncPair) * capacity);

	// Calculate number of UINT64 required for flag and alloc
	UINT numOfFlagWords = (UINT)ceil((FLOAT)capacity / 64.0F);
	m_ptrsBitset = (UINT64*)malloc(sizeof(UINT64) * numOfFlagWords);

	// Clear flags
	memset(&m_ptrsBitset, 0x00, sizeof(UINT64) * numOfFlagWords);

	// Create thread (and dispatch)
	m_hThread = CreateThread(NULL, 0, GxRenderIO::AsyncCmdExecutor::ThreadProc, this, NULL, NULL);
	if (!m_hThread) {
		throw EXEPTION_HR(L"CreateThread(...)", GetLastError());
	}
}

GxRenderIO::AsyncCmdExecutor::~AsyncCmdExecutor() {
	// Notify thread
	m_bThredShouldExit = TRUE;
	
	// Wait if thread returns normaly
	if (WaitForSingleObject(m_hThread, 30000) == WAIT_TIMEOUT) {
		// Force thread termination
		#pragma warning( push )
		#pragma warning( disable : 6258)
		TerminateThread(m_hThread, -1);
		#pragma warning( pop ) 
	}

	// Close handle to thread
	CloseHandle(m_hThread);

	// Check for pending list
	if (m_uiUsed) {
		for (UINT i = 0; i < m_uiCapacity; i++) {
			// Check if slot is used currently
			UINT flagIndex = (UINT)floor((FLOAT)i / 64.0F);
			UINT bitIndex = i % 64;

			// Check if slot is used
			if ((m_ptrsBitset[flagIndex] & (1ULL << bitIndex)) != 0x0) {
				// Delete command list
				delete m_ptrsCmdLists[i].ptrCmdList;
			}
		}
	}

	// Free memory
	free(m_ptrsCmdLists);
	free(m_ptrsBitset);
}

DWORD __stdcall GxRenderIO::AsyncCmdExecutor::ThreadProc(LPVOID param) {
	// Call implementation
	return ((GxRenderIO::AsyncCmdExecutor*)(param))->__itrlThreadProc();
}

DWORD GxRenderIO::AsyncCmdExecutor::__itrlThreadProc() {
	// While no exit code was send
	while (!m_bThredShouldExit) {
		// Sleep for 10s
		Sleep(1000);

		// Lock guard
		GxUtil::LockGuard janitor(&GxRenderIO::AsyncCmdExecutor::s_lock);

		// Cycle through list 
		if (m_uiUsed) {
			for (UINT i = 0; i < m_uiCapacity; i++) {
				// Check if slot is used currently
				UINT flagIndex = (UINT)floor((FLOAT)i / 64.0F);
				UINT bitIndex = i % 64;

				// Check if slot is used
				if ((m_ptrsBitset[flagIndex] & (1ULL << bitIndex)) != 0x0) {
					// Check if list is done with its work
					if (m_ptrsCmdLists[i].ptrCmdList->recordable()) {						
						// Push back to its allocator
						m_ptrsCmdLists[i].ptrReturnPool->push(m_ptrsCmdLists[i].ptrCmdList);
						
						// Notify caller 
						m_ptrsCmdLists[i].ptrFlag->__setCompleted();

						// Unset current bit
						m_ptrsBitset[flagIndex] &= ~(1ULL << bitIndex);

						// Set hint
						m_uiNextFreeIndex = i;
					}
				}
			}
		}
	}

	// All ok return null
	return 0;
}
