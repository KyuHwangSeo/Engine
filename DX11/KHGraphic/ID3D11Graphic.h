#pragma once
#include <windows.h>

class ID3D11Graphic
{
public:
	ID3D11Graphic() = default;
	virtual ~ID3D11Graphic() = default;

public:
	virtual void Initialize(HWND hwnd, int screenWidth, int screenHeight) abstract;

public:

};
