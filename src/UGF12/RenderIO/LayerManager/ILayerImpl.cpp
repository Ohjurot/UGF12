#include "pch.h"
#include "ILayerImpl.h"

GxRenderIO::LayerStack::ILayerImpl::ILayerImpl(std::wstring layerName, BOOL initialEnabled) :
	layer_name(layerName),
	m_bEnabled(initialEnabled)
{ }

void GxRenderIO::LayerStack::ILayerImpl::onResourceChange(UINT resourceType, UINT resourceIndex, void* ptrResource) {
	// ...
}

void GxRenderIO::LayerStack::ILayerImpl::onResize(UINT width, UINT height) {
	// ...
}

BOOL GxRenderIO::LayerStack::ILayerImpl::onDisable() {
	return TRUE;
}

BOOL GxRenderIO::LayerStack::ILayerImpl::onEnable() {
	return TRUE;
}

BOOL GxRenderIO::LayerStack::ILayerImpl::getLayerEnableState() {
	return m_bEnabled;
}

BOOL GxRenderIO::LayerStack::ILayerImpl::setLayerEnable(BOOL enable) {
	// Check is enable / disable is suported
	if (enable) {
		// If is already enbale or onEnable returned false -> Error
		if (m_bEnabled || !onEnable()) {
			return FALSE;
		}
	} else {
		// If is already disabled or onDisable returned false -> Error
		if (!m_bEnabled || !onDisable()) {
			return FALSE;
		}
	}

	// Setup new state and return true
	m_bEnabled = enable;
	return TRUE;
}
