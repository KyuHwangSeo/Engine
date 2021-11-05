#pragma once
class ShaderResourceView : public Texture2D
{
public:
	ShaderResourceView(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);
	~ShaderResourceView();

public:
	friend class GraphicResourceManager;

public:
	D3D11_SHADER_RESOURCE_VIEW_DESC GetDesc();

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
};

