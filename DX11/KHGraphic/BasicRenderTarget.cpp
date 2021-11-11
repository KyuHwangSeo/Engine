#include "DirectDefine.h"
#include "Texture2D.h"
#include "RenderTargetBase.h"
#include "BasicRenderTarget.h"

BasicRenderTarget::BasicRenderTarget(ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv)
	:RenderTarget(eRenderTargetType::BASIC, rtv), m_SRV(*srv)
{
	
}

BasicRenderTarget::~BasicRenderTarget()
{
	RESET_COM(m_SRV);
}

ID3D11Texture2D* BasicRenderTarget::GetTexture2D()
{
	ID3D11Resource* resource = nullptr;

	// 현재 View의 Texture 2D Resource..
	m_SRV->GetResource(&resource);

	return (ID3D11Texture2D*)resource;
}

D3D11_TEXTURE2D_DESC BasicRenderTarget::GetTextureDesc()
{
	ID3D11Resource* resource = nullptr;

	// 현재 View의 Texture 2D Resource..
	m_SRV->GetResource(&resource);

	// Texture 2D Description 추출..
	D3D11_TEXTURE2D_DESC texDesc;

	((ID3D11Texture2D*)resource)->GetDesc(&texDesc);

	return texDesc;
}

D3D11_SHADER_RESOURCE_VIEW_DESC BasicRenderTarget::GetSRVDesc()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	m_SRV->GetDesc(&srvDesc);

	return srvDesc;
}

ID3D11ShaderResourceView* BasicRenderTarget::GetSRV()
{
	return m_SRV.Get();
}

ID3D11ShaderResourceView** BasicRenderTarget::GetAddressSRV()
{
	return m_SRV.GetAddressOf();
}
