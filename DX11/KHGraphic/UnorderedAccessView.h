#pragma once
class UnorderedAccessView : public Texture2D
{
public:
	UnorderedAccessView(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav);
	~UnorderedAccessView();

public:
	friend class GraphicResourceManager;

public:
	D3D11_UNORDERED_ACCESS_VIEW_DESC GetDesc();

private:
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UAV;
};

