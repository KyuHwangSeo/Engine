#pragma once
class IRenderer
{
public:
	IRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context) :m_Device(device), m_DeviceContext(context) {};
	virtual ~IRenderer() = default;

protected:
	int m_Width;
	int m_Height;

	ComPtr<ID3D11Device> m_Device = nullptr;
	ComPtr<ID3D11DeviceContext> m_DeviceContext = nullptr;

public:
	virtual void OnResize(int width, int height) abstract;
};

