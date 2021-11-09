#include "DirectDefine.h"
#include "RenderBase.h"
#include "ShaderBase.h"
#include "ShaderResourceBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture2D.h"
#include "DepthStencilView.h"
#include "RenderTargetBase.h"
#include "BasicRenderTarget.h"
#include "DeferredRender.h"

#include "MathDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"

DeferredRender::DeferredRender()
{

}

DeferredRender::~DeferredRender()
{

}

void DeferredRender::Initialize(int width, int height)
{
	// Shader 설정..
	m_MeshVS = reinterpret_cast<VertexShader*>(g_Shader->GetShader("NormalSkinVS"));
	m_SkinVS = reinterpret_cast<VertexShader*>(g_Shader->GetShader("NormalTextureVS"));
	m_DeferredPS = reinterpret_cast<PixelShader*>(g_Shader->GetShader("NormalTextureDeferredPS"));
	
	// DepthStencilView 설정..
	m_DSV = g_Resource->GetDepthStencilView(eDepthStencilView::DEFALT);
	m_DepthStencilView = m_DSV->GetDSV();

	m_DepthStencilState = g_Resource->GetDepthStencilState(eDepthStencilState::DEFALT);
	m_RasterizerState = g_Resource->GetRasterizerState(eRasterizerState::SOLID);
	m_BlendState = g_Resource->GetBlendState(eBlendState::BLEND_ONE);

	// ViewPort 설정..
	m_ScreenViewport = g_Resource->GetViewPort(eViewPort::DEFALT);

	///////////////////////////////////////////////////////////////////////////
	// Texture 2D
	///////////////////////////////////////////////////////////////////////////
	D3D11_TEXTURE2D_DESC texDescDiffuse;
	ZeroMemory(&texDescDiffuse, sizeof(texDescDiffuse));
	texDescDiffuse.Width = width;
	texDescDiffuse.Height = height;
	texDescDiffuse.MipLevels = 1;
	texDescDiffuse.ArraySize = 1;
	texDescDiffuse.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDescDiffuse.SampleDesc.Count = 1;
	texDescDiffuse.SampleDesc.Quality = 0;
	texDescDiffuse.Usage = D3D11_USAGE_DEFAULT;
	texDescDiffuse.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDescDiffuse.CPUAccessFlags = 0;
	texDescDiffuse.MiscFlags = 0;

	D3D11_TEXTURE2D_DESC texDescPosNormal;
	ZeroMemory(&texDescPosNormal, sizeof(texDescPosNormal));
	texDescPosNormal.Width = width;
	texDescPosNormal.Height = height;
	texDescPosNormal.MipLevels = 1;
	texDescPosNormal.ArraySize = 1;
	texDescPosNormal.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDescPosNormal.SampleDesc.Count = 1;
	texDescPosNormal.SampleDesc.Quality = 0;
	texDescPosNormal.Usage = D3D11_USAGE_DEFAULT;
	texDescPosNormal.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDescPosNormal.CPUAccessFlags = 0;
	texDescPosNormal.MiscFlags = 0;

	// Texture 2D 생성..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D[5] = { nullptr, };
	g_Factory->CreateTexture2D(&texDescDiffuse, tex2D[0].GetAddressOf());
	g_Factory->CreateTexture2D(&texDescPosNormal, tex2D[1].GetAddressOf());
	g_Factory->CreateTexture2D(&texDescPosNormal, tex2D[2].GetAddressOf());
	g_Factory->CreateTexture2D(&texDescPosNormal, tex2D[3].GetAddressOf());
	g_Factory->CreateTexture2D(&texDescPosNormal, tex2D[4].GetAddressOf());

	///////////////////////////////////////////////////////////////////////////
	// RenderTargetView 2D
	///////////////////////////////////////////////////////////////////////////
	D3D11_RENDER_TARGET_VIEW_DESC rtvDescDiffuse;
	ZeroMemory(&rtvDescDiffuse, sizeof(rtvDescDiffuse));
	rtvDescDiffuse.Format = texDescDiffuse.Format;
	rtvDescDiffuse.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDescDiffuse.Texture2D.MipSlice = 0;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDescPosNormal;
	ZeroMemory(&rtvDescPosNormal, sizeof(rtvDescPosNormal));
	rtvDescPosNormal.Format = texDescPosNormal.Format;
	rtvDescPosNormal.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDescPosNormal.Texture2D.MipSlice = 0;

	// RenderTargetView 생성..
	m_RTVList.resize(5);
	g_Factory->CreateRTV(tex2D[0].Get(), &rtvDescDiffuse, &m_RTVList[0]);
	g_Factory->CreateRTV(tex2D[1].Get(), &rtvDescPosNormal, &m_RTVList[1]);
	g_Factory->CreateRTV(tex2D[2].Get(), &rtvDescPosNormal, &m_RTVList[2]);
	g_Factory->CreateRTV(tex2D[3].Get(), &rtvDescPosNormal, &m_RTVList[3]);
	g_Factory->CreateRTV(tex2D[4].Get(), &rtvDescPosNormal, &m_RTVList[4]);

	///////////////////////////////////////////////////////////////////////////
	// ShaderResourceView 2D
	///////////////////////////////////////////////////////////////////////////
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDescDiffuse;
	ZeroMemory(&srvDescDiffuse, sizeof(srvDescDiffuse));
	srvDescDiffuse.Format = texDescDiffuse.Format;
	srvDescDiffuse.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDescDiffuse.Texture2D.MostDetailedMip = 0;
	srvDescDiffuse.Texture2D.MipLevels = 1;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDescPosNormal;
	ZeroMemory(&srvDescPosNormal, sizeof(srvDescPosNormal));
	srvDescPosNormal.Format = texDescPosNormal.Format;
	srvDescPosNormal.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDescPosNormal.Texture2D.MostDetailedMip = 0;
	srvDescPosNormal.Texture2D.MipLevels = 1;

	// ShaderResourceView 생성..
	m_SRVList.resize(5);
	g_Factory->CreateSRV(tex2D[0].Get(), &srvDescDiffuse, &m_SRVList[0]);
	g_Factory->CreateSRV(tex2D[1].Get(), &srvDescPosNormal, &m_SRVList[1]);
	g_Factory->CreateSRV(tex2D[2].Get(), &srvDescPosNormal, &m_SRVList[2]);
	g_Factory->CreateSRV(tex2D[3].Get(), &srvDescPosNormal, &m_SRVList[3]);
	g_Factory->CreateSRV(tex2D[4].Get(), &srvDescPosNormal, &m_SRVList[4]);

	// RenderTarget 생성..
	m_AlbedoRT		= g_Factory->CreateBasicRenderTarget(&m_RTVList[0], &m_SRVList[0]);
	m_NormalRT		= g_Factory->CreateBasicRenderTarget(&m_RTVList[1], &m_SRVList[1]);
	m_PositionRT	= g_Factory->CreateBasicRenderTarget(&m_RTVList[2], &m_SRVList[2]);
	m_ShadowRT		= g_Factory->CreateBasicRenderTarget(&m_RTVList[3], &m_SRVList[3]);
	m_NormalDepthRT = g_Factory->CreateBasicRenderTarget(&m_RTVList[4], &m_SRVList[4]);

	// Texture2D Resource Reset..
	RESET_COM(tex2D[0]);
	RESET_COM(tex2D[1]);
	RESET_COM(tex2D[2]);
	RESET_COM(tex2D[3]);
	RESET_COM(tex2D[4]);
}

void DeferredRender::OnResize(int width, int height)
{
	// DepthStencilView 재설성..
	m_DepthStencilView = m_DSV->GetDSV();
	
	// ShaderResourceView List 재설정..
	m_SRVList[0] = m_AlbedoRT->GetSRV();
	m_SRVList[1] = m_NormalRT->GetSRV();
	m_SRVList[2] = m_PositionRT->GetSRV();
	m_SRVList[3] = m_ShadowRT->GetSRV();
	m_SRVList[4] = m_NormalDepthRT->GetSRV();

	// RenderTargetView List 재설정..
	m_RTVList[0] = m_AlbedoRT->GetRTV();
	m_RTVList[1] = m_NormalRT->GetRTV();
	m_RTVList[2] = m_PositionRT->GetRTV();
	m_RTVList[3] = m_ShadowRT->GetRTV();
	m_RTVList[4] = m_NormalDepthRT->GetRTV();
}

void DeferredRender::Render(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMMATRIX world, ID3D11Buffer* vb, ID3D11Buffer* ib, const UINT size, const UINT offset, UINT indexCount)
{
	g_Context->OMSetRenderTargets(5, &m_RTVList[0], m_DepthStencilView);

	// RenderTarget 초기화..
	g_Context->ClearRenderTargetView(m_RTVList[0], reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->ClearRenderTargetView(m_RTVList[1], reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->ClearRenderTargetView(m_RTVList[2], reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->ClearRenderTargetView(m_RTVList[3], reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->ClearRenderTargetView(m_RTVList[4], reinterpret_cast<const float*>(&DXColors::DeepDarkGray));

	g_Context->RSSetViewports(1, m_ScreenViewport);

	g_Context->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	g_Context->OMSetDepthStencilState(m_DepthStencilState, 0);
	
	g_Context->OMSetBlendState(m_BlendState, 0, 0xffffffff);

	g_Context->RSSetState(m_RasterizerState);

	// Shader Update
	cbPerObject objData;
	objData.gWorld = world;


	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_Context->IASetVertexBuffers(0, 1, &vb, &size, &offset);
	g_Context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

	// Draw..
	g_Context->DrawIndexed(indexCount, 0, 0);
}