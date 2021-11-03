#pragma once
#include "ID3D11Graphic.h"

class D3D11Graphic : public ID3D11Graphic
{
public:
	D3D11Graphic();
	~D3D11Graphic();
	 
public:
	void Initialize(HWND hwnd, int screenWidth, int screenHeight) override;

private:
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;
	ComPtr<IDXGISwapChain> m_SwapChain;
};