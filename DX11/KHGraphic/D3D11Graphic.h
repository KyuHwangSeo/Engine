#pragma once
#include "ID3D11Graphic.h"

class D3D11Graphic : public ID3D11Graphic
{
public:
	D3D11Graphic();
	~D3D11Graphic();
	 
public:
	void Initialize(HWND hwnd, int screenWidth, int screenHeight) override;

public:
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice()			{ return m_Device; }
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext()	{ return m_DeviceContext; }
	Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain()		{ return m_SwapChain; }

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
};