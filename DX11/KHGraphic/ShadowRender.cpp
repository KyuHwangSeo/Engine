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

	/// ���� ������ �߰�

	//g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//g_Context->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &m_Stride, &m_Offset);
	//g_Context->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
	//
	//// Draw..
	//g_Context->DrawIndexed(m_IndexCount, 0, 0);

	// �׸��⸸ �� ���̹Ƿ� null Render Target ����..
	// ���� ����, null Rendering ����� �����ϸ� ���� ���Ⱑ ��Ȱ��ȭ �ȴ�..
	g_Context->OMSetRenderTargets(0, nullptr, m_ShadowDSV.Get());
	g_Context->ClearDepthStencilView(m_ShadowDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}
