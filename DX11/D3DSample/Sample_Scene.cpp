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
	GameObject* player = KH_UTILITY::CreateObject("CHARACTER_idle", eModelType::Model, DXVector3(0.01f, 0.01f, 0.01f), true, eModelCollider::BoneList);
	KH_UTILITY::CreateAnimation("CHARACTER_idle", "Idle", player, true);
	player->SetName("Player");
	player->SetTag(eTag::Player);
	player->GetTransform()->MoveWorld(DXVector3(0.0f, 0.0f, 0.0f));
	player->GetTransform()->RotateLocal(DXVector3(0.0f, 180.0f, 0.0f));

	GameObject* ground = KH_UTILITY::CreateObject("Map_Origin", eModelType::TerrainMesh, DXVector3(1.0f, 1.0f, 1.0f));
	ground->GetTransform()->RotateLocal(DXVector3(90.0f, 0.0f, 0.0f));

	//Player* playerCom = new Player;
	//player->AddComponent(playerCom);
}

void Sample_Scene::SceneUpdate(float dTime)
{

}
