#include "pch.h"
#include "LayerStackShader.h"

// Static instance
GxRenderIO::LayerStack::Shader GxRenderIO::LayerStack::Shader::s_instance;

void GxRenderIO::LayerStack::Shader::compile(){
	// Hresult
	HRESULT hr;
	
	// empty compiler flags
	UINT compilerFlags = 0x0;
	#if defined(_DEBUG)
		compilerFlags |= D3DCOMPILE_DEBUG;
	#endif

	// If not exists compile
	if (!s_instance.m_ptrBlobVs) {
		// Error Buffer
		ID3D10Blob* pErr = NULL;
		
		// Compile shader VS
		if (FAILED(hr = D3DCompile(s_instance.c_vertexShader, strlen(s_instance.c_vertexShader), NULL, NULL, NULL, "main", "vs_5_1", compilerFlags, NULL, &s_instance.m_ptrBlobVs, &pErr))) {
			// Get Error string
			LPCSTR errStr = (LPSTR)pErr->GetBufferPointer();
			
			// Build message
			std::wstringstream wss;
			wss << L"D3DCompile(...) for vs_5_1" << std::endl;
			wss << GxUtil::StrConverter::ConvertN2W(errStr).c_str();

			throw EXEPTION_HR(wss.str(), hr);
		}

		// Compile shader PS
		if (FAILED(hr = D3DCompile(s_instance.c_pixelShader, strlen(s_instance.c_pixelShader), NULL, NULL, NULL, "main", "ps_5_1", compilerFlags, NULL, &s_instance.m_ptrBlobPs, &pErr))) {
			// Get Error string
			LPCSTR errStr = (LPSTR)pErr->GetBufferPointer();

			// Build message
			std::wstringstream wss;
			wss << L"D3DCompile(...) for ps_5_1" << std::endl;
			wss << GxUtil::StrConverter::ConvertN2W(errStr).c_str();

			throw EXEPTION_HR(wss.str(), hr);
		}

		// Rlease Error
		COM_RELEASE(pErr);
	}
}

void GxRenderIO::LayerStack::Shader::getVertexShader(void** ppVoidData, SIZE_T* ptrSize) {
	// Check if compiled
	if (!s_instance.m_ptrBlobVs) {
		throw EXEPTION(L"Vertex shader has not been compiled yet!");
	}

	// Set pointer and size
	*ppVoidData = s_instance.m_ptrBlobVs->GetBufferPointer();
	*ptrSize = s_instance.m_ptrBlobVs->GetBufferSize();
}

void GxRenderIO::LayerStack::Shader::getPixelShader(void** ppVoidData, SIZE_T* ptrSize) {
	// Check if compiled
	if (!s_instance.m_ptrBlobPs) {
		throw EXEPTION(L"Vertex shader has not been compiled yet!");
	}

	// Set pointer and size
	*ppVoidData = s_instance.m_ptrBlobPs->GetBufferPointer();
	*ptrSize = s_instance.m_ptrBlobPs->GetBufferSize();
}

GxRenderIO::LayerStack::Shader::~Shader() {
	// Release all
	COM_RELEASE(m_ptrBlobPs);
	COM_RELEASE(m_ptrBlobVs);
}
