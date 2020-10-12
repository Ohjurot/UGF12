#pragma once

#define COM_RELEASE(comPtr) if(comPtr){comPtr->Release(); comPtr = NULL;}

// == Include Windows ==

#define NOMINMAX

#include <Windows.h>

// == Include windows related stuff ==
#include <wincodec.h>

// == Include direcx ==
#include <d3dcompiler.h>
#include <dxgi.h>
#include <d3d12.h>
#include <Xinput.h>
#include <xaudio2.h>

// == Include std lib ==
#include <map>
#include <vector>
#include <cassert>
#include <atomic>
#include <bitset>
#include <string>
#include <sstream>

// == UGF12 Defines ==
#define UGF12_RESOURCE_TYPE_LAYER_FRAMEBUFFER 1UL