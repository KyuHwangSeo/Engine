#pragma once
class UIManager : public IManager
{
public:
	UIManager();
	~UIManager();

public:
	void Start();
	void Update(float dTime);
	void Render(DXMatrix4X4 view, DXMatrix4X4 proj);
	void AddObject(GameObject* obj);

	void Release();

	GameObject* FindObject(std::string objName);

private:
	std::unordered_map<std::string, GameObject*> m_UIList;
};

