#pragma once
#include "pch.h"

namespace GxRenderIO {
	namespace LayerStack {

		/// <summary>
		/// Implementation of a layer stack
		/// </summary>
		class ILayerImpl {
			public:
				ILayerImpl(std::wstring layerName) :
					layer_name(layerName)
				{}

				/// <summary>
				/// Function to be called on init (constructor)
				/// </summary>
				virtual void Init() = 0;
				
				/// <summary>
				/// Function to be called on destroy (destruct)
				/// </summary>
				virtual void Destroy() = 0;

				// Delete unsupported
				ILayerImpl(const ILayerImpl&) = delete;
				void operator=(const ILayerImpl&) = delete;
			
			public:
				const std::wstring layer_name;
		};
	}
}