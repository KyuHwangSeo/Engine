#include "KHMath.h"
#include "D3DDefine.h"
#include "D3DEngine.h"
#include "EnumDefine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Shader.h"
#include "Terrain.h"

void Terrain::Start()
{
	m_DeviceContext = D3DEngine::GetInstance()->GetDeviceContext();
}

void Terrain::ShadowRender(DXMatrix4X4 view, DXMatrix4X4 proj)
{

}

void Terrain::Render(DXMatrix4X4 view, DXMatrix4X4 proj)
{
	DXMatrix4X4 world = m_GameObject->GetWorld();
	DXMatrix4X4 prevWorld = m_GameObject->GetPrevWorld();
	DXMatrix4X4 worldViewProj = world * view * proj;
	DXMatrix4X4 worldInvTranspose = MathHelper::InverseTranspose(world);

	m_ObjectData.gWorld = world;
	m_ObjectData.gWorldInvTranspose = worldInvTranspose;
	m_ObjectData.gWorldViewProj = worldViewProj;
	m_ObjectData.gWorldView = world * view;
	m_ObjectData.gTexTransform = XMMatrixIdentity();
	m_ObjectData.gWorldInvTransposeView = worldInvTranspose * view;

	m_TerrainShader->SetVertexConstantBuffer(m_ObjectData);

	UINT index = 0;
	for (TerrainLayer* layer : m_LayerList)
	{
		m_TerrainShader->SetSRV(0 + index, layer->m_MaskTex.Get());
		m_TerrainShader->SetSRV(1 + index, layer->m_DiffuseTex.Get());
		m_TerrainShader->SetSRV(2 + index, layer->m_NormalMapTex.Get());
		index += 3;
	}

	// Vertex Shader Update
	m_TerrainShader->VSUpdate();

	// Pixel Shader Update
	m_TerrainShader->PSUpdate();

	D3DEngine::GetInstance()->SetSolid();

	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_DeviceContext->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &m_Stride, &m_Offset);
	m_DeviceContext->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Draw..
	m_DeviceContext->DrawIndexed(m_IndexCount, 0, 0);

	Shader::ResetVSResources();
	Shader::ResetPSResources();
}

void Terrain::Release()
{
	for (TerrainLayer* layer : m_LayerList)
	{
		layer->m_MaskTex.Reset();
		layer->m_DiffuseTex.Reset();
		layer->m_NormalMapTex.Reset();

		SAFE_DELETE(layer);
	}

	m_LayerList.clear();
}

void Terrain::SetVertexBuffer(VertexBuffer* vBuffer)
{
	if (vBuffer == nullptr) return;

	m_VB = vBuffer->VB;
	m_IB = vBuffer->IB;

	m_Stride = vBuffer->Stride;
	m_IndexCount = vBuffer->IndexCount;
	m_Offset = vBuffer->Offset;
}

void Terrain::SetShader(Shader* shader)
{
	m_TerrainShader = shader;
}

void Terrain::AddLayer(Texture mask, Texture diffuse, Texture normalmap)
{
	m_LayerList.push_back(new TerrainLayer(mask, diffuse, normalmap));
}
