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
#include "LightRender.h"

#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"

LightRender::LightRender()
{

}

LightRender::~LightRender()
{

}

void LightRender::Initialize(int width, int height)
{
	// Shader 설정..
	m_ScreenVS = reinterpret_cast<VertexShader*>(g_Shader->GetShader("FullScreenVS"));
	m_ScreenPS = reinterpret_cast<PixelShader*>(g_Shader->GetShader("LightPS"));

	// DepthStencilView 설정..
	m_DepthStencilView = g_Resource->GetDepthStencilView(eDepthStencilView::DEFALT);

	// ViewPort 설정..
	m_ScreenViewport = g_Resource->GetViewPort(eViewPort::SCREEN);

	// BackBuffer 생성..
	ComPtr<ID3D11Texture2D> tex2D = nullptr;
	tex2D = g_Factory->CreateBackBuffer(width, height);

	// BackBuffer RTV 생성..
	m_BackBufferRTV = g_Factory->CreateRTV(tex2D, nullptr);
	m_BackBufferSRV = g_Factory->CreateSRV(tex2D, nullptr);
	g_Factory->CreateMainRenderTarget(m_BackBufferRTV, m_BackBufferSRV);

	// Texture2D Resource Reset..
	RESET_COM(tex2D);
}

void LightRender::OnResize(int width, int height)
{

}
