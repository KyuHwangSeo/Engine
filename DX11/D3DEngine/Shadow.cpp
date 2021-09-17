#include "D3DDefine.h"
#include "IRenderer.h"
#include "RenderTarget.h"
#include "Shadow.h"

Shadow::Shadow(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context)
	:IRenderer(device, context)
{
	m_ShadowRT = new RenderTarget(device);
}

Shadow::~Shadow()
{

}

void Shadow::OnResize(int width, int height)
{
	m_Width = width;
	m_Height = height;

	RESET_COM(m_ShadowDepthStencilView);

	// 그림자 맵의 크기와 일치하는 뷰포트
	m_ShadowViewport.TopLeftX = 0.0f;
	m_ShadowViewport.TopLeftY = 0.0f;
	m_ShadowViewport.Width = static_cast<float>(width * 4);
	m_ShadowViewport.Height = static_cast<float>(height * 4);
	m_ShadowViewport.MinDepth = 0.0f;
	m_ShadowViewport.MaxDepth = 1.0f;

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = width * 4;
	texDesc.Height = height * 4;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> depthMap = nullptr;
	HR(m_Device->CreateTexture2D(&texDesc, 0, depthMap.GetAddressOf()));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	HR(m_Device->CreateDepthStencilView(depthMap.Get(), &dsvDesc, m_ShadowDepthStencilView.GetAddressOf()));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	m_ShadowRT->CreateSRV(depthMap.Get(), &srvDesc);

	RESET_COM(depthMap);
}

void Shadow::Render()
{
	m_DeviceContext->OMSetBlendState(0, 0, 0xffffffff);
	m_DeviceContext->RSSetViewports(1, &m_ShadowViewport);

	// 그리기만 할 것이므로 null Render Target 설정..
	// 깊이 버퍼, null Rendering 대상을 설정하면 색상 쓰기가 비활성화 된다..
	m_DeviceContext->OMSetRenderTargets(0, nullptr, m_ShadowDepthStencilView.Get());
	m_DeviceContext->ClearDepthStencilView(m_ShadowDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}
