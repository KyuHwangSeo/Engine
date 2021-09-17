#pragma once

#ifdef KH_DLL
// DLL ���� ������Ʈ
// DLL �� ����� ����  �Լ�/Ŭ�������� "export" �� ����.
#define KH_API  __declspec(dllexport)					//C++
#define KH_TEMPLATE
#else

// DLL ��� ������Ʈ
// DLL �� ����ϱ� ���� �������. "import" �� ����.
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
