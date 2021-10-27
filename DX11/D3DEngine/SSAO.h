#pragma once
class SSAO : public IRenderer
{
public:
	SSAO(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context);
	~SSAO();

public:
	RenderTarget* m_RandomVectorRT;
	RenderTarget* m_AmbientRT;
	RenderTarget* m_BlurRT;

private:
	DXVector4 m_FrustumFarCorner[4];

	DXVector4 m_Offsets[14];
	DXMatrix4X4 m_TextureSpace;

	D3D11_VIEWPORT m_AmbientMapViewport;

	ID3D11RenderTargetView* m_RenderTarget;

	VertexBuffer* m_QuadBuffer;

	Shader* m_SSAOShader;
	Shader* m_HorzBlurShader;
	Shader* m_VertBlurShader;

	Camera** m_MainCamera;

	cbSsaoFrame m_SsaoFrame;
	cbTexel m_TexelWidth;
	cbTexel m_TexelHeight;

public:
	void SsaoRender(ID3D11ShaderResourceView* normalDepthSRV);
	void BlurRender(ID3D11ShaderResourceView* normalDepthSRV, int blurCount);

	void OnResize(int width, int height);
	void BuildFrustumFarCorners();

	void BuildOffsetVectors();
	void BuildRandomVectorTexture();

	void SetVertexBuffer(VertexBuffer* vBuffer) { m_QuadBuffer = vBuffer; }

	D3D11_VIEWPORT& GetViewPort() { return m_AmbientMapViewport; }
};

