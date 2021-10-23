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
		m_TerrainShader->SetSRV(index, layer->m_MaskTex.Get());

		for (MaterialLayer& matLayer : layer->m_MatList)
		{
			index++;
			m_TerrainShader->SetSRV(index, matLayer.m_DiffuseTex.Get());
			index++;
			m_TerrainShader->SetSRV(index, matLayer.m_NormalMapTex.Get());
		}
		index++;
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

		for (MaterialLayer& matLayer : layer->m_MatList)
		{
			matLayer.m_DiffuseTex.Reset();
			matLayer.m_NormalMapTex.Reset();
		}

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

void Terrain::AddLayer(Texture mask, MaterialLayer& channel_R, MaterialLayer& channel_G, MaterialLayer& channel_B, MaterialLayer& channel_A)
{
	TerrainLayer* terrainLayer = new TerrainLayer(mask);
	terrainLayer->m_MatList.push_back(channel_R);
	terrainLayer->m_MatList.push_back(channel_G);
	terrainLayer->m_MatList.push_back(channel_B);
	terrainLayer->m_MatList.push_back(channel_A);

	m_LayerList.push_back(terrainLayer);
}

void Terrain::AddLayer(Texture mask, MaterialLayer& channel_R, MaterialLayer& channel_G, MaterialLayer& channel_B)
{


	TerrainLayer* terrainLayer = new TerrainLayer(mask);
	terrainLayer->m_MatList.push_back(channel_R);
	terrainLayer->m_MatList.push_back(channel_G);
	terrainLayer->m_MatList.push_back(channel_B);

	m_LayerList.push_back(terrainLayer);
}

void Terrain::AddLayer(Texture mask, MaterialLayer& channel_R, MaterialLayer& channel_G)
{
	TerrainLayer* terrainLayer = new TerrainLayer(mask);
	terrainLayer->m_MatList.push_back(channel_R);
	terrainLayer->m_MatList.push_back(channel_G);

	m_LayerList.push_back(terrainLayer);
}

void Terrain::AddLayer(Texture mask, MaterialLayer& channel_R)
{
	TerrainLayer* terrainLayer = new TerrainLayer(mask);
	terrainLayer->m_MatList.push_back(channel_R);

	m_LayerList.push_back(terrainLayer);
}
