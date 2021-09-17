#pragma once

#ifdef KH_DLL
// DLL 제작 프로젝트
// DLL 을 만들기 위해  함수/클래스등을 "export" 로 선언.
#define KH_API  __declspec(dllexport)					//C++
#define KH_TEMPLATE
#else

// DLL 사용 프로젝트
// DLL 을 사용하기 위한 헤더파일. "import" 로 선언.
#define KH_API  __declspec(dllimport)					//C++
#define KH_TEMPLATE extern

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
