#pragma once

// RenderTarget Type Enum Class..
enum class eRenderTargetType
{
	BASIC,
	COMPUTE,
};

class RenderTarget : public Texture2D
{
public: 
	RenderTarget(eRenderTargetType type, ID3D11RenderTargetView** rtv);
	virtual ~RenderTarget();

public:
	ID3D11Texture2D* GetTexture2D() override;
	D3D11_TEXTURE2D_DESC GetTextureDesc() override;

public:
	ID3D11RenderTargetView* GetRTV();
	ID3D11RenderTargetView** GetAddressRTV();
	D3D11_RENDER_TARGET_VIEW_DESC GetRTVDesc();

public:
	// 현재 RenderTarget Type 반환 함수..
	eRenderTargetType GetType();

private:
	eRenderTargetType m_RenderTargetType;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RTV;
};

