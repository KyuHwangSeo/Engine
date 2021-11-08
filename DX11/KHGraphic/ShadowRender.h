#pragma once
class ShadowRender : public RenderBase
{
public:
	ShadowRender();
	~ShadowRender();

public:
	void Initialize(int width, int height) override;
	void RenderFrame();

private:
	VertexShader* m_MeshShadowVS;
	VertexShader* m_SkinShadowVS;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_ShadowDSV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShadowSRV;

	D3D11_VIEWPORT* m_ShadowViewport;
};

