#include <vector>
#include "DirectDefine.h"
#include "ViewPort.h"
#include "Texture2D.h"
#include "DepthStencilView.h"
#include "RenderTargetBase.h"
#include "BasicRenderTarget.h"
#include "ComputeRenderTarget.h"
#include "ResourceManager.h"
#include "EnumDefine.h"

GraphicResourceManager::GraphicResourceManager()
	:m_Device(nullptr), m_SwapChain(nullptr),m_BackBuffer(nullptr)
{

}

GraphicResourceManager::~GraphicResourceManager()
{

}

void GraphicResourceManager::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain)
{
	m_Device = device;
	m_SwapChain = swapChain;
}

void GraphicResourceManager::OnResize(int width, int height)
{
	ComPtr<ID3D11Texture2D> tex2D = nullptr;

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

	// Get Swap Chain Back Buffer Pointer..
	HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(tex2D.GetAddressOf())));

	// BackBuffer Resize..
	BasicRenderTarget* bRenderTarget = reinterpret_cast<BasicRenderTarget*>(m_BackBuffer);
	HR(m_Device->CreateRenderTargetView(tex2D.Get(), nullptr, bRenderTarget->GetAddressRTV()));
	HR(m_Device->CreateShaderResourceView(tex2D.Get(), nullptr, bRenderTarget->GetAddressSRV()));
	
	// RenderTarget Resize..
	for (RenderTarget* rt : m_RenderTargetList)
	{
		// Texture2D Description 추출..
		texDesc = rt->GetTextureDesc();
		texDesc.Width = width;
		texDesc.Width = height;

		// Texture2D Resize..
		HR(m_Device->CreateTexture2D(&texDesc, 0, tex2D.ReleaseAndGetAddressOf()));

		// RenderTargetView Description 추출..
		rtvDesc = rt->GetRTVDesc();
		
		// RenderTargetView Resize..
		HR(m_Device->CreateRenderTargetView(tex2D.Get(), &rtvDesc, rt->GetAddressRTV()));

		switch (rt->GetType())
		{
		case eRenderTargetType::BASIC:
		{
			BasicRenderTarget* bRenderTarget = reinterpret_cast<BasicRenderTarget*>(rt);

			// ShaderResourceView Description 추출..
			srvDesc = bRenderTarget->GetSRVDesc();

			// ShaderResourceView Resize..
			HR(m_Device->CreateShaderResourceView(tex2D.Get(), &srvDesc, bRenderTarget->GetAddressSRV()));
		}
			break;
		case eRenderTargetType::COMPUTE:
		{
			ComputeRenderTarget* cRenderTarget = reinterpret_cast<ComputeRenderTarget*>(rt);
			
			// UnorderedAccessView Description 추출..
			uavDesc = cRenderTarget->GetUAVDesc();

			// UnorderedAccessView Resize..
			HR(m_Device->CreateUnorderedAccessView(tex2D.Get(), &uavDesc, cRenderTarget->GetAddressUAV()));
		}
			break;
		default:
			break;
		}
	}

	// DepthStecilView Resize..
	for (DepthStencilView* dsv : m_DepthStencilViewList)
	{
		texDesc = dsv->GetTextureDesc();
		texDesc.Width = width;
		texDesc.Width = height;
		HR(m_Device->CreateTexture2D(&texDesc, 0, tex2D.ReleaseAndGetAddressOf()));

		dsvDesc = dsv->GetDSVDesc();
		HR(m_Device->CreateDepthStencilView(tex2D.Get(), &dsvDesc, dsv->GetAddressDSV()));
	}

	// ViewPort Resize..
	for (ViewPort* viewport : m_ViewPortList)
	{
		viewport->OnResize(width, height);
	}

	RESET_COM(tex2D);
}

RenderTarget* GraphicResourceManager::GetMainRenderTarget()
{
	return m_BackBuffer;
}

RenderTarget* GraphicResourceManager::GetRenderTarget(eRenderTarget state)
{
	return m_RenderTargetList[(int)state];
}

DepthStencilView* GraphicResourceManager::GetDepthStencilView(eDepthStencilView state)
{
	return m_DepthStencilViewList[(int)state];
}

ID3D11BlendState* GraphicResourceManager::GetBlendState(eBlendState state)
{
	return m_BlendStateList[(int)state].Get();
}

ID3D11RasterizerState* GraphicResourceManager::GetRasterizerState(eRasterizerState state)
{
	return m_RasterizerStateList[(int)state].Get();
}

ID3D11DepthStencilState* GraphicResourceManager::GetDepthStencilState(eDepthStencilState state)
{
	return m_DepthStencilStateList[(int)state].Get();
}

D3D11_VIEWPORT* GraphicResourceManager::GetViewPort(eViewPort state)
{
	return m_ViewPortList[(int)state]->GetViewPort();
}

// AddResource
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