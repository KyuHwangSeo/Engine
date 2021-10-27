#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Material.h"
#include "Renderer.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(eRasterizerType renderType, eTopologyType topolType)
	:Renderer(renderType, topolType)
{
	SetName<MeshRenderer>();

	m_ShadowShader = D3DEngine::GetInstance()->GetResourceManager()->GetShader("NormalShadowShader");
}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::ShadowRender(DXMatrix4X4 view, DXMatrix4X4 proj)
{
	// Bone과 HelperObject는 Rendering 하지 않는다..
	if (m_GameObject->GetMeshType() == eMeshType::Bone || m_GameObject->GetMeshType() == eMeshType::Helper) return;

	DXMatrix4X4 world = m_GameObject->GetWorld();

	m_ShadowObjectData.gWorldViewProj = world * view * proj;
	m_ShadowObjectData.gTexTransform = m_Material->GetTexTransform();
	m_ShadowShader->SetVertexConstantBuffer(m_ShadowObjectData);

	// Vertex Shader Update
	m_ShadowShader->VSUpdate();

	// Pixel Shader Update
	//m_ShadowShader->PSUpdate();

	// Rendering Type
	D3DEngine::GetInstance()->SetDepth();

	// Rendering..
	Renderer::Render();
}

void MeshRenderer::Render(DXMatrix4X4 view, DXMatrix4X4 proj)
{
	// Bone과 HelperObject는 Rendering 하지 않는다..
	if (m_GameObject->GetMeshType() == eMeshType::Bone || m_GameObject->GetMeshType() == eMeshType::Helper) return;

	DXMatrix4X4 world = m_GameObject->GetWorld();
	DXMatrix4X4 prevWorld = m_GameObject->GetPrevWorld();
	DXMatrix4X4 worldViewProj = world * view * proj;
	DXMatrix4X4 worldInvTranspose = MathHelper::InverseTranspose(world);

	m_ObjectData.gWorld = world;
	m_ObjectData.gWorldInvTranspose = worldInvTranspose;
	m_ObjectData.gWorldViewProj = worldViewProj;
	m_ObjectData.gWorldView = world * view;
	m_ObjectData.gWorldInvTransposeView = worldInvTranspose * view;

	// Material & Shader Update
	for (auto& material : m_MaterialList)
	{
		m_ObjectData.gTexTransform = material.second->GetTexTransform();

		material.second->SetVertexConstantBuffer(m_ObjectData);
		material.second->Update(world, view, proj);
	}

	// Rendering Type
	switch (m_RasterizerType)
	{
	case eRasterizerType::Wire:
		D3DEngine::GetInstance()->SetWireFrame();
		break;
	case eRasterizerType::Solid:
		D3DEngine::GetInstance()->SetSolid();
		break;
	default:
		break;
	}

	// Rendering..
	Renderer::Render();
 }

