#include "DirectDefine.h"
#include "Texture2D.h"
#include "RenderTargetBase.h"
#include "ComputeRenderTarget.h"

ComputeRenderTarget::ComputeRenderTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv, Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav)
	:RenderTarget(eRenderTargetType::COMPUTE, rtv), m_UAV(uav)
{

}

ComputeRenderTarget::~ComputeRenderTarget()
{
	RESET_COM(m_UAV);
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
