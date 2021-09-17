#pragma once
#pragma comment (lib,"fmodex_vc")

#ifdef _WIN64
	#ifdef _DEBUG
		#pragma comment(lib, "KHSound_x64d")
	#else
		#pragma comment(lib, "KHSound_x64")
	#endif
#else
	#ifdef _DEBUG
		#pragma comment(lib, "KHSound_x86d")
	#else
		#pragma comment(lib, "KHSound_x86")
	#endif
#endif

#include "fmod.hpp"
#include "fmod_errors.h"

using namespace FMOD;
#include <iostream>
#include <string>
#include <map>
#include "Sound.h"