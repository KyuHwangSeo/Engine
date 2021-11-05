#pragma once
#include "ResourceBase.h"
#include <wrl.h>
#include <d3d11.h>

class Texture2D : public IResource
{
public:
	Texture2D(ResourceType type, Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D) : IResource(type), m_Resource(tex2D) {}
	virtual ~Texture2D() { if (m_Resource != nullptr) { m_Resource.Reset(); } }

public:
	D3D11_TEXTURE2D_DESC GetTextureDesc();

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Resource;
};

inline D3D11_TEXTURE2D_DESC Texture2D::GetTextureDesc()
{
	D3D11_TEXTURE2D_DESC texDesc;

	m_Resource->GetDesc(&texDesc);

	return texDesc;
}