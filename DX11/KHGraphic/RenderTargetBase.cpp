#include "DirectDefine.h"
#include "Texture2D.h"
#include "RenderTargetBase.h"
#include "BasicRenderTarget.h"
#include "ComputeRenderTarget.h"

RenderTarget::RenderTarget(eRenderTargetType type, ID3D11RenderTargetView** rtv)
	:m_RenderTargetType(type), m_RTV(*rtv)
{

}

RenderTarget::~RenderTarget()
{
	RESET_COM(m_RTV);
}

ID3D11Texture2D* RenderTarget::GetTexture2D()
{
	ID3D11Resource* resource = nullptr;

	// 현재 View의 Texture 2D Resource..
	m_RTV->GetResource(&resource);

	return (ID3D11Texture2D*)resource;
}

D3D11_TEXTURE2D_DESC RenderTarget::GetTextureDesc()
{
	ID3D11Resource* resource = nullptr;

	// 현재 View의 Texture 2D Resource..
	m_RTV->GetResource(&resource);

	// Texture 2D Description 추출..
	D3D11_TEXTURE2D_DESC texDesc;

	((ID3D11Texture2D*)resource)->GetDesc(&texDesc);

	return texDesc;
}

ID3D11RenderTargetView* RenderTarget::GetRTV()
{
	return m_RTV.Get();
}

ID3D11RenderTargetView** RenderTarget::GetAddressRTV()
{
	return m_RTV.GetAddressOf();
}

D3D11_RENDER_TARGET_VIEW_DESC RenderTarget::GetRTVDesc()
{
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	m_RTV->GetDesc(&rtvDesc);

	return rtvDesc;
}

eRenderTargetType RenderTarget::GetType()
{
	return m_RenderTargetType;
}