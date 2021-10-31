#pragma once
#include "DirectDefine.h"

class RenderBase
{
public:
	RenderBase(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context) : m_Device(device), m_DeviceContext(context) {}
	virtual ~RenderBase() = default;

public:
	virtual void OnResize(int width, int height) abstract;

protected:
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;
};

