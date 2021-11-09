#include "D3DDefine.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget(ComPtr<ID3D11Device> device)
	:m_Device(device)
{

}

RenderTarget::~RenderTarget()
{

}

void RenderTarget::CreateRenderTarget(ID3D11Texture2D* texture2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	RESET_COM(m_RTV);
	RESET_COM(m_SRV);
	RESET_COM(m_UAV);

	/// 悼老茄 Texture2D 扁馆栏肺 RenderTarget Resource Binding..

	// RenderTargetView 积己..
	HR(m_Device->CreateRenderTargetView(texture2D, rtvDesc, m_RTV.GetAddressOf()));

	// ShaderResourceVie 积己..
	HR(m_Device->CreateShaderResourceView(texture2D, srvDesc, m_SRV.GetAddressOf()));

	// UnorderedAccessView 积己..
	HR(m_Device->CreateUnorderedAccessView(texture2D, uavDesc, m_UAV.GetAddressOf()));
}

void RenderTarget::CreateRTV(ID3D11Texture2D* texture2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc)
{
	ComPtr<ID3D11RenderTargetView> rtv0 = nullptr;
	ID3D11RenderTargetView** pprtv0 = rtv0.GetAddressOf();
	ComPtr<ID3D11RenderTargetView> rtv1 = nullptr;
	ID3D11RenderTargetView** pprtv2 = nullptr;

	if (pprtv0 == nullptr)
	{
		// RenderTargetView 积己..
		HR(m_Device->CreateRenderTargetView(texture2D, rtvDesc, rtv1.GetAddressOf()));
	}
	else
	{
		// RenderTargetView 积己..
		HR(m_Device->CreateRenderTargetView(texture2D, rtvDesc, pprtv0));
		rtv1 = *pprtv0;
	}
	ID3D11RenderTargetView** pprtv1 = rtv1.GetAddressOf();

	// RenderTargetView 积己..
	HR(m_Device->CreateRenderTargetView(texture2D, rtvDesc, rtv1.GetAddressOf()));


	RESET_COM(m_RTV);

}

void RenderTarget::CreateSRV(ID3D11Texture2D* texture2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	RESET_COM(m_SRV);

	// ShaderResourceVie 积己..
	HR(m_Device->CreateShaderResourceView(texture2D, srvDesc, m_SRV.GetAddressOf()));
}

void RenderTarget::CreateUAV(ID3D11Texture2D* texture2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	RESET_COM(m_UAV);

	// UnorderedAccessView 积己..
	HR(m_Device->CreateUnorderedAccessView(texture2D, uavDesc, m_UAV.GetAddressOf()));
}

void RenderTarget::Reset()
{
	RESET_COM(m_RTV);
	RESET_COM(m_SRV);
	RESET_COM(m_UAV);
}

void RenderTarget::Release()
{
	RESET_COM(m_Device);

	RESET_COM(m_RTV);
	RESET_COM(m_SRV);
	RESET_COM(m_UAV);
}
