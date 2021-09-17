#pragma once
/// <summary>
/// RenderTarget Class
/// </summary>
/// 
/// RenderTarget 관련 Resource를 유동적으로 관리하기 위한 클래스
/// SRV & UAV & RTV 한 묶음으로 잡고 간다.

class RenderTarget
{
public:
	RenderTarget(ComPtr<ID3D11Device> device);
	~RenderTarget();

private:
	ComPtr<ID3D11Device> m_Device = nullptr;

	ComPtr<ID3D11RenderTargetView> m_RTV = nullptr;
	ComPtr<ID3D11ShaderResourceView> m_SRV = nullptr;
	ComPtr<ID3D11UnorderedAccessView> m_UAV = nullptr;

public:
	void CreateRenderTarget(ID3D11Texture2D* texture2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);
	void CreateRTV(ID3D11Texture2D* texture2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc);
	void CreateSRV(ID3D11Texture2D* texture2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);
	void CreateUAV(ID3D11Texture2D* texture2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);
	
	void Reset();
	void Release();

	ID3D11RenderTargetView* GetRTV()		{ return m_RTV.Get(); }
	ID3D11ShaderResourceView* GetSRV()		{ return m_SRV.Get(); }
	ID3D11UnorderedAccessView* GetUAV()		{ return m_UAV.Get(); }
};

