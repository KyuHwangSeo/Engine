#pragma once

#define KH_STDCALL		__stdcall
#define KH_CCALL		__cdecl

#ifdef KH_DLL
	#define DLL_DECLSPEC  __declspec(dllexport)					//C++
	#define DLL_TEMPLATE
#else
	#define DLL_DECLSPEC  __declspec(dllimport)					//C++
	#define DLL_TEMPLATE  extern

		#ifdef _WIN64
			#ifdef _DEBUG
				#pragma comment(lib, "D3DEngine_x64d.lib")		//64bit + debug
			#else
				#pragma comment(lib, "D3DEngine_x64.lib")		//64bit.
			#endif 
		#else
			#ifdef _DEBUG
				#pragma comment(lib, "D3DEngine_x86d.lib")		//32bit + debug
			#else
				#pragma comment(lib, "D3DEngine_x86.lib")		//32bit.
			#endif
		#endif
#endif
