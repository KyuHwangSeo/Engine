#pragma once
class LightRender : public RenderBase
{
public:
	LightRender();
	~LightRender();

public:
	void Initialize() override;

private:
	VertexShader* m_ScreenVS;
	PixelShader* m_ScreenPS;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_AlbedoSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_NormalSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_PositionSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShadowSRV;

	D3D11_VIEWPORT* m_ScreenViewport;
};

