#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "IManager.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Material.h"
#include "Renderer.h"
#include "MeshRenderer.h"
#include "SkinMeshRenderer.h"
#include "GameTimer.h"
#include "Animation.h"
#include "Animator.h"
#include "Gizmos.h"
#include "Camera.h"
#include "ObjectManager.h"

ObjectManager::ObjectManager()
	:m_PickObject(nullptr)
{

}

ObjectManager::~ObjectManager()
{

}

void ObjectManager::Start()
{
	for (Model model : m_ModelList)
	{
		for (pair<string, GameObject*> obj : model)
		{
			obj.second->Start();
		}
	}
}

void ObjectManager::Update(float dTime)
{
	// GameObject 추가 Update..
	//ObjectUpdate();

	for (Model model : m_ModelList)
	{
		if (model.begin()->second->GetEnabled() == false) continue;
		
		for (pair<string, GameObject*> obj : model)
		{
			obj.second->Update(dTime);
		}
	}
}

void ObjectManager::FinalUpdate(float dTime)
{
	for (Model model : m_ModelList)
	{
		if (model.begin()->second->GetEnabled() == false) continue;
		
		for (pair<string, GameObject*> obj : model)
		{
			obj.second->FinalUpdate(dTime);
		}
	}
}

void ObjectManager::ShadowRender(DXMatrix4X4 view, DXMatrix4X4 proj)
{
	for (Model model : m_ModelList)
	{
		if (model.begin()->second->GetEnabled() == false) continue;
		
		for (pair<string, GameObject*> obj : model)
		{
			obj.second->ShadowRender(view, proj);
		}
	}
}

void ObjectManager::Render(DXMatrix4X4 view, DXMatrix4X4 proj)
{
	for (Model model : m_ModelList)
	{
		if (model.begin()->second->GetEnabled() == false) continue;
		
		for (pair<string, GameObject*> obj : model)
		{
			obj.second->Render(view, proj);
		}
	}
}

void ObjectManager::AddObject(GameObject* obj)
{
	Model nowModel;
	nowModel.insert(pair<string, GameObject*>(obj->GetName(), obj));

	m_ModelList.push_back(nowModel);
}

void ObjectManager::AddModel(vector<GameObject*>& model)
{
	Model nowModel;

	for (unsigned int i = 0; i < model.size(); i++)
	{
		nowModel.insert(pair<string, GameObject*>(model[i]->GetName(), model[i]));
	}

	m_ModelList.push_back(nowModel);
}

void ObjectManager::AddTopObject(GameObject* obj)
{
	m_TopObjectList.insert(pair<string, GameObject*>(obj->GetName(), obj));
}

void ObjectManager::ObjectUpdate(float dTime)
{
	if (D3DEngine::GetInstance()->GetInput()->IsKeyUP(DIK_TAB))
	{
		for (Model model : m_ModelList)
		{
			for (pair<string, GameObject*> obj : model)
			{
				obj.second->SetPick(false);
			}
		}

		unordered_map<string, GameObject*>::iterator itor;
		if (m_PickObject == nullptr)
		{
			itor = m_TopObjectList.begin();
		}
		else
		{
			itor = m_TopObjectList.find(Model_Key);
			itor++;
		}


		if (itor == m_TopObjectList.end())
		{
			itor = m_TopObjectList.begin();
		}

		Model_Key = itor->first;

		ParentPick(true);

		// Target 설정..
		m_PickObject = m_TopObjectList[Model_Key];
		Camera::g_MainCamera->SetCameraTarget(m_PickObject->GetTransform());
	}

	if (m_PickObject == nullptr) return;

	// 선택중인 오브젝트 움직이기..
	if (D3DEngine::GetInstance()->GetInput()->IsKeyDownKeep(DIK_A))
	{
		m_PickObject->GetTransform()->MoveLocal(DXVector3(10.0f * dTime, 0, 0));
	}
	if (D3DEngine::GetInstance()->GetInput()->IsKeyDownKeep(DIK_D))
	{
		m_PickObject->GetTransform()->MoveLocal(DXVector3(-10.0f * dTime, 0, 0));
	}
	if (D3DEngine::GetInstance()->GetInput()->IsKeyDownKeep(DIK_W))
	{
		m_PickObject->GetTransform()->MoveLocal(DXVector3(0, 0, -10.0f * dTime));
	}
	if (D3DEngine::GetInstance()->GetInput()->IsKeyDownKeep(DIK_S))
	{
		m_PickObject->GetTransform()->MoveLocal(DXVector3(0, 0, 10.0f * dTime));
	}
}

void ObjectManager::SetHierarchy(vector<GameObject*>& model)
{
	ParserData::Mesh* mesh1 = nullptr;
	ParserData::Mesh* mesh2 = nullptr;

	// 초기 데이터 로딩 하이어라키 설정..
	for (GameObject* obj1 : model)
	{
		switch (obj1->GetMeshType())
		{
		case eMeshType::NormalMesh:
		case eMeshType::Helper:
		case eMeshType::Bone:
			mesh1 = obj1->GetComponent<MeshRenderer>()->GetMesh();
			break;
		case eMeshType::SkinMesh:
			mesh1 = obj1->GetComponent<SkinMeshRenderer>()->GetMesh();
			break;
		default:
			continue;
		}

		for (GameObject* obj2 : model)
		{
			if (obj1 == obj2) continue;

			switch (obj2->GetMeshType())
			{
			case eMeshType::NormalMesh:
			case eMeshType::Helper:
			case eMeshType::Bone:
			{
				mesh2 = obj2->GetComponent<MeshRenderer>()->GetMesh();
			}
			break;
			case eMeshType::SkinMesh:
			{
				SetBone(model, obj2);
				mesh2 = obj2->GetComponent<SkinMeshRenderer>()->GetMesh();
			}
			break;
			default:
				continue;
			}

			if (mesh1->m_NodeName == mesh2->m_ParentName)
			{
				obj1->AddChild(obj2->GetTransform());
				obj2->SetParent(obj1->GetTransform());

				// 최상위 Bone은 Parsing 할때 체크가 안되니
				// Hierarchy 연결과 동시에 Bone으로 바꿔주자..
				if (mesh1->m_IsBone || mesh2->m_IsBone)
				{
					obj1->SetMeshType(eMeshType::Bone);
					obj2->SetMeshType(eMeshType::Bone);

					mesh1->m_IsBone = true;
					mesh2->m_IsBone = true;
				}
			}
		}
	}

	// 초기 하이어라키 설정..
	(*model.begin())->GetTransform()->m_UpdateLocal = true;
}

void ObjectManager::SetBone(vector<GameObject*>& model, GameObject* obj)
{
	if (obj->GetMeshType() == eMeshType::SkinMesh)
	{
		for (GameObject* obj3 : model)
		{
			for (ParserData::Mesh* bone : obj->GetComponent<SkinMeshRenderer>()->GetMesh()->m_BoneMeshList)
			{
				if (obj3->GetName() == bone->m_NodeName)
				{
					obj->GetComponent<SkinMeshRenderer>()->SetBone(obj3);
					break;
				}
			}
		}
	}
}

void ObjectManager::Picking(int x, int y, int width, int height)
{
	XMMATRIX view = Camera::g_MainCamera->View();
	XMMATRIX proj = Camera::g_MainCamera->Proj();

	bool is_Pick = false;
	GameObject* selectObject = nullptr;

	int m_Index = 0;
	int pickIndex = 0;
	for (Model model : m_ModelList)
	{
		for (pair<string, GameObject*> obj : model)
		{
			obj.second->SetPick(false);

			if (obj.second->Picking(view, proj, width, height, x, y))
			{
				if (selectObject == nullptr)
				{
					Model_Key = obj.first;
					selectObject = obj.second;
					obj.second->SetPick(true);
					is_Pick = true;
					pickIndex = m_Index;
				}
				else if (selectObject != obj.second)
				{
					// 이전에 선택된 오브젝트와 현재 선택된 오브젝트와 카메라의 거리를 비교한다..
					float dis_before = KH_MATH::GetDistance(selectObject->GetTransform()->GetPosition(), Camera::g_MainCamera->GetPosition());
					float dis_now = KH_MATH::GetDistance(obj.second->GetTransform()->GetPosition(), Camera::g_MainCamera->GetPosition());

					// 가까운 오브젝트를 선택..
					if (dis_before > dis_now)
					{
						Model_Key = obj.first;

						selectObject->SetPick(false);
						selectObject = obj.second;

						obj.second->SetPick(true);
						is_Pick = true;
						pickIndex = m_Index;
					}
				}
			}
		}
		m_Index++;
	}

	if (is_Pick == false)
	{
		m_PickObject = nullptr;
	}
	else
	{
		// Target 설정..
		m_PickObject = m_ModelList[pickIndex][Model_Key];
		Camera::g_MainCamera->SetCameraTarget(m_ModelList[pickIndex][Model_Key]->GetTransform());
	}
}

void ObjectManager::ParentPick(bool pick)
{
	m_TopObjectList[Model_Key]->SetPick(pick);

	for (Transform* trans : m_TopObjectList[Model_Key]->GetAllChild())
	{
		trans->GetGameObject()->SetPick(pick);
		ChildPick(trans, pick);
	}
}

void ObjectManager::ChildPick(Transform* parent, bool pick)
{
	for (Transform* trans : parent->GetAllChild())
	{
		trans->GetGameObject()->SetPick(pick);
		ChildPick(trans, pick);
	}
}

void ObjectManager::Release()
{
	m_PickObject = nullptr;

	m_TopObjectList.clear();

	for (Model model : m_ModelList)
	{
		for (auto& obj : model)
		{
			RELEASE_COM(obj.second);
		}

		model.clear();
	}

	m_ModelList.clear();
}

GameObject* ObjectManager::FindObject(string objName)
{
	for (Model model : m_ModelList)
	{
		auto obj = model.find(objName);

		if (obj != model.end())
		{
			return obj->second;
		}
	}

	return nullptr;
}

Model ObjectManager::FindModel(string objName)
{
	for (Model model : m_ModelList)
	{
		auto obj = model.find(objName);

		if (obj != model.end())
		{
			return model;
		}
	}

	return Model();
}

GameObject* ObjectManager::GetNowObject()
{
	if (m_PickObject == nullptr)
		return nullptr;

	return m_TopObjectList[Model_Key];
}

//vector<GameObject*> ObjectManager::GetLayerAllObjects(int layerKey)
//{
//	vector<GameObject*> result;
//
//	for (pair<Layer*, GameObject*> layer : m_AllObjectList)
//	{
//		if (layer.first->m_LayerKey == layerKey)
//		{
//			result.push_back(layer.second);
//		}
//	}
//
//	return result;
//}
//
//vector<GameObject*> ObjectManager::GetLayerAllObjects(Layer* layerData)
//{
//	vector<GameObject*> result;
//
//	for (pair<Layer*, GameObject*> layer : m_AllObjectList)
//	{
//		if (layer.first == layerData)
//		{
//			result.push_back(layer.second);
//		}
//	}
//
//	return result;
//}
//
