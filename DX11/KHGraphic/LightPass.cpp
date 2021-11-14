#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "ShaderResourceBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture2D.h"
#include "DepthStencilView.h"
#include "RenderTargetBase.h"
#include "BasicRenderTarget.h"
#include "VertexDefine.h"
#include "LightPass.h"

#include "MathDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"

LightPass::LightPass()
{

}

LightPass::~LightPass()
{

}

void LightPass::Initialize(int width, int height)
{
	// Buffer 설정..
	m_ScreenBuffer = g_Resource->GetBuffer(eBuffer::SCREEN);

	// Shader 설정..
	m_ScreenVS = g_Shader->GetShader("FullScreenVS");
	m_ScreenPS = g_Shader->GetShader("LightPS");

	// DepthStencilView 설정..
	m_DSV = g_Resource->GetDepthStencilView(eDepthStencilView::DEFALT);
	m_DepthStencilView = m_DSV->GetDSV();

	m_DepthStencilState = g_Resource->GetDepthStencilState(eDepthStencilState::DEFALT);
	m_RasterizerState = g_Resource->GetRasterizerState(eRasterizerState::SOLID);
	m_BlendState = g_Resource->GetBlendState(eBlendState::BLEND_ONE);

	// ViewPort 설정..
	m_ScreenViewport = g_Resource->GetViewPort(eViewPort::SCREEN);

	// BackBuffer 생성..
	m_BackBuffer = g_Resource->GetMainRenderTarget();
	m_BackBufferRTV = m_BackBuffer->GetRTV();
	m_BackBufferSRV = m_BackBuffer->GetSRV();
}

void LightPass::OnResize(int width, int height)
{
	// BackBuffer RenderTargetView 재설정..
	m_BackBufferRTV = m_BackBuffer->GetRTV();

	// BackBuffer ShaderResourceView 재설정..
	m_BackBufferSRV = m_BackBuffer->GetSRV();

	// DepthStencilView 재설성..
	m_DepthStencilView = m_DSV->GetDSV();

	// ShaderResource 재설정..
	m_ScreenPS->SetShaderResourceView<AlbedoSRV>(&m_AlbedoSRV);
	m_ScreenPS->SetShaderResourceView<NormalSRV>(&m_NormalSRV);
	m_ScreenPS->SetShaderResourceView<PositionSRV>(&m_PositionSRV);
	m_ScreenPS->SetShaderResourceView<NormalSRV>(&m_ShadowSRV);
	m_ScreenPS->SetShaderResourceView<SsaoSRV>(&m_SSAOSRV);
}

void LightPass::Release()
{

}

void LightPass::BeginRender()
{
	g_Context->OMSetRenderTargets(1, &m_BackBufferRTV, m_DepthStencilView);
	g_Context->ClearRenderTargetView(m_BackBufferRTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->RSSetViewports(1, m_ScreenViewport);
}

void LightPass::Render()
{
	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_ScreenBuffer->VB.GetAddressOf(), &m_ScreenBuffer->Stride, &m_ScreenBuffer->Offset);
	g_Context->IASetIndexBuffer(m_ScreenBuffer->IB.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Vertex Shader Update..
	m_ScreenVS->Update();

	// Pixel Shader Update..
	m_ScreenPS->Update();

	g_Context->DrawIndexed(m_ScreenBuffer->IndexCount, 0, 0);
}
