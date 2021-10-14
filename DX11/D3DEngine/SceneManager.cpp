#include "SceneDefine.h"
#include "RendererDefine.h"
#include "Scene.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::Initialize()
{
	// Manager 생성 및 초기화..
	m_CamMG = new CameraManager();
	m_LightMG = new LightManager();
	m_HelpMG = new HelperManager();
	m_ObjMG = new ObjectManager();
	m_MatMG = new MaterialManager();
	m_ColMG = new CollisionManager();
	m_UiMG = new UIManager();

	// Factory 생성 및 Manager 설정..
	m_Factory = new Factory(m_CamMG, m_LightMG, m_HelpMG, m_ObjMG, m_MatMG, m_ColMG, m_UiMG);
}

void SceneManager::AddScene(std::string name, Scene* scene)
{
	m_SceneList.insert(pair<std::string, Scene*>(name, scene));

	// 최초 Scene 추가 시 실행..
	if (m_NowScene == nullptr)
	{
		// 현재 Scene 설정..
		m_NowScene = scene;

		// Scene 초기 설정..
		SetScene();
	}
}

Scene* SceneManager::SelectScene(std::string name)
{
	std::unordered_map<std::string, Scene*>::iterator scene = m_SceneList.find(name);

	if (scene == m_SceneList.end()) return nullptr;

	if (scene->second == m_NowScene) return m_NowScene;

	// 이전 Scene 초기화..
	m_NowScene->Release();

	// Scene 전환..
	m_NowScene = scene->second;

	// Scene 초기 설정..
	SetScene();

	return m_NowScene;
}

void SceneManager::RemoveScene(std::string name)
{
	std::unordered_map<std::string, Scene*>::iterator scene = m_SceneList.find(name);

	// Scene Data Release..
	SAFE_RELEASE(scene->second);

	// Scene 제거..
	m_SceneList.erase(scene);
}

void SceneManager::SetScene()
{
	// Manager 설정..
	m_NowScene->m_CamMG = m_CamMG;
	m_NowScene->m_LightMG = m_LightMG;
	m_NowScene->m_HelpMG = m_HelpMG;
	m_NowScene->m_ObjMG = m_ObjMG;
	m_NowScene->m_MatMG = m_MatMG;
	m_NowScene->m_ColMG = m_ColMG;
	m_NowScene->m_UiMG = m_UiMG;
}

void SceneManager::CreateHelper()
{
	// Camera 생성 및 초기화..
	GameObject* cam1 = m_Factory->CreateObject("Camera1", eModelType::Camera);
	cam1->GetTransform()->SetScale(DXVector3(0.5f, 0.5f, 0.5f));
	cam1->GetComponent<Camera>()->SetType(eCameraType::NormalCam);

	GameObject* cam2 = m_Factory->CreateObject("Camera2", eModelType::Camera);
	cam2->GetTransform()->SetScale(DXVector3(0.5f, 0.5f, 0.5f));
	cam2->GetComponent<Camera>()->SetType(eCameraType::FollowCam_3);

	// Main Camera 설정..
	Camera::g_MainCamera = cam2->GetComponent<Camera>();

	// HerlperObject 생성 및 초기화..
#ifdef _DEBUG
	m_Factory->CreateObject("Grid", eModelType::Grid);
	m_Factory->CreateObject("Axis", eModelType::Axis);
#endif

	// 기본 Directional Light 생성..
	GameObject* light1 = m_Factory->CreateObject("Direction Light", eModelType::DirLight);
	light1->GetTransform()->SetScale(DXVector3(10.0f, 10.0f, 10.0f));
	light1->GetTransform()->MoveLocal(DXVector3(0.0f, 15.0f, 0.0f));

	// Direction Light 설정..
	Light::g_DirLight = light1->GetComponent<Light>();
}
