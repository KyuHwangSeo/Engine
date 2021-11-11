#pragma once
class DeferredPass : public RenderPassBase
{
public:
	DeferredPass();
	~DeferredPass();

public:
	void Initialize(int width, int height) override;
	void OnResize(int width, int height) override;
	void Release() override;

	void BeginRender();
	void Render(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMMATRIX world, ID3D11Buffer* vb, ID3D11Buffer* ib, UINT size, UINT offset, UINT indexCount);

private:
	VertexShader* m_MeshVS;
	VertexShader* m_SkinVS;
	PixelShader* m_DeferredPS;

	DepthStencilView* m_DSV;
	ID3D11DepthStencilView* m_DepthStencilView;
	
	ID3D11DepthStencilState* m_DepthStencilState;
	ID3D11RasterizerState* m_RasterizerState;
	ID3D11BlendState* m_BlendState;

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

