#pragma once
class DeferredRender : public RenderBase
{
public:
	DeferredRender();
	~DeferredRender();

public:
	void Initialize(int width, int height) override;
	void OnResize(int width, int height) override;

private:
	VertexShader* m_MeshVS;
	VertexShader* m_SkinVS;
	PixelShader* m_DeferredPS;

	DepthStencilView* m_DepthStencilView;

	BasicRenderTarget* m_AlbedoRT;
	BasicRenderTarget* m_NormalRT;
	BasicRenderTarget* m_PositionRT;
	BasicRenderTarget* m_ShadowRT;
	BasicRenderTarget* m_NormalDepthRT;
	BasicRenderTarget* m_SSAORT;

	std::vector<ID3D11ShaderResourceView*> m_SRVList;
	std::vector<ID3D11RenderTargetView*> m_RTVList;


	D3D11_VIEWPORT* m_ScreenViewport;
};

