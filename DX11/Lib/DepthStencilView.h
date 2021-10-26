#pragma once
class DepthStencilView
{
public:
	DepthStencilView(std::string dsvName);
	~DepthStencilView();

public:
	void Create(D3D11_TEXTURE2D_DESC texDesc, D3D11_DEPTH_STENCIL_VIEW_DESC descDSV);
	void OnResize(int width, int height);

	ID3D11DepthStencilView* GetDSV() { return m_DSV.Get(); }

private:
	std::string m_Name;
	ComPtr<ID3D11Device> m_Device;

	ComPtr<ID3D11DepthStencilView> m_DSV;
	ComPtr<ID3D11Texture2D> m_DSB = nullptr;

	D3D11_TEXTURE2D_DESC m_DescTex;
	D3D11_DEPTH_STENCIL_VIEW_DESC m_DescDSV;
};

