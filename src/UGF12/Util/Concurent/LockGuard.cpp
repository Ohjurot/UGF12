#include "pch.h"
#include "LockGuard.h"

GxUtil::LockGuard::LockGuard(GxUtil::ILock* ptrLock) :
	m_ptrLock(ptrLock)
{
	// Aquire lock
	m_ptrLock->aquire();
}

GxUtil::LockGuard::~LockGuard() {
	// Release lock
	m_ptrLock->release();
}
