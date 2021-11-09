#include "DirectDefine.h"
#include "Texture2D.h"
#include "RenderTargetBase.h"
#include "ComputeRenderTarget.h"

ComputeRenderTarget::ComputeRenderTarget(ID3D11RenderTargetView** rtv, ID3D11UnorderedAccessView** uav)
	:RenderTarget(eRenderTargetType::COMPUTE, rtv), m_UAV(*uav)
{

}

ComputeRenderTarget::~ComputeRenderTarget()
{
	RESET_COM(m_UAV);
}

ID3D11Texture2D* ComputeRenderTarget::GetTexture2D()
{
	ID3D11Resource* resource = nullptr;

	// 현재 View의 Texture 2D Resource..
	m_UAV->GetResource(&resource);

	return (ID3D11Texture2D*)resource;
}

ID3D11Texture2D** ComputeRenderTarget::GetAddressTexture2D()
{
	ID3D11Resource* resource = nullptr;

	// 현재 View의 Texture 2D Resource..
	m_UAV->GetResource(&resource);

	return (ID3D11Texture2D**)(&resource);
}

D3D11_TEXTURE2D_DESC ComputeRenderTarget::GetTextureDesc()
{
	ID3D11Resource* resource = nullptr;

	// 현재 View의 Texture 2D Resource..
	m_UAV->GetResource(&resource);

	// Texture 2D Description 추출..
	D3D11_TEXTURE2D_DESC texDesc;

	((ID3D11Texture2D*)resource)->GetDesc(&texDesc);

	return texDesc;
}

D3D11_UNORDERED_ACCESS_VIEW_DESC ComputeRenderTarget::GetUAVDesc()
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	m_UAV->GetDesc(&uavDesc);

	return uavDesc;
}

ID3D11UnorderedAccessView* ComputeRenderTarget::GetUAV()
{
	return m_UAV.Get();
}

ID3D11UnorderedAccessView** ComputeRenderTarget::GetAddressUAV()
{
	return m_UAV.GetAddressOf();
}
