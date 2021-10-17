#include "D3DDefine.h"
#include "D3DEngine.h"
#include "IRenderer.h"
#include "Object.h"
#include "Shader.h"
#include "RenderTarget.h"
#include "Forward.h"
#include "PostProcessing.h"
#include "Deferred.h"
#include "Shadow.h"
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "SSAO.h"
#include "DebugViewPort.h"
#include "MotionBlur.h"
#include "ResourceManager.h"
#include "Light.h"
#include "DepthStencilState.h"
#include "DepthStencilView.h"
#include "DepthStencilManager.h"
#include "D3DRenderer.h"

D3DRenderer::D3DRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context, ComPtr<IDXGISwapChain> swapchain)
	:m_Device(device), m_DeviceContext(context), m_SwapChain(swapchain)
{
	m_DSManger = new DepthStencilManager();
	m_DSManger->Initialize();

	m_Forward = new Forward(device, context, swapchain);
	m_PostProcessing = new PostProcessing(device, context);
	m_Shadow = new Shadow(device, context);
	m_Deferred = new Deferred(device, context);
	m_SSAO = new SSAO(device, context);
	//m_MotionBlur = new MotionBlur(device, context);

	m_DebugViewPort = new DebugViewPort(device, context);

	m_MotionBlurShader = D3DEngine::GetInstance()->GetResourceManager()->GetShader("MotionBlurShader");
	m_QuadBuffer = D3DEngine::GetInstance()->GetResourceManager()->GetVertexBuffer("Quad");

	m_RendererList.emplace_back(m_Forward);
	m_RendererList.emplace_back(m_PostProcessing);
	m_RendererList.emplace_back(m_Shadow);
	m_RendererList.emplace_back(m_Deferred);
	m_RendererList.emplace_back(m_SSAO);
	//m_RendererList.emplace_back(m_MotionBlur);
	m_RendererList.emplace_back(m_DebugViewPort);
}

D3DRenderer::~D3DRenderer()
{
	for (IRenderer* render : m_RendererList)
	{
		SAFE_DELETE(render);
	}
}

void D3DRenderer::Initialize()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HR(m_Device->CreateBlendState(&blendDesc, m_AlphaBlend.GetAddressOf()));
}

void D3DRenderer::MainRender()
{
	m_DepthStencilView = m_DSManger->GetDSV("Defalt");
	m_DepthStencilState = m_DSManger->GetDSS("Defalt");

	/// Deferred Rendering
	if (Is_Deferred)
		m_Deferred->BeginRender(m_DepthStencilView);
	/// Forward Rendering
	else
		m_Forward->BeginRender(m_DepthStencilView);

	// ViewPort 설정..
	m_DeviceContext->RSSetViewports(1, &m_Forward->GetViewPort());

	// DepthStencil 초기화..
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// DepthStencil 설정..
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 0);

	// Blend 설정..
	m_DeviceContext->OMSetBlendState(m_AlphaBlend.Get(), 0, 0xffffffff);

	// Shadow Resource 설정..
	if (Is_Shadow)
		Shader::SetShadowMap(m_Shadow->m_ShadowRT->GetSRV());
	else
		Shader::SetShadowMap(nullptr);
}

void D3DRenderer::SSAORender()
{
	if (!Is_SSAO) return;

	m_RenderTarget = m_SSAO->m_AmbientRT->GetRTV();

	m_DeviceContext->OMSetBlendState(0, 0, 0xffffffff);
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTarget, 0);
	m_DeviceContext->ClearRenderTargetView(m_RenderTarget, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	m_DeviceContext->RSSetViewports(1, &m_SSAO->GetViewPort());

	// SSAO Render
	m_SSAO->SsaoRender(m_Deferred->m_NormalDepthRT->GetSRV(), m_Deferred->m_DepthRT->GetSRV());
	m_SSAO->BlurRender(m_Deferred->m_NormalDepthRT->GetSRV(), m_Deferred->m_DepthRT->GetSRV(), 4);
}

void D3DRenderer::PostRender(RenderTarget* finalRT, RenderTarget* horzRT, RenderTarget* vertRT)
{
	m_RenderTarget = finalRT->GetRTV();
	m_DepthStencilView = m_DSManger->GetDSV("Defalt");
	m_DepthStencilState = m_DSManger->GetDSS("Defalt");

	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTarget, m_DepthStencilView);
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 0);

	// Blur Render
	m_PostProcessing->Render(horzRT, vertRT);

	m_DeviceContext->ClearRenderTargetView(m_RenderTarget, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Final Render
	m_Forward->FinalRender(vertRT->GetSRV());
}

void D3DRenderer::FullScreenBlur()
{
	PostRender(m_Forward->m_ForwardRT, m_Forward->m_AllHorzBlurRT, m_Forward->m_AllVertBlurRT);
}

void D3DRenderer::ShadowRender()
{
	m_Shadow->Render();
}

void D3DRenderer::DeferredRender()
{
	if (Is_Blur)
		m_RenderTarget = m_Forward->m_AllHorzBlurRT->GetRTV();
	else
		m_RenderTarget = m_Forward->m_ForwardRT->GetRTV();

	m_DepthStencilView = m_DSManger->GetDSV("Defalt");

	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTarget, m_DepthStencilView);
	m_DeviceContext->ClearRenderTargetView(m_RenderTarget, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	m_DeviceContext->RSSetViewports(1, &m_Forward->GetViewPort());

	m_Deferred->LightRender(m_SSAO->m_AmbientRT->GetSRV());
}

void D3DRenderer::MotionBlurRender()
{
	m_DepthStencilView = m_DSManger->GetDSV("Defalt");
	
	m_RenderTarget = m_MotionBlur->m_VelocityRT->GetRTV();
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTarget, m_DepthStencilView);
	m_DeviceContext->ClearRenderTargetView(m_RenderTarget, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_MotionBlur->VelocityRender(m_Deferred->m_DepthRT->GetSRV());

	m_RenderTarget = m_MotionBlur->m_MotionBlurRT->GetRTV();
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTarget, m_DepthStencilView);
	m_DeviceContext->ClearRenderTargetView(m_RenderTarget, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_MotionBlur->MotionBlurRender(m_Forward->m_ForwardRT->GetSRV());

	m_RenderTarget = m_Forward->m_ForwardRT->GetRTV();
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTarget, m_DepthStencilView);
	m_DeviceContext->ClearRenderTargetView(m_RenderTarget, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_Forward->FinalRender(m_MotionBlur->m_MotionBlurRT->GetSRV());
}

void D3DRenderer::DebugRender()
{
#ifdef _DEBUG
	m_DepthStencilView = m_DSManger->GetDSV("Defalt");
	m_DepthStencilState = m_DSManger->GetDSS("Defalt");

	// DepthStencil 초기화..
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// DepthStencil 설정..
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 0);

	/// Deferred Debug..
	m_DebugViewPort->SetViewPort(1, 0);
	m_Forward->FinalRender(m_Deferred->GetDeferredSRV(0));

	m_DebugViewPort->SetViewPort(1, 1);
	m_Forward->FinalRender(m_Deferred->GetDeferredSRV(1));

	m_DebugViewPort->SetViewPort(1, 2);
	m_Forward->FinalRender(m_Deferred->GetDeferredSRV(2));

	m_DebugViewPort->SetViewPort(1, 3);
	m_Forward->FinalRender(m_Deferred->GetDeferredSRV(3));

	m_DebugViewPort->SetViewPort(1, 4);
	m_Forward->FinalRender(m_Deferred->GetDeferredSRV(4));

	m_DebugViewPort->SetViewPort(1, 5);
	m_Forward->FinalRender(m_Deferred->GetDeferredSRV(6));
	//m_Forward->FinalRender(m_Shadow->m_ShadowRT->GetSRV());

	//m_DebugViewPort->SetViewPort(1, 6);
	//m_Forward->FinalRender(m_MotionBlur->m_VelocityRT->GetSRV());
	//m_Forward->FinalRender(m_Deferred->GetDeferredSRV(6));

	/// SSAO Debug..
	m_DebugViewPort->SetViewPort(1, 6);
	m_Forward->FinalRender(m_SSAO->m_AmbientRT->GetSRV());

	// Viewport 초기화..
	m_DeviceContext->RSSetViewports(1, &m_Forward->GetViewPort());

#endif
}

void D3DRenderer::ResetSSAO()
{
	m_RenderTarget = m_SSAO->m_AmbientRT->GetRTV();
	m_DeviceContext->ClearRenderTargetView(m_RenderTarget, reinterpret_cast<const float*>(&DXColors::White));
}

void D3DRenderer::AlphaRender()
{
	m_DepthStencilState = m_DSManger->GetDSS("NoDepth"); 
	m_DeviceContext->OMSetBlendState(m_AlphaBlend.Get(), 0, 0xffffffff);
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 0);
}

void D3DRenderer::EndRender()
{
	m_DeviceContext->RSSetState(0);
	m_DeviceContext->OMSetDepthStencilState(0, 0);
}

void D3DRenderer::OnResize(int width, int height)
{
	assert(m_DeviceContext);
	assert(m_Device);
	assert(m_SwapChain);

	// DepthStencil 생성..
	m_DSManger->OnResize(width, height);

	for (IRenderer* renderer : m_RendererList)
	{
		renderer->OnResize(width, height);
	}
}
