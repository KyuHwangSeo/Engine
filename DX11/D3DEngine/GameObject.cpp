#include "D3DDefine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "KHMath.h"
#include "Shader.h"
#include "Material.h"
#include "Renderer.h"
#include "MeshRenderer.h"
#include "SkinMeshRenderer.h"
#include "CanvasRenderer.h"

GameObject::GameObject()
	:m_MeshType(eMeshType::None), m_Tag(eTag::Defalt), m_Layer(LayerList::GetLayer()), m_Picking(false), m_Enable(true)
{
	m_Transform = new Transform();
	m_Transform->SetGameObject(this);
}

GameObject::~GameObject()
{

}

void GameObject::Start()
{
	// ��� ������Ʈ ������ ���� ������Ʈ ����..
	for (auto& com : m_Components)
	{
		com.second->Start();
	}

	m_Transform->Start();
}

void GameObject::Update(float dTime)
{
	// GameObject Ȱ��ȭ ������ ��츸 ����..
	if (m_Enable == false) return;

	// ��� ������Ʈ ������ ������Ʈ ����..
	for (auto& com : m_Components)
	{
		// Ȱ��ȭ �� ������Ʈ���� üũ..
		if (com.second->GetEnabled() == false)	continue;

		com.second->Update(dTime);
	}

	m_Transform->Update(dTime);

	// �� ���� ������Ʈ..
	LateUpdate(dTime);
}

void GameObject::LateUpdate(float dTime)
{
	// GameObject Ȱ��ȭ ������ ��츸 ����..
	if (m_Enable == false) return;
	
	// ��� ������Ʈ ������ �� ���� ������Ʈ ����..
	for (auto& com : m_Components)
	{
		// Ȱ��ȭ �� ������Ʈ���� üũ..
		if (com.second->GetEnabled() == false)	continue;

		com.second->LateUpdate(dTime);
	}
}

void GameObject::FinalUpdate(float dTime)
{
	// GameObject Ȱ��ȭ ������ ��츸 ����..
	if (m_Enable == false) return;
	
	// ��� ������Ʈ ������ �� ���� ������Ʈ ����..
	for (auto& com : m_Components)
	{
		// Ȱ��ȭ �� ������Ʈ���� üũ..
		if (com.second->GetEnabled() == false)	continue;

		com.second->FinalUpdate(dTime);
	}
}

void GameObject::CollisionStart(Collision collsion)
{
	// GameObject Ȱ��ȭ ������ ��츸 ����..
	if (m_Enable == false) return;

	// GameObject Rendering ����..
	for (auto& com : m_Components)
	{
		// Ȱ��ȭ �� ������Ʈ���� üũ..
		if (com.second->GetEnabled() == false)	continue;

		com.second->CollisionStart(collsion);
	}
}

void GameObject::CollisionStay(Collision collsion)
{
	// GameObject Ȱ��ȭ ������ ��츸 ����..
	if (m_Enable == false) return;

	// GameObject Rendering ����..
	for (auto& com : m_Components)
	{
		// Ȱ��ȭ �� ������Ʈ���� üũ..
		if (com.second->GetEnabled() == false)	continue;

		com.second->CollisionStay(collsion);
	}
}

void GameObject::CollisionExit(Collision collsion)
{
	// GameObject Ȱ��ȭ ������ ��츸 ����..
	if (m_Enable == false) return;

	// GameObject Rendering ����..
	for (auto& com : m_Components)
	{
		// Ȱ��ȭ �� ������Ʈ���� üũ..
		if (com.second->GetEnabled() == false)	continue;

		com.second->CollisionExit(collsion);
	}
}

void GameObject::ShadowRender(DXMatrix4X4 view, DXMatrix4X4 proj)
{
	// GameObject Ȱ��ȭ ������ ��츸 ����..
	if (m_Enable == false) return;

	// GameObject Rendering ����..
	for (auto& com : m_Components)
	{
		// Ȱ��ȭ �� ������Ʈ���� üũ..
		if (com.second->GetEnabled() == false)	continue;

		com.second->ShadowRender(view, proj);
	}
}

void GameObject::Render(DXMatrix4X4 view, DXMatrix4X4 proj)
{
	// GameObject Ȱ��ȭ ������ ��츸 ����..
	if (m_Enable == false) return;

	// GameObject Rendering ����..
	for (auto& com : m_Components)
	{		
		// Ȱ��ȭ �� ������Ʈ���� üũ..
		if (com.second->GetEnabled() == false)	continue;

		com.second->Render(view, proj);
	}

	m_Transform->Render(view, proj);
}

void GameObject::Release()
{
	SAFE_RELEASE(m_Transform);

	for (auto& com : m_Components)
	{
		SAFE_RELEASE(com.second);
	}

	m_Components.clear();
}

bool GameObject::Picking(DXMatrix4X4 view, DXMatrix4X4 proj, int sw, int sh, int x, int y)
{
	switch (m_MeshType)
	{
	case eMeshType::NormalMesh:
		return GetComponent<MeshRenderer>()->Pick(view, proj, sw, sh, x, y);
	case eMeshType::SkinMesh:
		return GetComponent<SkinMeshRenderer>()->Pick(view, proj, sw, sh, x, y);
	case eMeshType::Canvas:
		return GetComponent<CanvasRenderer>()->Pick(view, proj, sw, sh, x, y);
	default:
		return false;
	}
}
