#include "DirectDefine.h"
#include "Texture2D.h"
#include "DepthStencilView.h"

DepthStencilView::DepthStencilView(ID3D11DepthStencilView** dsv)
	:m_DSV(*dsv)
{

}

DepthStencilView::~DepthStencilView()
{
	RESET_COM(m_DSV);
}

void DepthStencilView::Reset()
{
	RESET_COM(m_DSV);
}

ID3D11Texture2D* DepthStencilView::GetTexture2D()
{
	ID3D11Resource* resource = nullptr;

	// 현재 View의 Texture 2D Resource..
	m_DSV->GetResource(&resource);

	return (ID3D11Texture2D*)resource;
}

D3D11_TEXTURE2D_DESC DepthStencilView::GetTextureDesc()
{
	ID3D11Resource* resource = nullptr;

	// 현재 View의 Texture 2D Resource..
	m_DSV->GetResource(&resource);

	// Texture 2D Description 추출..
	D3D11_TEXTURE2D_DESC texDesc;

	((ID3D11Texture2D*)resource)->GetDesc(&texDesc);

	return texDesc;
}

ID3D11DepthStencilView* DepthStencilView::GetDSV()
{
	return m_DSV.Get();
}

ID3D11DepthStencilView** DepthStencilView::GetAddressDSV()
{
	return m_DSV.GetAddressOf();
}

D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilView::GetDSVDesc()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	m_DSV->GetDesc(&dsvDesc);

	return dsvDesc;
}
