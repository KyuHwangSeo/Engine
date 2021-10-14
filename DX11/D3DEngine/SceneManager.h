#pragma once
class SceneManager
{
public:
	SceneManager();
	~SceneManager();

public:
	void Initialize();

	void AddScene(std::string name, Scene* scene);
	Scene* SelectScene(std::string name);
	void RemoveScene(std::string name);
	void SetScene();

	void CreateHelper();

	Scene* GetScene() { return m_NowScene; }
	Factory* GetFactory() { return m_Factory; }

private:
	std::unordered_map<std::string, Scene*> m_SceneList;

	Factory* m_Factory;

	CameraManager* m_CamMG;
	LightManager* m_LightMG;
	HelperManager* m_HelpMG;
	ObjectManager* m_ObjMG;
	MaterialManager* m_MatMG;
	CollisionManager* m_ColMG;
	UIManager* m_UiMG;

	Scene* m_NowScene;

};

