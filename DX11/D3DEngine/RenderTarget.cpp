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
	RESET_COM(m_RTV);

	// RenderTargetView 积己..
	HR(m_Device->CreateRenderTargetView(texture2D, rtvDesc, m_RTV.GetAddressOf()));
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
