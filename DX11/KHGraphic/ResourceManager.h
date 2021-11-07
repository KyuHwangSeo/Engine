#pragma once
#include <vector>
#include "ResourceManagerBase.h"

class RenderTargetView;
class ShaderResourceView;
class UnorderedAccessView;
class DepthStecilView;

class GraphicResourceManager : public IGraphicResourceManager
{
public:
	GraphicResourceManager();
	~GraphicResourceManager();

public:
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain) override;
	void OnResize(int width, int height) override;
	
public:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GetDepthStencilState(eDSState state) override;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> GetRasterizerState(eRState state) override;
	Microsoft::WRL::ComPtr<ID3D11BlendState> GetBlendState(eBState state) override;

public:
	template<typename T>
	void AddResource(T resource);
	void AddBackBufferRTV(RenderTargetView* rtv) { m_BackBuffer = rtv; }

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

	// BackBuffer Àü¿ë RenderTargetView..
	RenderTargetView* m_BackBuffer;

	/////////////////////////////////////////////////////////////////////////////////////////
	// View Resource List
	/////////////////////////////////////////////////////////////////////////////////////////

	std::vector<RenderTargetView*> m_RTVList;
	std::vector<ShaderResourceView*> m_SRVList;
	std::vector<UnorderedAccessView*> m_UAVList;
	std::vector<DepthStecilView*> m_DSVList; 

	/////////////////////////////////////////////////////////////////////////////////////////
	// State Resource List
	/////////////////////////////////////////////////////////////////////////////////////////

	std::vector<Microsoft::WRL::ComPtr<ID3D11DepthStencilState>> m_DSSList;
	std::vector<Microsoft::WRL::ComPtr<ID3D11RasterizerState>> m_RSList;
	std::vector<Microsoft::WRL::ComPtr<ID3D11BlendState>> m_BSList;
	std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_SSList;
};

template<typename T>
inline void GraphicResourceManager::AddResource(T resource) {}
