#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>
#include <UGF12/Util/StrConverter.h>

namespace GxRenderIO {
	namespace LayerStack {
		class Shader {
			public:
				/// <summary>
				/// Compile the shaders
				/// </summary>
				static void compile();

				/// <summary>
				/// Get the vertex shader binary data
				/// </summary>
				/// <param name="ppVoidData">Pointer to void* to be set to shader data</param>
				/// <param name="ptrSize">Pointer to SIZE_T to be set</param>
				static void getVertexShader(void** ppVoidData, SIZE_T* ptrSize);

				/// <summary>
				/// Get the pixel shader binary data
				/// </summary>
				/// <param name="ppVoidData">Pointer to void* to be set to shader data</param>
				/// <param name="ptrSize">Pointer to SIZE_T to be set</param>
				static void getPixelShader(void** ppVoidData, SIZE_T* ptrSize);
				
				// Delete unsupported
				Shader(const Shader&) = delete;
				void operator==(const Shader&) = delete;
			private:
				// Singelton
				Shader() {};

				// Destructor to free resources
				~Shader();

				/// <summary>
				/// Static instance
				/// </summary>
				static Shader s_instance;

			private:
				/// <summary>
				/// Blob for vertex shader bytecode
				/// </summary>
				ID3D10Blob* m_ptrBlobVs = NULL;

				/// <summary>
				/// Blob for pixel shader bytecode
				/// </summary>
				ID3D10Blob* m_ptrBlobPs = NULL;

			private:
				/// <summary>
				/// Vertex shader
				/// </summary>
				LPCSTR c_vertexShader = 
R"(#define ROOT "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
			 "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX), " \
			 "DescriptorTable(SRV(t0, numDescriptors = unbounded), visibility = SHADER_VISIBILITY_PIXEL), " \
			 "StaticSampler(s0, visibility = SHADER_VISIBILITY_PIXEL) "

struct InputCB{
	uint enableMask;
};

struct OuputD{
	float4 pos : SV_Position;
    float2 tex : TEXTCORDS;
	uint instance : SV_InstanceID;
};

ConstantBuffer<InputCB> layerStackBuffer : register(b0);

[RootSignature(ROOT)]
void main(float2 pos : POSITION, float2 tex : TEXTCORDS, uint instance : SV_InstanceID, out OuputD outData){
	outData.tex = tex;
	outData.instance = instance;

	if(layerStackBuffer.enableMask & (1U << instance)){
		outData.pos.xy = pos.xy;
		outData.pos.zw = 1.0f;
	} else {
		outData.pos.xyzw = 99.0f;
	}
}					
)";

				/// <summary>
				/// Pixel shader
				/// </summary>
				LPCSTR c_pixelShader = 
R"(#define ROOT "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
			 "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX), " \
			 "DescriptorTable(SRV(t0, numDescriptors = unbounded), visibility = SHADER_VISIBILITY_PIXEL), " \
			 "StaticSampler(s0, visibility = SHADER_VISIBILITY_PIXEL) "

struct InputD{
	float4 pos : SV_Position;
    float2 tex : TEXTCORDS;
	uint instance : SV_InstanceID;
};

Texture2D tex[16] : register(t0);
sampler smp : register(s0);

[RootSignature(ROOT)]
float4 main(InputD data) : SV_TARGET{
	float4 color = float4(tex[data.instance].Sample(smp, data.tex).rgb, 1.0f);
	if(color.r < 0.01 && color.g < 0.01 && color.b < 0.01){
		discard;
	}
	return color;
}
)";
		};
	}
}
