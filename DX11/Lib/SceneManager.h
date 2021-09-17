#pragma once
class SceneManager
{
public:
	SceneManager();
	~SceneManager();

public:
	void AddScene(std::string name, Scene* scene);
	Scene* SelectScene(std::string name);
	void RemoveScene(std::string name);

	Scene* GetScene() { return m_NowScene; }

private:
	std::unordered_map<std::string, Scene*> m_SceneList;
	Scene* m_NowScene;
};

