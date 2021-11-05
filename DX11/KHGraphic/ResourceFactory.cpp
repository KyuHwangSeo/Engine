#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include "ResourceFactory.h"
#include "MacroDefine.h"
#include "Texture2D.h"
#include "RenderTargetView.h"
#include "ShaderResourceView.h"
#include "UnorderedAccessView.h"
#include "DepthStecilView.h"
#include "ResourceManager.h"
 
GraphicResourceFactory::GraphicResourceFactory(GraphicResourceManager* manager)
	:m_ResourceManager(manager)
{
	m_Device = m_ResourceManager->GetDevie();
	m_SwapChain = m_ResourceManager->GetSwapChain();
}

GraphicResourceFactory::~GraphicResourceFactory()
{

}

Microsoft::WRL::ComPtr<ID3D11Texture2D> GraphicResourceFactory::CreateBackBuffer(UINT width, UINT height, DXGI_FORMAT format, UINT swapchainflags)
{
	// Swap Chain, Render Target View Resize
	HR(m_SwapChain->ResizeBuffers(1, (UINT)width, (UINT)height, format, swapchainflags));

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer = nullptr;

	// Get Swap Chain Back Buffer Pointer..
	HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));

	return backBuffer;
}

Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GraphicResourceFactory::CreateBackBufferRTV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D)
{
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr;

	// RenderTargetView 持失..
	HR(m_Device->CreateRenderTargetView(tex2D.Get(), nullptr, rtv.GetAddressOf()));

	m_ResourceManager->AddBackBufferRTV(new RenderTargetView(tex2D, rtv));

	return rtv;
}

Microsoft::WRL::ComPtr<ID3D11Texture2D> GraphicResourceFactory::CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;

	HR(m_Device->CreateTexture2D(texDesc, 0, tex2D.GetAddressOf()));

	return tex2D;
}

Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GraphicResourceFactory::CreateRTV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc)
{
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr;

	// RenderTargetView 持失..
	HR(m_Device->CreateRenderTargetView(tex2D.Get(), rtvDesc, rtv.GetAddressOf()));

	m_ResourceManager->AddResource(new RenderTargetView(tex2D, rtv));

	return rtv;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GraphicResourceFactory::CreateSRV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;

	// ShaderResourceView 持失..
	HR(m_Device->CreateShaderResourceView(tex2D.Get(), srvDesc, srv.GetAddressOf()));

	m_ResourceManager->AddResource(new ShaderResourceView(tex2D, srv));

	return srv;
}

Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> GraphicResourceFactory::CreateUAV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav = nullptr;

	// UnorderedAccessView 持失..
	HR(m_Device->CreateUnorderedAccessView(tex2D.Get(), uavDesc, uav.GetAddressOf()));

	m_ResourceManager->AddResource(new UnorderedAccessView(tex2D, uav));

	return uav;
}

Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GraphicResourceFactory::CreateDSV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc)
{
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv = nullptr;

	// DepthStencilView 持失..
	HR(m_Device->CreateDepthStencilView(tex2D.Get(), dsvDesc, dsv.GetAddressOf()));

	m_ResourceManager->AddResource(new DepthStecilView(tex2D, dsv));

	return dsv;
}

Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GraphicResourceFactory::CreateDSS(D3D11_DEPTH_STENCIL_DESC* dssDesc)
{
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dss = nullptr;

	// DepthStencilState 持失..
	HR(m_Device->CreateDepthStencilState(dssDesc, dss.GetAddressOf()));

	return dss;
}

Microsoft::WRL::ComPtr<ID3D11RasterizerState> GraphicResourceFactory::CreateRSS(D3D11_RASTERIZER_DESC* rssDesc)
{
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rss = nullptr;

	// DepthStencilState 持失..
	HR(m_Device->CreateRasterizerState(rssDesc, rss.GetAddressOf()));

	return rss;
}
