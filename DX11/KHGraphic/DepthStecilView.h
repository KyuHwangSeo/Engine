#pragma once
class DepthStecilView : public Texture2D
{
public:
	DepthStecilView(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv);
	~DepthStecilView();

public:
	friend class GraphicResourceManager;

public:
	D3D11_DEPTH_STENCIL_VIEW_DESC GetDesc();

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DSV;
};

