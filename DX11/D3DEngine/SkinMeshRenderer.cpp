#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Material.h"
#include "Renderer.h"
#include "SkinMeshRenderer.h"

SkinMeshRenderer::SkinMeshRenderer(eRasterizerType renderType, eTopologyType topolType)
	:Renderer(renderType, topolType)
{
	SetName<SkinMeshRenderer>();

	m_ShadowShader = D3DEngine::GetInstance()->GetResourceManager()->GetShader("SkinShadowShader");
}

SkinMeshRenderer::~SkinMeshRenderer()
{

}

void SkinMeshRenderer::ShadowRender(DXMatrix4X4 view, DXMatrix4X4 proj)
{
	DXMatrix4X4 world = m_GameObject->GetWorld();
	
	m_ShadowObjectData.gWorldViewProj = world * view * proj;
	m_ShadowObjectData.gTexTransform = m_Material->GetTexTransform();
	m_ShadowShader->SetVertexConstantBuffer(m_ShadowObjectData);

	// BoneTM Update..
	BoneUpdate();
	m_ShadowShader->SetVertexConstantBuffer(m_BoneData);

	// Vertex Shader Update
	m_ShadowShader->VSUpdate();

	// Pixel Shader Update
	//m_ShadowShader->PSUpdate();

	// Rendering Type
	D3DEngine::GetInstance()->SetDepth();

	// Rendering..
	Renderer::Render();
}

void SkinMeshRenderer::Render(DXMatrix4X4 view, DXMatrix4X4 proj)
{
	DXMatrix4X4 world = m_GameObject->GetWorld();
	DXMatrix4X4 prevWorld = m_GameObject->GetPrevWorld();
	DXMatrix4X4 worldViewProj = world * view * proj;
	DXMatrix4X4 worldInvTranspose = MathHelper::InverseTranspose(world);

	m_ObjectData.gWorld = world;
	m_ObjectData.gWorldInvTranspose = worldInvTranspose;
	m_ObjectData.gWorldViewProj = worldViewProj;
	m_ObjectData.gTexTransform = m_Material->GetTexTransform();
	m_ObjectData.gWorldView = world * view;
	m_ObjectData.gWorldInvTransposeView = worldInvTranspose * view;
	m_Material->SetVertexConstantBuffer(m_ObjectData);

	// BoneTM Update..
	BoneUpdate();

	// Material Update
	m_Material->SetVertexConstantBuffer(m_BoneData);
	m_Material->Update(world, view, proj);

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

void SkinMeshRenderer::Release()
{
	m_BoneTransList.clear();

	Renderer::Release();
}

void SkinMeshRenderer::SetMesh(ParserData::Mesh* mesh)
{
	Renderer::SetMesh(mesh);
}

void SkinMeshRenderer::SetBone(GameObject* bone)
{
	m_BoneTransList.push_back(bone->GetTransform());
}

void SkinMeshRenderer::BoneUpdate()
{
	for (size_t i = 0; i < m_Mesh->m_BoneTMList.size(); i++)
	{
		/// 최종 Bone TM -> BoneOffsetTM * BoneWorldTM (애니메이션 적용후의 TM)
		m_BoneData.gBoneTransforms[i] = m_Mesh->m_BoneTMList[i] * m_BoneTransList[i]->m_LocalTM;
	}
}
