#include "pch.h"
#include "XCounted.h"

GxDirect::XCounted::~XCounted() noexcept(false){
	// Check if ref count is not zero
	if (m_uiRefCount != 0) {
		throw EXEPTION_HR(L"Trying to delete a ref counted object where ref count was not zero", E_ABORT);
	}
}

void GxDirect::XCounted::IncRef() {
	m_uiRefCount++;
}

void GxDirect::XCounted::DecRef() {
	m_uiRefCount--;
}
