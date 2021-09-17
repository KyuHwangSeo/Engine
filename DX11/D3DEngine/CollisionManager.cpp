#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Shader.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "BoxCollider2D.h"
#include "CircleCollider2D.h"

CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{

}

void CollisionManager::Update(float dTime)
{
	// 3D Collision Check
	for (auto& model_1 : m_ModelCollider3DList)
	{
		for (auto& model_2 : m_ModelCollider3DList)
		{
			if (model_1.second == model_2.second) continue;

			if (model_1.first == model_2.first)
			{
				CollisionCheck3D(model_1.second, model_2.second);
			}
		}
	}

	// 2D Collision Check
	for (auto& obj_1 : m_Collider2DList)
	{
		for (auto& obj_2 : m_Collider2DList)
		{
			if (obj_1.second == obj_2.second) continue;

			if (obj_1.first == obj_2.first)
			{
				CollisionCheck2D(obj_1.second, obj_2.second);
			}
		}
	}
}

void CollisionManager::CollisionCheck3D(ModelCollider& model_1, ModelCollider& model_2)
{
	for (Collider* col_1 : model_1.m_ColliderList)
	{
		if (model_1.m_TopObject->GetEnabled() == false) continue;

		for (Collider* col_2 : model_2.m_ColliderList)
		{
			if (model_2.m_TopObject->GetEnabled() == false) continue;

			if (col_1->CollisionCheck(col_2))
			{
				OnCollsion3D(model_1, model_2, col_1->GetGameObject(), col_2->GetGameObject());
				OnCollsion3D(model_2, model_1, col_2->GetGameObject(), col_1->GetGameObject());
				break;
			}
			else
			{
				NoneCollsion3D(model_1, model_2, col_1->GetGameObject(), col_2->GetGameObject());
				NoneCollsion3D(model_2, model_1, col_2->GetGameObject(), col_1->GetGameObject());
			}
		}
	}
}

void CollisionManager::CollisionCheck2D(UICollider& ui_1, UICollider& ui_2)
{
	Collider* col_1 = ui_1.m_Collider;
	Collider* col_2 = ui_2.m_Collider;

	if (col_1->CollisionCheck(col_2))
	{
		OnCollsion2D(ui_1, ui_2);
		OnCollsion2D(ui_2, ui_1);
	}
	else
	{
		NoneCollsion2D(ui_1, ui_2);
		NoneCollsion2D(ui_2, ui_1);
	}
}

void CollisionManager::OnCollsion3D(ModelCollider& model_1, ModelCollider& model_2, GameObject* parts, GameObject* target)
{
	switch (model_1.m_CollisionState)
	{
	case eCollisionState::None:
	{
		model_1.m_CollisionState = eCollisionState::Start;
		model_1.m_TopObject->CollisionStart(Collision(model_2.m_TopObject, target, parts, model_1.m_CollisionState));
	}
	break;
	case eCollisionState::Start:
	case eCollisionState::Stay:
	{
		model_1.m_CollisionState = eCollisionState::Stay;
		model_1.m_TopObject->CollisionStay(Collision(model_2.m_TopObject, target, parts, model_1.m_CollisionState));
	}
	break;
	case eCollisionState::Exit:
	{
		// 이전상태가 Exit일 경우는 없을것..
	}
	break;
	default:
		break;
	}
}

void CollisionManager::NoneCollsion3D(ModelCollider& model_1, ModelCollider& model_2, GameObject* parts, GameObject* target)
{
	switch (model_1.m_CollisionState)
	{
	case eCollisionState::None:
		break;
	case eCollisionState::Start:
	case eCollisionState::Stay:
	{
		model_1.m_CollisionState = eCollisionState::Exit;
		model_1.m_TopObject->CollisionExit(Collision(model_2.m_TopObject, target, parts, model_1.m_CollisionState));
	}
	break;
	case eCollisionState::Exit:
	{
		model_1.m_CollisionState = eCollisionState::None;
	}
	break;
	default:
		break;
	}
}

void CollisionManager::OnCollsion2D(UICollider& ui_1, UICollider& ui_2)
{
	switch (ui_1.m_CollisionState)
	{
	case eCollisionState::None:
	{
		ui_1.m_CollisionState = eCollisionState::Start;
		ui_1.m_Object->CollisionStart(Collision(ui_2.m_Object, ui_1.m_Object, ui_1.m_CollisionState));
	}
	break;
	case eCollisionState::Start:
	case eCollisionState::Stay:
	{
		ui_1.m_CollisionState = eCollisionState::Stay;
		ui_1.m_Object->CollisionStay(Collision(ui_2.m_Object, ui_1.m_Object, ui_1.m_CollisionState));
	}
	break;
	case eCollisionState::Exit:
	{
		// 이전상태가 Exit일 경우는 없을것..
	}
	break;
	default:
		break;
	}
}

void CollisionManager::NoneCollsion2D(UICollider& ui_1, UICollider& ui_2)
{
	switch (ui_1.m_CollisionState)
	{
	case eCollisionState::None:
		break;
	case eCollisionState::Start:
	case eCollisionState::Stay:
	{
		ui_1.m_CollisionState = eCollisionState::Exit;
		ui_1.m_Object->CollisionExit(Collision(ui_2.m_Object, ui_1.m_Object, ui_1.m_CollisionState));
	}
	break;
	case eCollisionState::Exit:
	{
		ui_1.m_CollisionState = eCollisionState::None;
	}
	break;
	default:
		break;
	}
}

void CollisionManager::AddModel(std::vector<GameObject*>& model)
{
	Collider* collider = nullptr;
	GameObject* topObject = *model.begin();

	Layer* layer = topObject->GetLayer();
	ModelCollider modelBox(topObject);

	for (GameObject* obj : model)
	{
		// 현재 모든 Collider 종류에 따라 GetComponent를 하며 비교하여 넣어주는데
		// 수정이 필요할 것 같다..
		collider = obj->GetComponent<BoxCollider>();

		if (collider == nullptr)
			collider = obj->GetComponent<SphereCollider>();

		if (collider == nullptr) continue;

		modelBox.m_ColliderList.push_back(collider);
	}

	m_ModelCollider3DList.insert(pair<Layer*, ModelCollider>(layer, modelBox));
}

void CollisionManager::AddObject3D(GameObject* obj)
{
	Collider* collider = nullptr;

	Layer* layer = obj->GetLayer();
	ModelCollider modelBox(obj);

	// 현재 모든 Collider 종류에 따라 GetComponent를 하며 비교하여 넣어주는데
	// 수정이 필요할 것 같다..
	collider = obj->GetComponent<BoxCollider>();

	if (collider == nullptr)
		collider = obj->GetComponent<SphereCollider>();

	if (collider == nullptr) return;

	modelBox.m_ColliderList.push_back(collider);

	m_ModelCollider3DList.insert(pair<Layer*, ModelCollider>(layer, modelBox));
}

void CollisionManager::AddObject2D(GameObject* obj)
{
	Collider* collider = nullptr;

	Layer* layer = obj->GetLayer();

	// 현재 모든 Collider 종류에 따라 GetComponent를 하며 비교하여 넣어주는데
	// 수정이 필요할 것 같다..
	collider = obj->GetComponent<BoxCollider2D>();

	if (collider == nullptr)
		collider = obj->GetComponent<CircleCollider2D>();

	if (collider == nullptr) return;

	m_Collider2DList.insert(pair<Layer*, UICollider>(layer, UICollider(obj, collider)));
}

void CollisionManager::Release()
{
	m_ModelCollider3DList.clear();
	m_Collider2DList.clear();
}
