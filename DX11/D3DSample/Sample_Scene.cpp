#include "D3DEngineDLL.h"
#include "Sample_Scene.h"

Sample_Scene::Sample_Scene()
{

}

Sample_Scene::~Sample_Scene()
{

}

void Sample_Scene::Create()
{
	GameObject* player = KH_UTILITY::CreateObject("CHARACTER_idle", eModelType::FBX, DXVector3(0.01f, 0.01f, 0.01f), true, eModelCollider::BoneList);
	KH_UTILITY::CreateAnimation("CHARACTER_idle", "Idle", player, true);
	player->SetName("Player");
	player->SetTag(eTag::Player);
	player->GetTransform()->MoveWorld(DXVector3(0.0f, 0.0f, 0.0f));
	player->GetTransform()->RotateLocal(DXVector3(0.0f, 180.0f, 0.0f));
}

void Sample_Scene::SceneUpdate(float dTime)
{

}
