#pragma once

class ForwardPass : public RenderPassBase
{
public:
	ForwardPass();
	~ForwardPass();

public:
	void Initialize(int width, int height) override;
	void OnResize(int width, int height) override;
	void Release() override;

	void BeginRender();
	void Update(MeshData* mesh, GlobalData* global);
	void Render(MeshData* mesh);

private:
	VertexShader* m_MeshVS;
	VertexShader* m_SkinVS;
	PixelShader* m_ForwardPS;

	DepthStencilView* m_DSV;
	ID3D11DepthStencilView* m_DepthStencilView;

	ID3D11DepthStencilState* m_DepthStencilState;
	ID3D11RasterizerState* m_RasterizerState;
	ID3D11BlendState* m_BlendState;

	BasicRenderTarget* m_BackBuffer;

	ID3D11RenderTargetView* m_BackBufferRTV;
	ID3D11ShaderResourceView* m_BackBufferSRV;

	D3D11_VIEWPORT* m_ScreenViewport;
};

