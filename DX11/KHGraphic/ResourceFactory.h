#pragma once
#include "IResourceFactory.h"

class GraphicResourceManager;
class GraphicResourceFactory : public IGraphicResourceFactory
{
public:
	GraphicResourceFactory(GraphicResourceManager* manager);
	~GraphicResourceFactory();

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateBackBuffer(UINT width, UINT height, DXGI_FORMAT format, UINT swapchainflags);
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> CreateBackBufferRTV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D);
	
	Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc);

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> CreateRTV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc);
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateSRV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> CreateUAV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> CreateDSV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc);
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> CreateDSS(D3D11_DEPTH_STENCIL_DESC* dssDesc);
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> CreateRSS(D3D11_RASTERIZER_DESC* rssDesc);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

	GraphicResourceManager* m_ResourceManager;
};

