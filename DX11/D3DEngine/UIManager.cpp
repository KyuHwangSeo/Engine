#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "IManager.h"
#include "Shader.h"
#include "UIManager.h"

UIManager::UIManager()
{

}

UIManager::~UIManager()
{

}

void UIManager::Start()
{
	for (pair<string, GameObject*> ui : m_UIList)
	{
		ui.second->Start();
	}
}

void UIManager::Update(float dTime)
{
	for (pair<string, GameObject*> ui : m_UIList)
	{
		ui.second->Update(dTime);
	}
}

void UIManager::Render(DXMatrix4X4 view, DXMatrix4X4 proj)
{
	for (pair<string, GameObject*> ui : m_UIList)
	{
		ui.second->Render(view, proj);
	}
}

void UIManager::AddObject(GameObject* obj)
{
	m_UIList.insert(pair<string, GameObject*>(obj->GetName(), obj));
}

void UIManager::Release()
{
	for (auto& ui : m_UIList)
	{
		RELEASE_COM(ui.second);
	}

	m_UIList.clear();
}

GameObject* UIManager::FindObject(string objName)
{
	auto ui = m_UIList.find(objName);

	if (ui != m_UIList.end())
		return ui->second;

	return nullptr;
}
