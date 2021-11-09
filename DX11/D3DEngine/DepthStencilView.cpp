#include "D3DDefine.h"
#include "D3DEngine.h"
#include "DepthStencilView.h"

DepthStencilView::DepthStencilView(string dsvName)
	:m_Name(dsvName)
{
	m_Device = D3DEngine::GetInstance()->GetDevice();
}

DepthStencilView::~DepthStencilView()
{
	RESET_COM(m_Device);
	RESET_COM(m_DSV);
	RESET_COM(m_DSB);
}

void DepthStencilView::Create(D3D11_TEXTURE2D_DESC texDesc, D3D11_DEPTH_STENCIL_VIEW_DESC descDSV)
{
	m_DescTex = texDesc;
	m_DescDSV = descDSV;
}

void DepthStencilView::OnResize(int width, int height)
{
	RESET_COM(m_DSV);
	RESET_COM(m_DSB);

	m_DescTex.Width = width;
	m_DescTex.Height = height;

	/// Texture2D 배열을 생성..
	HR(m_Device->CreateTexture2D(&m_DescTex, 0, m_DSB.GetAddressOf()));

	/// 리소스 데이터에 액세스하기 위한 DepthStencilView 생성..
	HR(m_Device->CreateDepthStencilView(m_DSB.Get(), nullptr, m_DSV.GetAddressOf()));

}
