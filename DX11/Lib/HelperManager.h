#pragma once
class HelperManager : public IManager
{
public:
	HelperManager();
	~HelperManager();

public:
	void Start();
	void Update(float dTime);
	void Render(DXMatrix4X4 view, DXMatrix4X4 proj);
	void AddObject(GameObject* obj);
	
	void Release();

	GameObject* FindObject(string objName);

private:
	vector<GameObject*> m_HelperList;
};

