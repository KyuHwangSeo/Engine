#include "Texture2D.h"
#include "RenderTargetView.h"
#include "MacroDefine.h"

RenderTargetView::RenderTargetView(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv, bool backbuffer)
	:Texture2D(ResourceType::RTV, tex2D), m_RTV(rtv), m_BackBuffer(backbuffer)
{

}

RenderTargetView::~RenderTargetView()
{
	RESET_COM(m_RTV);
}

D3D11_RENDER_TARGET_VIEW_DESC RenderTargetView::GetDesc()
{
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;

	m_RTV->GetDesc(&rtvDesc);

	return rtvDesc;
}
