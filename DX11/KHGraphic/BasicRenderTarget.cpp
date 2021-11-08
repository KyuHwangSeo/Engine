#include "DirectDefine.h"
#include "Texture2D.h"
#include "RenderTargetBase.h"
#include "BasicRenderTarget.h"

BasicRenderTarget::BasicRenderTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
	:RenderTarget(eRenderTargetType::BASIC, rtv), m_SRV(srv)
{
	
}

BasicRenderTarget::~BasicRenderTarget()
{
	RESET_COM(m_SRV);
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
