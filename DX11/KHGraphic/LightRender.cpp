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

#include "MathDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"

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
	m_DSV = g_Resource->GetDepthStencilView(eDepthStencilView::DEFALT);
	m_DepthStencilView = m_DSV->GetDSV();

	m_DepthStencilState = g_Resource->GetDepthStencilState(eDepthStencilState::DEFALT);
	m_RasterizerState = g_Resource->GetRasterizerState(eRasterizerState::SOLID);
	m_BlendState = g_Resource->GetBlendState(eBlendState::BLEND_ONE);

	// ViewPort 설정..
	m_ScreenViewport = g_Resource->GetViewPort(eViewPort::DEFALT);

	// BackBuffer 생성..
	m_BackBuffer = g_Factory->CreateMainRenderTarget(width, height);
	m_BackBufferRTV = m_BackBuffer->GetRTV();
	m_BackBufferSRV = m_BackBuffer->GetSRV();
}

void LightRender::OnResize(int width, int height)
{
	// BackBuffer RenderTargetView 재설정..
	m_BackBufferRTV = m_BackBuffer->GetRTV();

	// BackBuffer ShaderResourceView 재설정..
	m_BackBufferSRV = m_BackBuffer->GetSRV();

	// DepthStencilView 재설성..
	m_DepthStencilView = m_DSV->GetDSV();

	// ShaderResource 재설정..
	m_ScreenPS->SetShaderResourceView<AlbedoSRV>(m_AlbedoSRV);
	m_ScreenPS->SetShaderResourceView<NormalSRV>(m_NormalSRV);
	m_ScreenPS->SetShaderResourceView<PositionSRV>(m_PositionSRV);
	m_ScreenPS->SetShaderResourceView<NormalSRV>(m_ShadowSRV);
	m_ScreenPS->SetShaderResourceView<SsaoSRV>(m_SSAOSRV);
}

void LightRender::Render()
{

}
