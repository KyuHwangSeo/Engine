#pragma once

#ifdef KH_DLL
// DLL ���� ������Ʈ
// DLL �� ����� ����  �Լ�/Ŭ�������� "export" �� ����.
#define DLL_DECLSPEC  __declspec(dllexport)					//C++
#else

// DLL ��� ������Ʈ
// DLL �� ����ϱ� ���� �������. "import" �� ����.
#define DLL_DECLSPEC  __declspec(dllimport)					//C++
#endif
