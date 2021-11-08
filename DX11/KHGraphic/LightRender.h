#pragma once
class LightRender : public RenderBase
{
public:
	LightRender();
	~LightRender();

public:
	void Initialize(int width, int height) override;
	void OnResize(int width, int height) override;

private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_BackBufferRTV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_BackBufferSRV;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_AlbedoSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_NormalSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_PositionSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShadowSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SSAOSRV;

	VertexShader* m_ScreenVS;
	PixelShader* m_ScreenPS;

	DepthStencilView* m_DepthStencilView;

	D3D11_VIEWPORT* m_ScreenViewport;
};

