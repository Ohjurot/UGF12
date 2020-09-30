#include "pch.h"
#include "XCmdList.h"

GxDirect::XCmdList::XCmdList(GxDirect::XContext* ptrContext, GxDirect::XCmdQueue* ptrCmdQue) :
	m_ptrCmdQue(ptrCmdQue)
{
	// Ref counting
	m_ptrCmdQue->IncRef();

	HRESULT hr;

	// Get device
	ID3D12Device* ptrDevice;
	ptrContext->getDevice(&ptrDevice);

	// Create Allocator
	if (FAILED(hr = ptrDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT , IID_PPV_ARGS(&m_ptrCmdAlloc)))) {
		throw EXEPTION_HR(L"ID3D12Device->CreateCommandAllocator(...)", hr);
	}

	// Create cmd list
	if (FAILED(hr = ptrDevice->CreateCommandList(NULL, D3D12_COMMAND_LIST_TYPE_DIRECT, m_ptrCmdAlloc, NULL, IID_PPV_ARGS(&m_ptrCmdList)))) {
		throw EXEPTION_HR(L"ID3D12Device->CreateCommandList(...)", hr);
	}

	// Set sate to open
	m_state = CmdListState::CMD_LIST_OPEN;

	// Free device
	COM_RELEASE(ptrDevice);
}

GxDirect::XCmdList::~XCmdList() {
	// Ref counting
	m_ptrCmdQue->DecRef();

	// Check if list is in flight
	if (m_state == CmdListState::CMD_LIST_EXECUTING) {
		// Wait for execution
		m_ptrCmdQue->waitForValue(m_uiExecutionValue);

		// Updating of m_state not required because object gets destroyd now
	}

	// Relase list and allocator
	COM_RELEASE(m_ptrCmdList);
	COM_RELEASE(m_ptrCmdAlloc);
}

ID3D12GraphicsCommandList* GxDirect::XCmdList::get(BOOL doAssertState) {
	// Wait is assert is required
	if (doAssertState) {
		wait();
	}

	// Return pointer
	return m_ptrCmdList;
}

GxDirect::CmdListState GxDirect::XCmdList::getState() {
	return m_state;
}

void GxDirect::XCmdList::close() {
	// Check if fast return is possible
	if (m_state == CmdListState::CMD_LIST_CLOSED) {
		return;
	}

	// Check if wait is required
	if (m_state != CmdListState::CMD_LIST_OPEN) {
		wait();
	}

	// Close command list
	HRESULT hr;
	if (FAILED(hr = m_ptrCmdList->Close())) {
		throw EXEPTION_HR(L"ID3D12GraphicsCommandList->Close(...)", hr);
	}

	// Set state
	m_state = CmdListState::CMD_LIST_CLOSED;
}

void GxDirect::XCmdList::execute() {
	// Check if fast return is possible
	if (m_state == CmdListState::CMD_LIST_EXECUTING) {
		return;
	}

	// Check if close is required
	if (m_state != CmdListState::CMD_LIST_CLOSED) {
		close();
	}

	// Execute
	ID3D12CommandList* arr[] = {
		m_ptrCmdList
	};
	m_uiExecutionValue = m_ptrCmdQue->execute(arr, 1);

	// Set state
	m_state = CmdListState::CMD_LIST_EXECUTING;
}

void GxDirect::XCmdList::wait(){
	// Check if fast return is possible
	if (m_state == CmdListState::CMD_LIST_OPEN) {
		return;
	}

	// Check if execute is required
	if (m_state == CmdListState::CMD_LIST_CLOSED) {
		execute();
	}

	// Wait until completion
	m_ptrCmdQue->waitForValue(m_uiExecutionValue);

	// Reset alloc and list
	HRESULT hr;
	if (FAILED(hr = m_ptrCmdAlloc->Reset())) {
		throw EXEPTION_HR(L"ID3D12CommandAllocator->Reset(...)", hr);
	}
	if (FAILED(hr = m_ptrCmdList->Reset(m_ptrCmdAlloc, NULL))) {
		throw EXEPTION_HR(L"ID3D12GraphicsCommandList->Reset(...)", hr);
	}

	// Set state
	m_state = CmdListState::CMD_LIST_OPEN;
}

void GxDirect::XCmdList::flush(){
	// Check if realy finsihed
	if (!m_ptrCmdQue->finished(m_uiExecutionValue)) {
		throw EXEPTION(L"Flush has to be calle on command que first!");
	}

	// Check if command list needs reset
	if (m_state != CmdListState::CMD_LIST_OPEN) {
		// Reset alloc and list
		HRESULT hr;
		if (FAILED(hr = m_ptrCmdAlloc->Reset())) {
			throw EXEPTION_HR(L"ID3D12CommandAllocator->Reset(...)", hr);
		}
		if (FAILED(hr = m_ptrCmdList->Reset(m_ptrCmdAlloc, NULL))) {
			throw EXEPTION_HR(L"ID3D12GraphicsCommandList->Reset(...)", hr);
		}

		// Set state
		m_state = CmdListState::CMD_LIST_OPEN;
	}
}

BOOL GxDirect::XCmdList::recordable() {
	// If executing and done trigger wait (no blocking now!)
	if (m_state == CmdListState::CMD_LIST_EXECUTING &&  m_ptrCmdQue->finished(m_uiExecutionValue)) {
		wait();
	}

	// Return if list is (now) open
	return (m_state == CmdListState::CMD_LIST_OPEN);
}
