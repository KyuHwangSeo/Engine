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
#include "EngineData.h"
#include "ForwardPass.h"

#include "MathDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"

ForwardPass::ForwardPass()
{

}

ForwardPass::~ForwardPass()
{

}

void ForwardPass::Initialize(int width, int height)
{
	// Shader 설정..
	m_MeshVS = g_Shader->GetShader("MeshVS");
	m_SkinVS = g_Shader->GetShader("SkinVS");
	m_ForwardPS = g_Shader->GetShader("ForwardPS");

	// DepthStencilView 설정..
	m_DSV = g_Resource->GetDepthStencilView(eDepthStencilView::DEFALT);
	m_DepthStencilView = m_DSV->GetDSV();

	m_DepthStencilState = g_Resource->GetDepthStencilState(eDepthStencilState::DEFALT);
	m_RasterizerState = g_Resource->GetRasterizerState(eRasterizerState::WIRE);
	m_BlendState = g_Resource->GetBlendState(eBlendState::BLEND_ONE);

	// ViewPort 설정..
	m_ScreenViewport = g_Resource->GetViewPort(eViewPort::SCREEN);

	// BackBuffer 생성..
	m_BackBuffer = g_Resource->GetMainRenderTarget();
	m_BackBufferRTV = m_BackBuffer->GetRTV();
	m_BackBufferSRV = m_BackBuffer->GetSRV();
}

void ForwardPass::OnResize(int width, int height)
{
	// BackBuffer RenderTargetView 재설정..
	m_BackBufferRTV = m_BackBuffer->GetRTV();

	// BackBuffer ShaderResourceView 재설정..
	m_BackBufferSRV = m_BackBuffer->GetSRV();

	// DepthStencilView 재설성..
	m_DepthStencilView = m_DSV->GetDSV();
}

void ForwardPass::Release()
{

}

void ForwardPass::BeginRender()
{
	g_Context->OMSetRenderTargets(1, &m_BackBufferRTV, m_DepthStencilView);
	g_Context->OMSetDepthStencilState(m_DepthStencilState, 0);
	g_Context->ClearRenderTargetView(m_BackBufferRTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	g_Context->RSSetViewports(1, m_ScreenViewport);
	g_Context->RSSetState(m_RasterizerState);
}

void ForwardPass::Update(MeshData* mesh, GlobalData* global)
{
	DirectX::XMMATRIX world = mesh->mWorld;
	DirectX::XMMATRIX view = *global->mViewMX;
	DirectX::XMMATRIX proj = *global->mProj;
	
	switch (mesh->ObjType)
	{
	case OBJECT_TYPE::Base:
	{
		cbPerObject objectBuf;
		objectBuf.gWorld = world * view * proj;
		m_MeshVS->SetConstantBuffer(objectBuf);

		// Vertex Shader Update..
		m_MeshVS->Update();

		// Pixel Shader Update..
		m_ForwardPS->Update();
	}
		break;
	case OBJECT_TYPE::Skinning:
	{
		cbPerObject objectBuf;
		objectBuf.gWorld = world * view * proj;
		m_SkinVS->SetConstantBuffer(objectBuf);

		cbSkinned skinBuf;
		for (int i = 0; i < mesh->BoneOffsetTM.size(); i++)
		{
			skinBuf.gBoneTransforms[i] = mesh->BoneOffsetTM[i];
		}
		m_SkinVS->SetConstantBuffer(skinBuf);

		// Vertex Shader Update..
		m_SkinVS->Update();

		// Pixel Shader Update..
		m_ForwardPS->Update();
	}
		break;
	default:
		break;
	}
}

void ForwardPass::Render(MeshData* mesh)
{
	ID3D11Buffer* iBuffer = reinterpret_cast<ID3D11Buffer*>(mesh->IB->IndexBufferPointer);
	ID3D11Buffer* vBuffer = reinterpret_cast<ID3D11Buffer*>(mesh->VB->VertexbufferPointer);

	UINT indexCount = mesh->IB->Count;
	UINT stride = mesh->VB->VertexDataSize;
	UINT offset = 0;

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	g_Context->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(indexCount, 0, 0);
}
