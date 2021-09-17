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

void SceneManager::AddScene(std::string name, Scene* scene)
{
	m_SceneList.insert(pair<std::string, Scene*>(name, scene));

	if (m_NowScene == nullptr)
		m_NowScene = scene;
}

Scene* SceneManager::SelectScene(std::string name)
{
	std::unordered_map<std::string, Scene*>::iterator scene = m_SceneList.find(name);

	if (scene == m_SceneList.end()) return nullptr;

	// Scene 전환..
	m_NowScene = scene->second;

	return m_NowScene;
}

void SceneManager::RemoveScene(std::string name)
{
	std::unordered_map<std::string, Scene*>::iterator scene = m_SceneList.find(name);

	// Scene Data Release..
	scene->second->Release();

	// Scene 제거..
	//m_SceneList.erase(scene);
}