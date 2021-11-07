#pragma once
class DeferredRender : public RenderBase
{
public:
	DeferredRender();
	~DeferredRender();

public:
	void Initialize() override;

private:
	VertexShader* m_MeshVS;
	VertexShader* m_SkinVS;
	PixelShader* m_DeferredPS;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_AlbedoSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_NormalSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_PositionSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShadowSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_NormalDepthSRV;

	D3D11_VIEWPORT* m_ScreenViewport;
};

