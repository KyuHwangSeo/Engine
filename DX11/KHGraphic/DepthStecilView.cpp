#include "DirectDefine.h"
#include "Texture2D.h"
#include "DepthStecilView.h"
#include "MacroDefine.h"

DepthStecilView::DepthStecilView(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv)
	:Texture2D(ResourceType::DSV, tex2D), m_DSV(dsv)
{

}

DepthStecilView::~DepthStecilView()
{
	RESET_COM(m_DSV);
}

D3D11_DEPTH_STENCIL_VIEW_DESC DepthStecilView::GetDesc()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;

	m_DSV->GetDesc(&dsvDesc);

	return dsvDesc;
}
