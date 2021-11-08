#pragma once
class DepthStencilView : public Texture2D
{
public:
	DepthStencilView(Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv);
	~DepthStencilView();

public:
	ID3D11Texture2D* GetTexture2D() override;
	ID3D11Texture2D** GetAddressTexture2D() override;
	D3D11_TEXTURE2D_DESC GetTextureDesc() override;

public:
	ID3D11DepthStencilView* GetDSV();
	ID3D11DepthStencilView** GetAddressDSV();
	D3D11_DEPTH_STENCIL_VIEW_DESC GetDSVDesc();

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DSV;
};

