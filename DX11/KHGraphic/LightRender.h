#pragma once
class LightRender : public RenderBase
{
public:
	LightRender();
	~LightRender();

public:
	void Initialize(int width, int height) override;
	void OnResize(int width, int height) override;

	void Render();

private:
	VertexShader* m_ScreenVS;
	PixelShader* m_ScreenPS; 
	
	DepthStencilView* m_DSV;
	ID3D11DepthStencilView* m_DepthStencilView;

	ID3D11DepthStencilState* m_DepthStencilState;
	ID3D11RasterizerState* m_RasterizerState;
	ID3D11BlendState* m_BlendState;

	BasicRenderTarget* m_BackBuffer;

	ID3D11RenderTargetView* m_BackBufferRTV;
	ID3D11ShaderResourceView* m_BackBufferSRV;

	ID3D11ShaderResourceView* m_AlbedoSRV;
	ID3D11ShaderResourceView* m_NormalSRV;
	ID3D11ShaderResourceView* m_PositionSRV;
	ID3D11ShaderResourceView* m_ShadowSRV;
	ID3D11ShaderResourceView* m_SSAOSRV;

	D3D11_VIEWPORT* m_ScreenViewport;
};

