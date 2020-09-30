#pragma once
#include "pch.h"

#include <UGF12/DirectX/XContext.h>

namespace GxDirect {
	/// <summary>
	/// Collection of static helper functions for checking feature support
	/// </summary>
	class XFeature {
		public:
			/// <summary>
			/// Checks the multi-sampling image quality levels for a given format and sample count
			/// </summary>
			/// <param name="ptrContext">Pointer to context</param>
			/// <param name="msaaCount">Samples count</param>
			/// <param name="fbFormat">Framebuffer formate</param>
			/// <returns>Quality for input count</returns>
			static UINT msaaMaxQuality(GxDirect::XContext* ptrContext, UINT msaaCount, DXGI_FORMAT fbFormat = DXGI_FORMAT_R8G8B8A8_UNORM);
	};
}