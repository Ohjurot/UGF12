#include "pch.h"
#include "XCmdQueue.h"

GxDirect::XCmdQueue::XCmdQueue(GxDirect::XContext* ptrContext) {
	HRESULT hr;
	
	// Get device pointer
	ID3D12Device* ptrDevice;
	ptrContext->getDevice(&ptrDevice);

	// Create fece
	if (FAILED(hr = ptrDevice->CreateFence(m_uiFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_ptrFence)))) {
		throw EXEPTION_HR(L"ID3D12Device->CreateFence(...)", hr);
	}

	// Create descriptor for cmd que
	D3D12_COMMAND_QUEUE_DESC queDesk;
	ZeroMemory(&queDesk, sizeof(D3D12_COMMAND_QUEUE_DESC));

	// Describe que
	queDesk.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queDesk.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	queDesk.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queDesk.NodeMask = NULL;

	// Create cmd que
	if (FAILED(hr = ptrDevice->CreateCommandQueue(&queDesk, IID_PPV_ARGS(&m_ptrCmdQue)))) {
		throw EXEPTION_HR(L"ID3D12Device->CreateCommandQueue(...)", hr);
	}

	#ifdef _DEBUG
		// Name command que
		m_ptrCmdQue->SetName(L"Command queue");
	#endif
	
	// Release local device pointer
	COM_RELEASE(ptrDevice);
}

GxDirect::XCmdQueue::~XCmdQueue() {
	// Wait for completion
	waitForValue(m_uiFenceValue);
	
	// Release que
	COM_RELEASE(m_ptrCmdQue);

	// Release fence
	COM_RELEASE(m_ptrFence);
}

void GxDirect::XCmdQueue::getCommandQueue(ID3D12CommandQueue** ppQue){
	// Set pointer
	*ppQue = m_ptrCmdQue;

	// Increment ref count
	m_ptrCmdQue->AddRef();
}

UINT64 GxDirect::XCmdQueue::execute(ID3D12CommandList** ptrsCmdList, UINT listCount) {
	HRESULT hr;
	
	// Submit cmd lists to que
	m_ptrCmdQue->ExecuteCommandLists(listCount, ptrsCmdList);

	// Set fenc value
	m_uiFenceValue++;
	if (FAILED(hr = m_ptrCmdQue->Signal(m_ptrFence, m_uiFenceValue))) {
		throw EXEPTION_HR(L"ID3D12CommandQueue->Signal(...)", hr);
	}

	// Return wait for value to caller
	return m_uiFenceValue;
}

BOOL GxDirect::XCmdQueue::finished(UINT64 value) {
	// Return if fence is equal or higher
	return (m_ptrFence->GetCompletedValue() >= value);
}

void GxDirect::XCmdQueue::waitForValue(UINT64 value, BOOL forceSignal) {
	HRESULT hr;
	
	// Check if force is required
	if (forceSignal) {
		// Signal value and set internal
		if (FAILED(hr = m_ptrCmdQue->Signal(m_ptrFence, value))) {
			throw EXEPTION_HR(L"ID3D12CommandQueue->Signal(...)", hr);
		}
		m_uiFenceValue = value;
	}
	
	// If que is not finished
	if (!finished(value)) {
		// Create OS wait event
		HANDLE hEvent = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);

		// check if event has been created
		if (hEvent) {
			// Tell fence to trigger event
			m_ptrFence->SetEventOnCompletion(value, hEvent);

			// Wait for event
			WaitForSingleObject(hEvent, INFINITE);

			// Close handle to event
			CloseHandle(hEvent);
		}
		else {
			// Event creation failed fallback to loop and pause
			while (!finished(value)) {
				_mm_pause();
			}
		}
	}
}

void GxDirect::XCmdQueue::flushGpu(){
	// Force next index
	waitForValue(m_uiFenceValue + 1, TRUE);
}
