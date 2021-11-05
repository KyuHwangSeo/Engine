#pragma once
#include <vector>
#include "ResourceManagerBase.h"

class IResource;
class RenderTargetView;
class ShaderResourceView;
class UnorderedAccessView;
class DepthStecilView;

class GraphicResourceManager : public IGraphicResourceManager
{
public:
	GraphicResourceManager(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain);
	~GraphicResourceManager();

public:
	void OnResize(int width, int height) override;
	
	template<typename T>
	void AddResource(T resource);

	void AddBackBufferRTV(RenderTargetView* rtv) { m_BackBuffer = rtv; }

public:
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevie() { return m_Device; }
	Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain() { return m_SwapChain; }

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

	// BackBuffer Àü¿ë RenderTargetView..
	RenderTargetView* m_BackBuffer;

	// RenderTargetView List..
	std::vector<RenderTargetView*> m_RTVList;
	// ShaderResourceView List..
	std::vector<ShaderResourceView*> m_SRVList;
	// UnorderedAccessView List..
	std::vector<UnorderedAccessView*> m_UAVList;
	// DepthStecilView List..
	std::vector<DepthStecilView*> m_DSVList;
};

template<typename T>
inline void GraphicResourceManager::AddResource(T resource) {}

template<>
inline void GraphicResourceManager::AddResource(RenderTargetView* resource)		{ m_RTVList.push_back(resource); }

template<>
inline void GraphicResourceManager::AddResource(ShaderResourceView* resource)	{ m_SRVList.push_back(resource); }

template<>
inline void GraphicResourceManager::AddResource(UnorderedAccessView* resource)	{ m_UAVList.push_back(resource); }

template<>
inline void GraphicResourceManager::AddResource(DepthStecilView* resource)		{ m_DSVList.push_back(resource); }
