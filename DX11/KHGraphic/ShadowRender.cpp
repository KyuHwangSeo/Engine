#include "DirectDefine.h"
#include "RenderBase.h"
#include "ShaderBase.h"
#include "ShaderResourceBase.h"
#include "VertexShader.h"
#include "ViewPort.h"
#include "Texture2D.h"
#include "DepthStencilView.h"
#include "RenderTargetBase.h"
#include "BasicRenderTarget.h"
#include "ShadowRender.h"

#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"

ShadowRender::ShadowRender()
	:m_ShadowDSV(nullptr), m_ShadowSRV(nullptr)
{
}

ShadowRender::~ShadowRender()
{

}

void ShadowRender::Initialize(int width, int height)
{
	// Shader 설정..
	m_MeshShadowVS = reinterpret_cast<VertexShader*>(g_Shader->GetShader("NormalShadowVS"));
	m_SkinShadowVS = reinterpret_cast<VertexShader*>(g_Shader->GetShader("SkinShadowVS"));

	// ViewPort 설정..
	m_ShadowViewport = g_Factory->CreateViewPort(0.0f, 0.0f, width, height, 4.0f, 4.0f);;

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

	// Texture 2D 생성..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	g_Factory->CreateTexture2D(&texDesc, tex2D.GetAddressOf());

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	// DepthStencilView 생성..
	g_Factory->CreateDSV(tex2D.Get(), &dsvDesc, &m_ShadowDSV);

	// DepthStencilView 설정..
	m_ShadowDepthStencilView = g_Resource->GetDepthStencilView(eDepthStencilView::SHADOW);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	// ShaderResourceView 생성..
	g_Factory->CreateSRV(tex2D.Get(), &srvDesc, &m_ShadowSRV);

	// RenderTarget 생성..
	m_ShadowRT = g_Factory->CreateBasicRenderTarget(nullptr, &m_ShadowSRV);

	// Texture2D Resource Reset..
	RESET_COM(tex2D);
}

void ShadowRender::OnResize(int width, int height)
{
	// Shadow DepthStencilView 재설성..
	m_ShadowDSV = m_ShadowDepthStencilView->GetDSV();

	// Shadow ShaderResourceView 재설성..
	m_ShadowSRV = m_ShadowRT->GetSRV();
}

void ShadowRender::Render()
{
	g_Context->OMSetBlendState(0, 0, 0xffffffff);
	g_Context->RSSetViewports(1, m_ShadowViewport);

	/// 실제 렌더링 추가

	//g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//g_Context->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &m_Stride, &m_Offset);
	//g_Context->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
	//
	//// Draw..
	//g_Context->DrawIndexed(m_IndexCount, 0, 0);

	// 그리기만 할 것이므로 null Render Target 설정..
	// 깊이 버퍼, null Rendering 대상을 설정하면 색상 쓰기가 비활성화 된다..
	g_Context->OMSetRenderTargets(0, nullptr, m_ShadowDSV);
	g_Context->ClearDepthStencilView(m_ShadowDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
