#pragma once
class RenderTargetView : public Texture2D
{
public:
	RenderTargetView(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv, bool backbuffer = false);
	~RenderTargetView();

public:
	friend class GraphicResourceManager;

public:
	D3D11_RENDER_TARGET_VIEW_DESC GetDesc();

private:
	bool m_BackBuffer;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RTV;
};

