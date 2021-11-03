#pragma once
class Shadow : public RenderBase
{
public:
	Shadow(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context);
	~Shadow();

public:
	void OnResize(int width, int height) override;
	void RenderFrame();

private:
	ComPtr<ID3D11DepthStencilView> m_ShadowDepthStencilView;
	ComPtr<ID3D11ShaderResourceView> m_ShadowSRV;
	D3D11_VIEWPORT m_ShadowViewport;

public:
	D3D11_VIEWPORT& GetViewPort() { return m_ShadowViewport; }
};

