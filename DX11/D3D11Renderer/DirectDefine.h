#pragma once

// DX Lib
#pragma comment(lib, "dxguid.lib") 
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#ifdef _WIN64
	#ifdef _DEBUG
		#pragma comment( lib, "DirectXTK_x64d.lib" )
	#else
		#pragma comment( lib, "DirectXTK_x64.lib" )
	#endif
#else
	#ifdef _DEBUG
		#pragma comment( lib, "DirectXTK_x86d.lib" )
	#else
		#pragma comment( lib, "DirectXTK_x86.lib" )
	#endif
#endif

#include <d3dcompiler.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <wrl.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)									\
	{									\
		HRESULT hr = (x);						\
		if(FAILED(hr))							\
		{								\
			LPWSTR output;                                    	\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |		\
				FORMAT_MESSAGE_IGNORE_INSERTS 	 |		\
				FORMAT_MESSAGE_ALLOCATE_BUFFER,			\
				NULL,						\
				hr,						\
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	\
				(LPTSTR) &output,				\
				0,						\
				NULL);					        \
			MessageBox(NULL, output, L"Error", MB_OK);		\
		}								\
	}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 
