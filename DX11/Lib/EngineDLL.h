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

#ifdef ENGINE_EXPORT
	#define ENGINE_DLL  __declspec(dllexport)			
	#define DLL_TEMPLATE
#else
	#define ENGINE_DLL  __declspec(dllimport)			
	#define DLL_TEMPLATE  extern

		#ifdef _WIN64
			#ifdef _DEBUG
				#pragma comment(lib, "D3DEngine_x64d.lib")		// 64bit + Debug
			#else
				#pragma comment(lib, "D3DEngine_x64.lib")		// 64bit
			#endif 
		#else
			#ifdef _DEBUG
				#pragma comment(lib, "D3DEngine_x86d.lib")		// 32bit + Debug
			#else
				#pragma comment(lib, "D3DEngine_x86.lib")		// 32bit
			#endif
		#endif
#endif
 