#include "DLLDefine.h"
#include "SceneDefine.h"
#include "RendererDefine.h"
#include "Scene.h"

Scene::Scene()
{
	// Manager 생성 및 초기화..
	m_RsMG = ResourceManager::GetInstance();
	m_CamMG = new CameraManager();
	m_LightMG = new LightManager();
	m_HelpMG = new HelperManager();
	m_ObjMG = new ObjectManager();
	m_MatMG = new MaterialManager();
	m_ColMG = new CollisionManager();
	m_UiMG = new UIManager();

	m_ClientSize = D3DEngine::GetInstance()->GetClinetSize();
}

void Scene::Start()
{
	// Camera Start Update..
	m_CamMG->Start();

	// Light Start Update..
	m_LightMG->Start();

	// Helper Start Update..
	m_HelpMG->Start();

	// GameObject Start Update..
	m_ObjMG->Start();

	// UI Start Update..
	m_UiMG->Start();
}

void Scene::Update(float dTime)
{
	// Camera Update..
	m_CamMG->Update(dTime);

	// Light Update..
	m_LightMG->Update(dTime);

	// Helper Update..
	m_HelpMG->Update(dTime);

	// GameObject Update..
	m_ObjMG->Update(dTime);

	// UI Update..
	m_UiMG->Update(dTime);

	// Collision Update..
	m_ColMG->Update(dTime);
}

void Scene::FinalUpdate(float dTime)
{
	// GameObject Final Update..
	m_ObjMG->FinalUpdate(dTime);
}

void Scene::Render()
{
	// Shadow Render
	ShadowRender();

	// Deferred Render
	MainRender();

	// Alpha Render
	LateRender();
}

void Scene::SetCameraLens(int screenWidth, int screenHeight)
{
	m_CamMG->CameraSetLens(screenWidth, screenHeight);
}

void Scene::Picking(int x, int y, int screenWidth, int screenHeight)
{
	m_ObjMG->Picking(x, y, screenWidth, screenHeight);
}

void Scene::CreateSkyBox()
{
	// SkyBox 생성..
	m_SkyBox = new SkyBox();
}

void Scene::SetSkyBox(const char* mapName)
{
	// SkyBox 설정..
	m_SkyBox->SetCubeMap(m_RsMG->GetTexture(mapName));
}

GameObject* Scene::FindObject(const char* objName, eObjectType objType)
{
	switch (objType)
	{
	case eObjectType::Model:
		return m_ObjMG->FindObject(objName);
	case eObjectType::Camera:
		return m_CamMG->FindObject(objName);
	case eObjectType::Light:
		return m_LightMG->FindObject(objName);
	default:
		break;
	}

	return nullptr;
}

void Scene::Release()
{
	SAFE_DELETE(m_SkyBox);
	
	m_CamMG->Release();
	m_LightMG->Release();
	m_HelpMG->Release();
	m_ObjMG->Release();
	m_MatMG->Release();
	m_ColMG->Release();
	m_UiMG->Release();
}

void Scene::ShadowRender()
{
	// 전체 오브젝트 그리기..
	view = Light::g_DirLight->GetLightView();
	proj = Light::g_DirLight->GetLightProj();

	// Shadow Rendering..
	m_Renderer->ShadowRender();
	m_ObjMG->ShadowRender(view, proj);
}

void Scene::MainRender()
{
	// 전체 오브젝트 그리기..
	view = Camera::g_MainCamera->View();
	proj = Camera::g_MainCamera->Proj();

	// Main Rendering..
	m_Renderer->MainRender();

	// SkyBox Render..
	if (m_SkyBox != nullptr)
		m_SkyBox->Render(view, proj);

	// Gizmo Render..
	m_LightMG->Render(view, proj);

	m_CamMG->Render(view, proj);

	// Helper Render..
	m_HelpMG->Render(view, proj);

	// GameObject Render..
	m_ObjMG->Render(view, proj);

	// SSAO Rendering..
	m_Renderer->SSAORender();

	// Final Deferred Rendering..
	m_Renderer->DeferredRender();
}

void Scene::LateRender()
{
	view = Camera::g_MainCamera->View(); 
	ortho = Camera::g_MainCamera->Ortho();

	// UI Rendering..
	m_Renderer->AlphaRender(); 
	m_UiMG->Render(view, ortho);
}
