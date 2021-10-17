#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "IManager.h"
#include "Light.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "LightManager.h"

LightManager::LightManager()
{
	m_LightShader = D3DEngine::GetInstance()->GetResourceManager()->GetShader("LightShader");

	m_ObjectData.gWorldViewProj = XMMatrixIdentity();

	m_TextureSpace = DXMatrix4X4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
}

LightManager::~LightManager()
{
	for (GameObject* light : m_LightObjectList)
	{
		SAFE_RELEASE(light);
	}
}

void LightManager::Start()
{
	for (GameObject* light : m_LightObjectList)
	{
		light->Start();
	}
}

void LightManager::Update(float dTime)
{
	for (GameObject* light : m_LightObjectList)
	{
		light->Update(dTime);
	}

	m_CameraData.gEyePosW = Camera::g_MainCamera->GetPosition();
	
	//SetLightData_Forward();

	/// Forward Shader Update
	//for (pair<string, Shader*> shader : D3DEngine::GetInstance()->GetResourceManager()->GetShaderList())
	//{
	//	shader.second->SetVertexConstantBuffer(m_CameraData);
	//	shader.second->SetVertexConstantBuffer(m_LightData);
	//}

	/// Deferred Shader Update
	SetLightData_Deferred();

	m_LightShader->SetVertexConstantBuffer(m_ObjectData);

	m_LightShader->SetPixelConstantBuffer(m_LightListData);
	m_LightShader->SetPixelConstantBuffer(m_CameraData);

	m_TexViewProj.gViewProjTex = Camera::g_MainCamera->View() * Camera::g_MainCamera->Proj() * m_TextureSpace;
	m_LightShader->SetPixelConstantBuffer(m_TexViewProj);
}

void LightManager::Render(DXMatrix4X4 view, DXMatrix4X4 proj)
{
	for (GameObject* light : m_LightObjectList)
	{
		light->Render(view, proj);
	}
}

void LightManager::AddObject(GameObject* obj)
{
	m_LightObjectList.push_back(obj);

	Light* nowLight = obj->GetComponent<Light>();

	// Light 종류에 따른 해당 Light Component List에 삽입..
	switch (nowLight->GetLightType())
	{
	case eLightType::Point:
		m_PointLightList.push_back(nowLight);
		break;
	case eLightType::Spot:
		m_SpotLightList.push_back(nowLight);
		break;
	default:
		break;
	}
}

void LightManager::SetLightData_Forward()
{
	Light::g_DirLight->SetLightData(m_LightData);

	for (unsigned int m_Index = 0; m_Index < m_PointLightList.size(); m_Index++)
	{
		m_PointLightList[m_Index]->SetLightData(m_LightData, m_Index);
	}

	for (unsigned int m_Index = 0; m_Index < m_SpotLightList.size(); m_Index++)
	{
		m_SpotLightList[m_Index]->SetLightData(m_LightData, m_Index);
	}
}

void LightManager::SetLightData_Deferred()
{
	Light::g_DirLight->SetLightListData(m_LightListData);

	// NoLight
	if (m_IsLight == false)
	{
		m_LightListData.gPointLightCount = 0;
		m_LightListData.gSpotLightCount = 0;
		return;
	}

	for (unsigned int m_Index = 0; m_Index < m_PointLightList.size(); m_Index++)
	{
		m_PointLightList[m_Index]->SetLightListData(m_LightListData, m_Index);
	}

	for (unsigned int m_Index = 0; m_Index < m_SpotLightList.size(); m_Index++)
	{
		m_SpotLightList[m_Index]->SetLightListData(m_LightListData, m_Index);
	}

	m_LightListData.gPointLightCount = (UINT)m_PointLightList.size();
	m_LightListData.gSpotLightCount = (UINT)m_SpotLightList.size();
}

void LightManager::Release()
{
	m_PointLightList.clear();
	m_SpotLightList.clear();

	for (GameObject* light : m_LightObjectList)
	{
		RELEASE_COM(light);
	}

	m_LightObjectList.clear();
}

GameObject* LightManager::FindObject(string objName)
{
	for (GameObject* obj : m_LightObjectList)
	{
		if (obj->GetName() == objName)
		{
			return obj;
		}
	}
	return nullptr;
}
