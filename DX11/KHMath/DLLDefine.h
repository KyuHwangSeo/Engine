#pragma once

#ifdef KH_DLL
// DLL 제작 프로젝트
// DLL 을 만들기 위해  함수/클래스등을 "export" 로 선언.
#define DLL_DECLSPEC  __declspec(dllexport)					//C++
#else

// DLL 사용 프로젝트
// DLL 을 사용하기 위한 헤더파일. "import" 로 선언.
#define DLL_DECLSPEC  __declspec(dllimport)					//C++
#endif
