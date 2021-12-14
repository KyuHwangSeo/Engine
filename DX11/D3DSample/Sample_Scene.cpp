#include "D3DEngineDLL.h"
#include "Sample_Scene.h"
#include "Player.h"

Sample_Scene::Sample_Scene()
{

}

Sample_Scene::~Sample_Scene()
{

}

void Sample_Scene::Create()
{
	//GameObject* player = KH_UTILITY::CreateObject("CHARACTER_idle", eModelType::Model, DXVector3(0.5f, 0.5f, 0.5f), true, eModelCollider::BoneList);
	//KH_UTILITY::CreateAnimation("CHARACTER_idle", "Idle", player, true);
	//player->SetName("Player");
	//player->SetTag(eTag::Player);
	//player->GetTransform()->MoveWorld(DXVector3(10.0f, 0.0f, 0.0f));
	//player->GetTransform()->RotateLocal(DXVector3(0.0f, 0.0f, 0.0f));

	GameObject* ground = KH_UTILITY::CreateObject("Terrain", eModelType::TerrainMesh, DXVector3(1.0f, 1.0f, 1.0f));
	ground->GetTransform()->RotateLocal(DXVector3(90.0f, 0.0f, 0.0f));

	//GameObject* field = KH_UTILITY::CreateObject("Field", eModelType::Model, DXVector3(1.0f, 1.0f, 1.0f));

	GameObject* dome = KH_UTILITY::CreateObject("Dome_v03", eModelType::Model, DXVector3(1.0f, 1.0f, 1.0f));
	//dome->GetTransform()->MoveWorld(DXVector3(-10.0f, 0.0f, 0.0f));
	dome->GetTransform()->RotateLocal(DXVector3(90.0f, 0.0f, 0.0f));
	//m_Player = KH_UTILITY::CreateObject("join_droid", eModelType::Model, DXVector3(0.01f, 0.01f, 0.01f));
	//m_Mat = m_Player->GetComponent<MeshRenderer>()->GetMaterial();

	//diffuese = m_Mat->GetDiffuseMap();
	//gray = KH_UTILITY::GetTexture("Gray");

	//GameObject* light2 = KH_UTILITY::CreateObject("Spot Light", eModelType::PointLight);
	//light2->GetTransform()->SetScale(DXVector3(10.0f, 10.0f, 10.0f));
	//light2->GetTransform()->MoveWorld(DXVector3(0.0f, 25.0f, 0.0f));
	//m_Transform = light2->GetTransform();

	//GameObject* light3 = KH_UTILITY::CreateObject("Spot Light1", eModelType::PointLight);
	//light3->GetTransform()->SetScale(DXVector3(10.0f, 10.0f, 10.0f));
	//light3->GetTransform()->MoveLocal(DXVector3(20.0f, 10.0f, 0.0f));
	//
	//GameObject* light4 = KH_UTILITY::CreateObject("Spot Light2", eModelType::PointLight);
	//light4->GetTransform()->SetScale(DXVector3(10.0f, 10.0f, 10.0f));
	//light4->GetTransform()->MoveLocal(DXVector3(30.0f, 10.0f, 0.0f));

	//Player* playerCom = new Player;
	//player->AddComponent(playerCom);
}

void Sample_Scene::SceneUpdate(float dTime)
{
	if (KH_KEYINPUT::IsKeyUP(VK_F1))
	{
		if (IsTex)
		{
			m_Mat->SetDiffuseMap(diffuese);
			IsTex = false;
		}
		else
		{
			m_Mat->SetDiffuseMap(gray);
			IsTex = true;
		}
	}
	//if (KH_KEYINPUT::IsKeyDownKeep(VK_W))
	//{
	//	m_Transform->MoveLocal(DXVector3(0.0f, 0.0f, 2.0f * dTime));
	//}
	//if (KH_KEYINPUT::IsKeyDownKeep(VK_S))
	//{
	//	m_Transform->MoveLocal(DXVector3(0.0f, 0.0f, -2.0f * dTime));
	//}
	//
	//if (KH_KEYINPUT::IsKeyDownKeep(VK_A))
	//{
	//	m_Transform->MoveLocal(DXVector3(-2.0f * dTime, 0.0f, 0.0f));
	//}
	//
	//if (KH_KEYINPUT::IsKeyDownKeep(VK_D))
	//{
	//	m_Transform->MoveLocal(DXVector3(2.0f * dTime, 0.0f, 0.0f));
	//}
	//
	//if (KH_KEYINPUT::IsKeyDownKeep(VK_Z))
	//{
	//	m_Transform->MoveLocal(DXVector3(0.0f, 2.0f * dTime, 0.0f));
	//}
	//if (KH_KEYINPUT::IsKeyDownKeep(VK_X))
	//{
	//	m_Transform->MoveLocal(DXVector3(0.0f, -2.0f * dTime, 0.0f));
	//}
}
