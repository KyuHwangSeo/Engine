#include "D3DDefine.h"
#include "D3DEngine.h"
#include "GameTimer.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Gizmos.h"
#include "Camera.h"
#include "IManager.h"
#include "CameraManager.h"

CameraManager::CameraManager()
	:Camera_Index(0)
{
	m_Input = D3DEngine::GetInstance()->GetInput();
	m_NowMousePos = m_Input->GetMousePos();
}

CameraManager::~CameraManager()
{
	for (GameObject* cam : m_CameraList)
	{
		SAFE_RELEASE(cam);
	}
}

void CameraManager::Start()
{
	for (GameObject* cam : m_CameraList)
	{
		cam->Start();
	}
}

void CameraManager::Update(float dTime)
{
	for (GameObject* cam : m_CameraList)
	{
		CameraUpdate(cam, dTime);

		cam->Update(dTime);
	}
}

void CameraManager::Render(DXMatrix4X4 view, DXMatrix4X4 proj)
{
	for (GameObject* cam : m_CameraList)
	{
		cam->Render(view, proj);
	}
}

void CameraManager::AddObject(GameObject* obj)
{
	m_CameraList.push_back(obj);
}

void CameraManager::CameraUpdate(GameObject* cam, float dTime)
{
	CameraMove();

	// 카메라 시점 변경..
	/// 일단은 상수값으로 위치를 조정하는데 좀더 좋은 방법이 있을거같다..
	Camera* camera = cam->GetComponent<Camera>();

	switch (camera->m_CameraType)
	{
	case eCameraType::NormalCam:
	{
		if (m_Input->IsKeyDownKeep(VK_RSHIFT))
			camera->m_Speed += dTime;
		else
			camera->m_Speed = 1;

		//if (m_Input->IsKeyDownKeep(VK_R))
		//{
		//	camera->RotateZ(10.0f * dTime);
		//}

		if (m_Input->IsKeyDownKeep(VK_UP))
		{
			camera->Walk(10.0f * dTime * Camera::g_MainCamera->m_Speed);
		}

		if (m_Input->IsKeyDownKeep(VK_DOWN))
		{
			camera->Walk(-10.0f * dTime * Camera::g_MainCamera->m_Speed);
		}

		if (m_Input->IsKeyDownKeep(VK_LEFT))
		{
			camera->Strafe(-10.0f * dTime * Camera::g_MainCamera->m_Speed);
		}

		if (m_Input->IsKeyDownKeep(VK_RIGHT))
		{
			camera->Strafe(10.0f * dTime * Camera::g_MainCamera->m_Speed);
		}

		if (m_Input->IsKeyDownKeep(VK_Q))
		{
			camera->WorldUpDown(-10.0f * dTime * Camera::g_MainCamera->m_Speed);
		}

		if (m_Input->IsKeyDownKeep(VK_E))
		{
			camera->WorldUpDown(10.0f * dTime * Camera::g_MainCamera->m_Speed);
		}
		break;
	}
	case eCameraType::FollowCam_3:
	{
		//XMFLOAT3 offsetPos = XMFLOAT3(0.0f, 3.0f, 0.0f);
		XMFLOAT3 offsetPos2 = XMFLOAT3(0.0f, 15.0f, -30.0f);

		//if (D3DEngine::GetInstance()->GetInput()->IsKeyDownKeep(DIK_LALT))
		//{
		//	camera->LookAt_3_Person_FreeView(offsetPos, 11.25f);
		//}
		//else
		{
			camera->LookAt_3_Person_MoveX(offsetPos2);
		}
		break;
	}
	case eCameraType::FollowCam_1:
	{
		XMFLOAT3 offsetPos = XMFLOAT3(0.0f, 1.0f, 0.0f);

		camera->LookAt_1_Person(offsetPos, -1.0f);
		break;
	}
	}

	if (m_Input->IsKeyUP(VK_C))
	{
		//if (Camera::m_Target == nullptr) return;
	
		Camera_Index++;
		if (Camera_Index >= (int)m_CameraList.size())
		{
			Camera_Index = 0;
		}
	
		// MainCamera 교체..
		Camera::g_MainCamera = m_CameraList[Camera_Index]->GetComponent<Camera>();
		Camera::g_MainCamera->m_Angle = 0;
	}
}

void CameraManager::CameraSetLens(int screenWidth, int screenHeight)
{
	for (GameObject* cam : m_CameraList)
	{
		cam->GetComponent<Camera>()->SetLens(0.25f * KH_MATH::Pi, (float)screenWidth, (float)screenHeight, 1.0f, 1000.0f);
	}
}

void CameraManager::CameraMove()
{
	if (Camera::g_MainCamera == nullptr) return;

	float dx = 0;
	float dy = 0;

	switch (Camera::g_MainCamera->m_CameraType)
	{
	case eCameraType::NormalCam:
	{
		if (m_Input->IsKeyDownKeep(VK_RBUTTON) != 0)
		{
			// Make each pixel correspond to a quarter of a degree.
			dx = XMConvertToRadians(0.25f * static_cast<float>(m_NowMousePos->x - m_LastMousePos.x));
			dy = XMConvertToRadians(0.25f * static_cast<float>(m_NowMousePos->y - m_LastMousePos.y));

			Camera::g_MainCamera->Pitch(dy);
			Camera::g_MainCamera->RotateY(dx);
		}
	}
	break;

	case  eCameraType::FollowCam_3:
	{
		if (m_Input->IsKeyDownKeep(DIK_LALT))
		{
			// Make each pixel correspond to a quarter of a degree.
			dx = XMConvertToRadians(0.25f * static_cast<float>(m_NowMousePos->x - m_LastMousePos.x));
			dy = XMConvertToRadians(0.25f * static_cast<float>(m_NowMousePos->y - m_LastMousePos.y));

			Camera::g_MainCamera->RotateX(dy);
			Camera::g_MainCamera->RotateY(dx);
		}
	}
	break;
	case  eCameraType::FollowCam_1:
		break;
	}

	m_LastMousePos = *m_NowMousePos;
}

void CameraManager::Release()
{
	for (GameObject* cam : m_CameraList)
	{
		RELEASE_COM(cam);
	}

	m_CameraList.clear();
}

GameObject* CameraManager::FindObject(string objName)
{
	for (GameObject* obj : m_CameraList)
	{
		if (obj->GetName() == objName)
		{
			return obj;
		}
	}
	return nullptr;
}
