#pragma once
#include "pch.h"

#include <UGF12/RenderIO/Framebuffer.h>
#include <UGF12/RenderIO/Executing/CmdListProxy.h>

namespace GxRenderIO {
	namespace LayerStack {
		/// <summary>
		/// Struck for storing the frame info
		/// </summary>
		struct LayerFrameInfo {
			/// <summary>
			/// Index of the current frame
			/// </summary>
			UINT64 frameIndex = 0;

			/// <summary>
			/// Start time of the Frame
			/// </summary>
			INT64 frameStartTime = 0;

			/// <summary>
			/// Delta time since last frame
			/// </summary>
			FLOAT deltaTMs = 0.0F;
		};

		/// <summary>
		/// Implementation of a layer stack
		/// </summary>
		class ILayerImpl {
			public:
				/// <summary>
				/// Create layer implementation
				/// </summary>
				/// <param name="layerName">Name of the layer</param>
				/// <param name="initialEnabled">If the layer is currently enabled</param>
				ILayerImpl(std::wstring layerName, BOOL initialEnabled = TRUE);

				/// <summary>
				/// Function to be called on init (constructor)
				/// </summary>
				virtual void Init() = 0;
				
				/// <summary>
				/// Function to be called on destroy (destruct)
				/// </summary>
				virtual void Destroy() = 0;

				/// <summary>
				/// Function is called when layer should do its draw operations
				/// </summary>
				/// <param name="ptrFrameInfo">Info about the frame (pointer)</param>
				/// <param name="ptrCmdListProxy">Pointer to cmd list proxy</param>
				/// <param name="ptrFrameBuffer">Pointer to frame buffer</param>
				virtual void draw(GxRenderIO::LayerStack::LayerFrameInfo* ptrFrameInfo, GxRenderIO::CmdListProxy* ptrCmdListProxy, GxRenderIO::FrameBuffer* ptrFrameBuffer) = 0;

				/// <summary>
				/// Indicates layer disable event
				/// </summary>
				/// <returns>True if layer can be disabled</returns>
				virtual BOOL onDisable();

				/// <summary>
				/// Indicates layer enable event
				/// </summary>
				/// <returns>True if layer can be enabled</returns>
				virtual BOOL onEnable();

				/// <summary>
				/// Returns if the layer is currently enabled
				/// </summary>
				/// <returns></returns>
				BOOL getLayerEnableState();

				/// <summary>
				/// Enables or disables the layer
				/// </summary>
				/// <param name="enable">If the layer should be enabled</param>
				/// <returns>If layer could be disables / enabled </returns>
				BOOL setLayerEnable(BOOL enable);

				// Delete unsupported
				ILayerImpl(const ILayerImpl&) = delete;
				void operator=(const ILayerImpl&) = delete;
			
			public:
				/// <summary>
				/// Name of the layer
				/// </summary>
				const std::wstring layer_name;

			private:
				/// <summary>
				/// Enable flag
				/// </summary>
				BOOL m_bEnabled;
		};
	}
}