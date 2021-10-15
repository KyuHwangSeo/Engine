#pragma once
#ifndef KH_STDCALL
	#define KH_STDCALL		__stdcall
#endif

#ifndef KH_CCALL
	#define KH_CCALL		__cdecl
#endif

#ifndef KH_VECTORCALL
	#define KH_VECTORCALL	__vectorcall
#endif

#ifdef SOUND_EXPORT
	#define SOUND_DLL  __declspec(dllexport)				
#else
	#define SOUND_DLL  __declspec(dllimport)			

	#ifdef _WIN64
		#ifdef _DEBUG
			#pragma comment(lib, "KHSound_x64d")		// 64bit + Debug
		#else
			#pragma comment(lib, "KHSound_x64")			// 64bit
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "KHSound_x86d")		// 32bit + Debug
		#else
			#pragma comment(lib, "KHSound_x86")			// 32bit
		#endif
	#endif
#endif
 