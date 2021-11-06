#include "Texture2D.h"
#include "RenderTargetView.h"
#include "ShaderResourceView.h"
#include "UnorderedAccessView.h"
#include "DepthStecilView.h"
#include "ResourceManager.h"
#include "MacroDefine.h"

GraphicResourceManager::GraphicResourceManager(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain)
	:m_Device(device), m_SwapChain(swapChain), m_BackBuffer(nullptr)
{

}

GraphicResourceManager::~GraphicResourceManager()
{

}

void GraphicResourceManager::OnResize(int width, int height)
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));

	// Swap Chain, Render Target View Resize
	HR(m_SwapChain->ResizeBuffers(1, (UINT)width, (UINT)height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer = m_BackBuffer->m_Resource;

	// Get Swap Chain Back Buffer Pointer..
	HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));

	// BackBuffer Resize..
	texDesc = m_BackBuffer->GetTextureDesc();
	texDesc.Width = width;
	texDesc.Width = height;
	HR(m_Device->CreateTexture2D(&texDesc, 0, backBuffer.GetAddressOf()));

	rtvDesc = m_BackBuffer->GetDesc();
	HR(m_Device->CreateRenderTargetView(backBuffer.Get(), &rtvDesc, m_BackBuffer->m_RTV.GetAddressOf()));

	// RenderTargetView Resize..
	for (RenderTargetView* rtv : m_RTVList)
	{
		texDesc = rtv->GetTextureDesc();
		texDesc.Width = width;
		texDesc.Width = height;
		HR(m_Device->CreateTexture2D(&texDesc, 0, rtv->m_Resource.GetAddressOf()));

		rtvDesc = rtv->GetDesc();
		HR(m_Device->CreateRenderTargetView(rtv->m_Resource.Get(), &rtvDesc, rtv->m_RTV.GetAddressOf()));
	}

	// ShaderResourceView Resize..
	for (ShaderResourceView* srv : m_SRVList)
	{
		texDesc = srv->GetTextureDesc();
		texDesc.Width = width;
		texDesc.Width = height;
		HR(m_Device->CreateTexture2D(&texDesc, 0, srv->m_Resource.GetAddressOf()));

		srvDesc = srv->GetDesc();
		HR(m_Device->CreateShaderResourceView(srv->m_Resource.Get(), &srvDesc, srv->m_SRV.GetAddressOf()));
	}

	// UnorderedAccessView Resize..
	for (UnorderedAccessView* uav : m_UAVList)
	{
		texDesc = uav->GetTextureDesc();
		texDesc.Width = width;
		texDesc.Width = height;
		HR(m_Device->CreateTexture2D(&texDesc, 0, uav->m_Resource.GetAddressOf()));

		uavDesc = uav->GetDesc();
		HR(m_Device->CreateUnorderedAccessView(uav->m_Resource.Get(), &uavDesc, uav->m_UAV.GetAddressOf()));
	}

	// DepthStecilView Resize..
	for (DepthStecilView* dsv : m_DSVList)
	{
		texDesc = dsv->GetTextureDesc();
		texDesc.Width = width;
		texDesc.Width = height;
		HR(m_Device->CreateTexture2D(&texDesc, 0, dsv->m_Resource.GetAddressOf()));

		dsvDesc = dsv->GetDesc();
		HR(m_Device->CreateDepthStencilView(dsv->m_Resource.Get(), &dsvDesc, dsv->m_DSV.GetAddressOf()));
	}
}