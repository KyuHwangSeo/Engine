#pragma once
class ShadowPass : public RenderPassBase
{
public:
	ShadowPass();
	~ShadowPass();

public:
	void Initialize(int width, int height) override;
	void OnResize(int width, int height) override;

	void BeginRender();
	void Render(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMMATRIX world, ID3D11Buffer* vb, ID3D11Buffer* ib, UINT size, UINT offset, UINT indexCount);

private:
	VertexShader* m_MeshShadowVS;
	VertexShader* m_SkinShadowVS;

	DepthStencilView* m_ShadowDepthStencilView;
	
	BasicRenderTarget* m_ShadowRT;

	ID3D11DepthStencilView* m_ShadowDSV;
	ID3D11ShaderResourceView* m_ShadowSRV;

	D3D11_VIEWPORT* m_ShadowViewport;
};

