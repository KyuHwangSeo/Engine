#pragma once
#include <windows.h>

interface ID3D11Graphic
{
public:
	virtual void Initialize(HWND hwnd, int screenWidth, int screenHeight) abstract;
};
