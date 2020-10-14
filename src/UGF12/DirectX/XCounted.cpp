#include "pch.h"
#include "XCounted.h"

GxDirect::XCounted::~XCounted() noexcept(false){
	// Check if ref count is not zero
	if (m_uiRefCount != 0) {
		throw EXEPTION(L"Trying to delete a ref counted object where ref count was not zero");
	}
}

void GxDirect::XCounted::IncRef() {
	// Atomic increment
	InterlockedIncrement64(&m_uiRefCount);
}

void GxDirect::XCounted::DecRef() {
	// Check if problem
	if (m_uiRefCount == 0) {
		throw EXEPTION(L"Decremented RefCounting while already beeing zero!");
	}

	// Atomic decrement
	InterlockedDecrement64(&m_uiRefCount);
}
