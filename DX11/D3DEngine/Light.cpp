#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Gizmos.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"

// Light 전용 전역 변수 초기화..
Light* Light::g_DirLight = nullptr;
DXVector3	Light::g_CenterPos = DXVector3(0.0, 0.0, 0.0);
float		Light::g_ShadowRadius = sqrtf(10.0f * 10.0f + 15.0f * 15.0f);

Light::Light(eLightType lightType)
	:m_LightType(lightType)
{
	SetName<Light>();
}

Light::~Light()
{

}

void Light::Start()
{
	// Light Gizmo 생성..
#ifdef _DEBUG
	m_Gizmo = new Gizmos(eGizmoType::Light, DXVector3(10.0f, 10.0f, 10.0f));
	m_Gizmo->SetGameObject(m_GameObject);
#endif

	/// Before Light Data
	//m_DirLights[0].Ambient = DXVector4(0.2f, 0.2f, 0.2f, 1.0f);
	//m_DirLights[0].Diffuse = DXVector4(0.5f, 0.5f, 0.5f, 1.0f);
	//m_DirLights[0].Specular = DXVector4(0.5f, 0.5f, 0.5f, 1.0f);
	//m_DirLights[0].Direction = DXVector3(0.57735f, -0.57735f, 0.57735f);
	//
	//m_DirLights[1].Ambient = DXVector4(0.0f, 0.0f, 0.0f, 1.0f);
	//m_DirLights[1].Diffuse = DXVector4(0.20f, 0.20f, 0.20f, 1.0f);
	//m_DirLights[1].Specular = DXVector4(0.25f, 0.25f, 0.25f, 1.0f);
	//m_DirLights[1].Direction = DXVector3(-0.57735f, -0.57735f, 0.57735f);
	//
	//m_DirLights[2].Ambient = DXVector4(0.0f, 0.0f, 0.0f, 1.0f);
	//m_DirLights[2].Diffuse = DXVector4(0.2f, 0.2f, 0.2f, 1.0f);
	//m_DirLights[2].Specular = DXVector4(0.0f, 0.0f, 0.0f, 1.0f);
	//m_DirLights[2].Direction = DXVector3(0.0f, -0.707f, -0.707f);
	//
	// DirLight Initial Data
	//m_DirLights[0].Ambient = DXVector4(1.0f, 1.0f, 1.0f, 1.0f);
	//m_DirLights[0].Diffuse = DXVector4(0.5f, 0.5f, 0.4f, 1.0f);
	//m_DirLights[0].Specular = DXVector4(0.8f, 0.8f, 0.7f, 1.0f);
	//m_DirLights[0].Direction = DXVector3(-0.57735f, -0.57735f, 0.57735f);
	//
	//m_DirLights[1].Ambient = DXVector4(0.0f, 0.0f, 0.0f, 1.0f);
	//m_DirLights[1].Diffuse = DXVector4(0.4f, 0.4f, 0.4f, 1.0f);
	//m_DirLights[1].Specular = DXVector4(0.2f, 0.2f, 0.2f, 1.0f);
	//m_DirLights[1].Direction = DXVector3(0.707f, -0.707f, 0.0f);
	//
	//m_DirLights[2].Ambient = DXVector4(0.0f, 0.0f, 0.0f, 1.0f);
	//m_DirLights[2].Diffuse = DXVector4(0.2f, 0.2f, 0.2f, 1.0f);
	//m_DirLights[2].Specular = DXVector4(0.2f, 0.2f, 0.2f, 1.0f);
	//m_DirLights[2].Direction = DXVector3(0.0f, 0.0f, -1.0f);

	m_DirLights[0].Ambient = DXVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLights[0].Diffuse = DXVector4(0.5f, 0.5f, 0.5f, 1.0f);
	m_DirLights[0].Specular = DXVector4(0.8f, 0.8f, 0.8f, 1.0f);
	m_DirLights[0].Direction = DXVector3(0.3f, -0.707f, -0.3f);

	m_DirLights[1].Ambient = DXVector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_DirLights[1].Diffuse = DXVector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_DirLights[1].Specular = DXVector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_DirLights[1].Direction = DXVector3(-0.5f, -0.707f, -0.5f);

	m_DirLights[2].Ambient = DXVector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_DirLights[2].Diffuse = DXVector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_DirLights[2].Specular = DXVector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_DirLights[2].Direction = DXVector3(-0.7f, 0.0f, 0.7f);

	// PointLight Initial Data
	m_PointLight.Ambient = DXVector4(0.3f, 0.3f, 0.3f, 1.0f);
	//m_PointLight.Diffuse = DXVector4(0.7f, 0.7f, 0.7f, 1.0f);
	m_PointLight.Specular = DXVector4(0.7f, 0.7f, 0.7f, 1.0f);
	m_PointLight.Position = m_GameObject->GetTransform()->GetPosition();
	m_PointLight.Att = DXVector3(0.0f, 0.2f, 0.0f);
	m_PointLight.Range = 50.0f;
	m_PointLight.Pad = 1.0f;

	// SpotLight Initial Data
	m_SpotLight.Ambient = DXVector4(0.5f, 0.5f, 0.6f, 1.0f);
	//m_SpotLight.Diffuse = DXVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpotLight.Specular = DXVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpotLight.Direction = DXVector3(0.0f, -1.0f, 0.0f);
	m_SpotLight.Att = DXVector3(0.9f, 0.9f, 0.0f);
	m_SpotLight.Range = 10000.0f;
	m_SpotLight.Spot = 96.0f;
	m_SpotLight.Position = m_GameObject->GetTransform()->GetPosition();

	// Shadow 전용 함수..
	Light::g_DirLight->SetLightViewProj();
}

void Light::Update(float dTime)
{
	switch (m_LightType)
	{
	case eLightType::Direction:
		break;
	case eLightType::Point:
		SetPosition<PointLight>(m_PointLight);
		break;
	case eLightType::Spot:
		SetPosition<SpotLight>(m_SpotLight);
		break;
	default:
		break;
	}

	if (m_Target != nullptr)
	{
		switch (m_LightType)
		{
		case eLightType::Point:
		{
			m_PointLight.Position.x = m_Target->GetPosition().x;
			m_PointLight.Position.z = m_Target->GetPosition().z;
		}
		break;
		case eLightType::Spot:
		{
			m_SpotLight.Position.x = m_Target->GetPosition().x;
			m_SpotLight.Position.z = m_Target->GetPosition().z;
		}
		break;
		default:
			break;
		}
	}
}

void Light::Render(DXMatrix4X4 view, DXMatrix4X4 proj)
{
#ifdef _DEBUG
	m_Gizmo->Render(view, proj);
#endif
}

void Light::Release()
{
	SAFE_DELETE(m_Gizmo);
}

ENGINE_DLL void Light::SetLightType(eLightType lightType)
{
	m_LightType = lightType;
}

ENGINE_DLL void Light::SetLightColor(DXVector4 lightColor)
{
	switch (m_LightType)
	{
	case eLightType::Point:
		m_PointLight.Diffuse = lightColor;
		break;
	case eLightType::Spot:
		m_SpotLight.Diffuse = lightColor;
		break;
	default:
		break;
	}
}

void Light::DirLightRotate(DXVector3 axis, float angle)
{
	for (int i = 0; i < 3; i++)
	{
		DXVector3 dirVec = XMVector3Transform(GetDirection(i), XMMatrixRotationAxis(axis, angle));
		m_DirLights[i].Direction = dirVec;
	}
}

void Light::SetPointLight(PointLight pointLight)
{
	m_PointLight = pointLight;
}

void Light::SetSpotLight(SpotLight spotLight)
{
	m_SpotLight = spotLight;
}

ENGINE_DLL void Light::SetTarget(GameObject* obj)
{
	m_Target = obj->GetTransform();
}

void Light::SetLightViewProj()
{
	/// Light Direction 값 변동시 같이 실행해주어야 함..
	// Light View, Proj 설정..
	DXVector3 lightPos = Light::g_DirLight->GetDirection() * -2.0f * g_ShadowRadius;

	// Ligh View
	m_LightView = XMMatrixLookAtLH(lightPos, g_CenterPos, DXVector4(0.0f, 1.0f, 0.0f, 0.0f));

	DXVector3 lightSpace = XMVector3TransformCoord(g_CenterPos, m_LightView);

	// 장면을 감싸는 광원 공간 직교 투영 상자..
	float l = lightSpace.x - g_ShadowRadius;
	float b = lightSpace.y - g_ShadowRadius;
	float n = lightSpace.z - g_ShadowRadius;
	float r = lightSpace.x + g_ShadowRadius;
	float t = lightSpace.y + g_ShadowRadius;
	float f = lightSpace.z + g_ShadowRadius;

	// Light Proj
	m_LightProj = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	// NDC 공간 [-1, +1]^2을 텍스처 공간 [0, 1]^2으로 변환..
	Material::SetShadowTransform(m_LightView * m_LightProj);
}

void Light::SetLightData(cbLights& lightData, int m_Index)
{
	switch (m_LightType)
	{
	case eLightType::Direction:
		lightData.gDirLights[0] = m_DirLights[0];
		lightData.gDirLights[1] = m_DirLights[1];
		lightData.gDirLights[2] = m_DirLights[2];
		break;
	case eLightType::Point:
		lightData.gPointLights[m_Index] = m_PointLight;
		break;
	case eLightType::Spot:
		lightData.gSpotLights[m_Index] = m_SpotLight;
		break;
	default:
		break;
	}
}

void Light::SetLightListData(cbLightList& lightListData, int m_Index)
{
	switch (m_LightType)
	{
	case eLightType::Direction:
		lightListData.gDirLights[0] = m_DirLights[0];
		lightListData.gDirLights[1] = m_DirLights[1];
		lightListData.gDirLights[2] = m_DirLights[2];
		break;
	case eLightType::Point:
		lightListData.gPointLights[m_Index] = m_PointLight;
		break;
	case eLightType::Spot:
		lightListData.gSpotLights[m_Index] = m_SpotLight;
		break;
	default:
		break;
	}
}

DXVector3 Light::GetDirection(int m_Index)
{
	switch (m_LightType)
	{
	case eLightType::Direction:
		return m_DirLights[m_Index].Direction;
		break;
	case eLightType::Spot:
		return m_SpotLight.Direction;
		break;
	default:
		break;
	}

	return DXVector3::Zero();
}