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

#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"

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
	m_DepthStencilView = g_Resource->GetDepthStencilView(eDepthStencilView::DEFALT);
	
	// ViewPort 설정..
	m_ScreenViewport = g_Resource->GetViewPort(eViewPort::SCREEN);

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

	ComPtr<ID3D11Texture2D> tex2D[5] = { nullptr, };

	// Texture 2D 생성..
	tex2D[0] = g_Factory->CreateTexture2D(&texDescDiffuse);
	tex2D[1] = g_Factory->CreateTexture2D(&texDescPosNormal);
	tex2D[2] = g_Factory->CreateTexture2D(&texDescPosNormal);
	tex2D[3] = g_Factory->CreateTexture2D(&texDescPosNormal);
	tex2D[4] = g_Factory->CreateTexture2D(&texDescPosNormal);

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
	std::vector<ID3D11RenderTargetView*> rtvList(5);
	rtvList[0] = g_Factory->CreateRTV(tex2D[0], &rtvDescDiffuse).Get();
	rtvList[1] = g_Factory->CreateRTV(tex2D[1], &rtvDescPosNormal).Get();
	rtvList[2] = g_Factory->CreateRTV(tex2D[2], &rtvDescPosNormal).Get();
	rtvList[3] = g_Factory->CreateRTV(tex2D[3], &rtvDescPosNormal).Get();
	rtvList[4] = g_Factory->CreateRTV(tex2D[4], &rtvDescPosNormal).Get();

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
	std::vector<ID3D11ShaderResourceView*> srvList(5);
	srvList[0] = g_Factory->CreateSRV(tex2D[0], &srvDescDiffuse).Get();
	srvList[1] = g_Factory->CreateSRV(tex2D[1], &srvDescPosNormal).Get();
	srvList[2] = g_Factory->CreateSRV(tex2D[2], &srvDescPosNormal).Get();
	srvList[3] = g_Factory->CreateSRV(tex2D[3], &srvDescPosNormal).Get();
	srvList[4] = g_Factory->CreateSRV(tex2D[4], &srvDescPosNormal).Get();

	// Graphic Resource 설정..
	m_SRVList.swap(srvList);
	m_RTVList.swap(rtvList);

	// Texture2D Resource Reset..
	RESET_COM(tex2D[0]);
	RESET_COM(tex2D[1]);
	RESET_COM(tex2D[2]);
	RESET_COM(tex2D[3]);
	RESET_COM(tex2D[4]);
}

void DeferredRender::OnResize(int width, int height)
{

}
