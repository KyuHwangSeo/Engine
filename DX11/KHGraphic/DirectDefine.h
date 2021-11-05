#pragma once

// DX Lib
#pragma comment(lib, "dxguid.lib") 
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#ifdef _WIN64
	#ifdef _DEBUG
		#pragma comment( lib, "../lib/DirectXTK_x64d.lib" )
	#else
		#pragma comment( lib, "../lib/DirectXTK_x64.lib" )
	#endif
#else
	#ifdef _DEBUG
		#pragma comment( lib, "../lib/DirectXTK_x86d.lib" )
	#else
		#pragma comment( lib, "../lib/DirectXTK_x86.lib" )
	#endif
#endif

#include <d3dcompiler.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <wrl.h>
#include "SimpleMath.h"

using namespace Microsoft::WRL;

#include "MacroDefine.h"