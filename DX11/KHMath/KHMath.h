#pragma once

#ifdef _WIN64
	#ifdef _DEBUG
		#pragma comment(lib, "KHMath_x64d.lib")
	#else
		#pragma comment(lib, "KHMath_x64.lib")
	#endif 
#else
	#ifdef _DEBUG
		#pragma comment(lib, "KHMath_x86d.lib")
	#else
		#pragma comment(lib, "KHMath_x86.lib")
	#endif
#endif

#include "SimpleMath.h"
using namespace DirectX;
using namespace SimpleMath;

#include "DXMath.h"

