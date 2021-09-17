#include "D3DDefine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "IManager.h"
#include "HelperManager.h"

HelperManager::HelperManager()
{
	for (GameObject* helper: m_HelperList)
	{
		SAFE_RELEASE(helper);
	}
}

HelperManager::~HelperManager()
{

}

void HelperManager::Start()
{
	for (GameObject* helper : m_HelperList)
	{
		helper->Start();
	}
}

void HelperManager::Update(float dTime)
{
	for (GameObject* helper : m_HelperList)
	{
		helper->Update(dTime);
	}
}

void HelperManager::Render(DXMatrix4X4 view, DXMatrix4X4 proj)
{
	for (GameObject* helper : m_HelperList)
	{
		helper->Render(view, proj);
	}
}

void HelperManager::AddObject(GameObject* obj)
{
	m_HelperList.push_back(obj);
}

void HelperManager::Release()
{
	for (GameObject* helper : m_HelperList)
	{
		RELEASE_COM(helper);
	}

	m_HelperList.clear();
}

GameObject* HelperManager::FindObject(string objName)
{
	for (GameObject* obj : m_HelperList)
	{
		if (obj->GetName() == objName)
		{
			return obj;
		}
	}
	return nullptr;
}
