#pragma once

#define COM_RELEASE(comPtr) if(comPtr){comPtr->Release(); comPtr = NULL;}

// == Include Windows ==

#define NOMINMAX

#include <Windows.h>

// == Include windows related stuff ==
#include <wincodec.h>
#include <Wbemidl.h>
#include <comdef.h>
#include <intrin.h>

// == Include direcx ==
#include <d3dcompiler.h>
#include <dxgi.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <Xinput.h>
#include <xaudio2.h>

// == Include std lib ==
#include <map>
#include <array>
#include <vector>
#include <cassert>
#include <atomic>
#include <bitset>
#include <string>
#include <sstream>
#include <iomanip>

// == UGF12 Defines ==
#define UGF12_RESOURCE_TYPE_LAYER_FRAMEBUFFER 1UL