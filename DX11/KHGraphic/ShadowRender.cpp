#include "DirectDefine.h"
#include "RenderBase.h"
#include "ShaderBase.h"
#include "ShaderResourceBase.h"
#include "VertexShader.h"
#include "ShadowRender.h"

ShadowRender::ShadowRender()
	:m_ShadowDSV(nullptr), m_ShadowSRV(nullptr)
{
}

ShadowRender::~ShadowRender()
{

}

void ShadowRender::Initialize()
{

}

void ShadowRender::RenderFrame()
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
	g_Context->OMSetRenderTargets(0, nullptr, m_ShadowDSV.Get());
	g_Context->ClearDepthStencilView(m_ShadowDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}
