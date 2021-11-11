#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "ShaderResourceBase.h"
#include "VertexShader.h"
#include "ViewPort.h"
#include "Texture2D.h"
#include "DepthStencilView.h"
#include "RenderTargetBase.h"
#include "BasicRenderTarget.h"
#include "MathDefine.h"
#include "ShadowPass.h"

#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"

ShadowPass::ShadowPass()
	:m_ShadowDSV(nullptr), m_ShadowSRV(nullptr)
{
}

ShadowPass::~ShadowPass()
{

}

void ShadowPass::Initialize(int width, int height)
{
	// Shader ����..
	m_MeshShadowVS = g_Shader->GetShader("NormalShadowVS");
	m_SkinShadowVS = g_Shader->GetShader("SkinShadowVS");

	// ViewPort ����..
	m_ShadowViewport = g_Factory->CreateViewPort(0.0f, 0.0f, (float)width, (float)height, 4.0f, 4.0f);

	// DepthStencilView ����..
	m_ShadowDepthStencilView = g_Resource->GetDepthStencilView(eDepthStencilView::SHADOW);
	m_ShadowDepthStencilView->SetRatio(4.0f, 4.0f);

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

	// Texture 2D ����..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	g_Factory->CreateTexture2D(&texDesc, tex2D.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	// ShaderResourceView ����..
	g_Factory->CreateSRV(tex2D.Get(), &srvDesc, &m_ShadowSRV);

	// RenderTarget ����..
	m_ShadowRT = g_Factory->CreateBasicRenderTarget(nullptr, &m_ShadowSRV);
	m_ShadowRT->SetRatio(4.0f, 4.0f);

	// Texture2D Resource Reset..
	RESET_COM(tex2D);
}

void ShadowPass::OnResize(int width, int height)
{
	// Shadow DepthStencilView �缳��..
	m_ShadowDSV = m_ShadowDepthStencilView->GetDSV();

	// Shadow ShaderResourceView �缳��..
	m_ShadowSRV = m_ShadowRT->GetSRV();
}

void ShadowPass::Release()
{

}

void ShadowPass::BeginRender()
{
	g_Context->OMSetBlendState(0, 0, 0xffffffff);
	g_Context->RSSetViewports(1, m_ShadowViewport);

	// �׸��⸸ �� ���̹Ƿ� null Render Target ����..
	// ���� ����, null Rendering ����� �����ϸ� ���� ���Ⱑ ��Ȱ��ȭ �ȴ�..
	g_Context->OMSetRenderTargets(0, nullptr, m_ShadowDSV);
	g_Context->ClearDepthStencilView(m_ShadowDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void ShadowPass::Render(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMMATRIX world, ID3D11Buffer* vb, ID3D11Buffer* ib, UINT size, UINT offset, UINT indexCount)
{
	/// ���� ������ �߰�

	//g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//g_Context->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &m_Stride, &m_Offset);
	//g_Context->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
	//
	//// Draw..
	//g_Context->DrawIndexed(m_IndexCount, 0, 0);
}
