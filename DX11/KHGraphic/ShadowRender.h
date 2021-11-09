#pragma once
class ShadowRender : public RenderBase
{
public:
	ShadowRender();
	~ShadowRender();

public:
	void Initialize(int width, int height) override;
	void OnResize(int width, int height) override;

	void Render();

private:
	VertexShader* m_MeshShadowVS;
	VertexShader* m_SkinShadowVS;

	DepthStencilView* m_ShadowDepthStencilView;
	
	BasicRenderTarget* m_ShadowRT;

	ID3D11DepthStencilView* m_ShadowDSV;
	ID3D11ShaderResourceView* m_ShadowSRV;

	D3D11_VIEWPORT* m_ShadowViewport;
};

