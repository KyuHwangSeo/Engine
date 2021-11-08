#pragma once

class ComputeRenderTarget : public RenderTarget
{
public:
	ComputeRenderTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv, Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav);
	~ComputeRenderTarget();

public:
	D3D11_UNORDERED_ACCESS_VIEW_DESC GetUAVDesc();
	ID3D11UnorderedAccessView* GetUAV();
	ID3D11UnorderedAccessView** GetAddressUAV();

private:
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UAV;
};

