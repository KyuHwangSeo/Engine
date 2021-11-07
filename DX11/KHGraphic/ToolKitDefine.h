#pragma once

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

#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"