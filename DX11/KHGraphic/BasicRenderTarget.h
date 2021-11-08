#pragma once
class BasicRenderTarget : public RenderTarget
{
public:
	BasicRenderTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);
	~BasicRenderTarget();

public:
	D3D11_SHADER_RESOURCE_VIEW_DESC GetSRVDesc();
	ID3D11ShaderResourceView* GetSRV();
	ID3D11ShaderResourceView** GetAddressSRV();

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
};

