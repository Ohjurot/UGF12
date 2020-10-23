#include "pch.h"
#include "File.h"

GxUtil::FS::File::File(GxUtil::FS::FileSystem* ptrFileSystm, LPCWSTR fileName) {
	// craft file name
	LPCWSTR ffName = ptrFileSystm->createPath(fileName);

	// Copy file name
	wcscpy_s(m_filePathComplete, MAX_PATH, ffName);

	// Set impl
	m_ptrImpl = ptrFileSystm->getImpl();
}

GxUtil::FS::File::~File() {
	// Close file
	close();
}

BOOL GxUtil::FS::File::exists() {
	// If already open return
	if (m_bFileOpen) {
		return TRUE;
	}

	// Open only existing
	if (m_hFile = m_ptrImpl->openFile(m_filePathComplete, UGF12_FILE_OPEN_READ | UGF12_FILE_OPEN_EXISTING)) {
		// direct close return true
		m_ptrImpl->closeFile(m_hFile);
		return TRUE;
	}

	// Open failed
	return FALSE;
}

BOOL GxUtil::FS::File::open(BOOL read, BOOL write, BOOL create) {
	// Flags
	DWORD flags = 0x0;

	// Check read
	if (read) {
		flags = UGF12_FILE_OPEN_READ;
	}

	// Check write
	if (write) {
		flags |= UGF12_FILE_OPEN_WRITE;
	}

	// Check create
	if(create) {
		flags |= UGF12_FILE_OPEN_ALLWAYS;
	}
	else {
		flags |= UGF12_FILE_OPEN_EXISTING;
	}

	// Call with flags
	return open(flags);
}

BOOL GxUtil::FS::File::open(DWORD flags) {
	// Already open means true
	if (m_bFileOpen) {
		return TRUE;
	}

	// Check if opening succeeds
	if (m_hFile = m_ptrImpl->openFile(m_filePathComplete, flags)) {
		// Is now open
		m_bFileOpen = TRUE;
		return TRUE;
	}

	// Fallback false
	return FALSE;
}

BOOL GxUtil::FS::File::create(BOOL keepOpen) {
	// If open creation failed
	if (m_bFileOpen) {
		return FALSE;
	}
	
	// Create file
	if (m_hFile = m_ptrImpl->openFile(m_filePathComplete, UGF12_FILE_OPEN_NEW)) {
		// Check if should be keep open
		if (keepOpen) {
			m_bFileOpen = TRUE;
		}
		else {
			m_ptrImpl->closeFile(m_hFile);
		}

		// Return true
		return TRUE;
	}

	// Fallback false
	return FALSE;
}

void GxUtil::FS::File::close() {
	// If open close and set flag
	if (m_bFileOpen) {
		m_ptrImpl->closeFile(m_hFile);
		m_bFileOpen = FALSE;
	}
}

BOOL GxUtil::FS::File::isOpen() {
	return m_bFileOpen;
}
