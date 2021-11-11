#pragma once
#include "ResourceManagerBase.h"

class GraphicResourceManager : public IGraphicResourceManager
{
public:
	GraphicResourceManager();
	~GraphicResourceManager();

public:
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain) override;
	void OnResize(int width, int height) override;
	void Release() override;

public:
	RenderTarget* GetMainRenderTarget() override;
	RenderTarget* GetRenderTarget(eRenderTarget state) override;

	DepthStencilView* GetDepthStencilView(eDepthStencilView state) override;

	ID3D11BlendState* GetBlendState(eBlendState state) override;
	ID3D11RasterizerState* GetRasterizerState(eRasterizerState state) override;
	ID3D11DepthStencilState* GetDepthStencilState(eDepthStencilState state) override;

	D3D11_VIEWPORT* GetViewPort(eViewPort state) override;
	BufferData* GetBuffer(eBuffer state) override;

public:
	template<typename T>
	void AddResource(T resource);
	void AddMainRenderTarget(RenderTarget* rtv) { m_BackBuffer = rtv; }

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

	/////////////////////////////////////////////////////////////////////////////////////////
	// RenderTarget Resource List
	/////////////////////////////////////////////////////////////////////////////////////////

	RenderTarget* m_BackBuffer;
	std::vector<RenderTarget*> m_RenderTargetList;

	/////////////////////////////////////////////////////////////////////////////////////////
	// View Resource List
	/////////////////////////////////////////////////////////////////////////////////////////

	std::vector<DepthStencilView*> m_DepthStencilViewList;
	std::vector<ViewPort*> m_ViewPortList;

	/////////////////////////////////////////////////////////////////////////////////////////
	// State Resource List
	/////////////////////////////////////////////////////////////////////////////////////////

	std::vector<Microsoft::WRL::ComPtr<ID3D11DepthStencilState>> m_DepthStencilStateList;
	std::vector<Microsoft::WRL::ComPtr<ID3D11RasterizerState>> m_RasterizerStateList;
	std::vector<Microsoft::WRL::ComPtr<ID3D11BlendState>> m_BlendStateList;
	std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_SamplerStateList;

	/////////////////////////////////////////////////////////////////////////////////////////
	// Buffer Resource List
	/////////////////////////////////////////////////////////////////////////////////////////
	std::vector<BufferData*> m_BufferList;
};

template<typename T>
inline void GraphicResourceManager::AddResource(T resource) {}

template<>
inline void GraphicResourceManager::AddResource(ViewPort* resource) { m_ViewPortList.push_back(resource); }

template<>
inline void GraphicResourceManager::AddResource(RenderTarget* resource) { m_RenderTargetList.push_back(resource); }

template<>
inline void GraphicResourceManager::AddResource(Microsoft::WRL::ComPtr<ID3D11DepthStencilState> resource) { m_DepthStencilStateList.push_back(resource); }

template<>
inline void GraphicResourceManager::AddResource(Microsoft::WRL::ComPtr<ID3D11RasterizerState> resource) { m_RasterizerStateList.push_back(resource); }

template<>
inline void GraphicResourceManager::AddResource(Microsoft::WRL::ComPtr<ID3D11BlendState> resource) { m_BlendStateList.push_back(resource); }

template<>
inline void GraphicResourceManager::AddResource(Microsoft::WRL::ComPtr<ID3D11SamplerState> resource) { m_SamplerStateList.push_back(resource); }

template<>
inline void GraphicResourceManager::AddResource(BufferData* resource) { m_BufferList.push_back(resource); }