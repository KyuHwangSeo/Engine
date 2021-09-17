#pragma once
typedef std::unordered_map<std::string, GameObject*> Model;

class ObjectManager : public IManager
{
public:
	ObjectManager();
	~ObjectManager();

public:
	void Start();
	void Update(float dTime);
	void FinalUpdate(float dTime);
	void ShadowRender(DXMatrix4X4 view, DXMatrix4X4 proj);
	void Render(DXMatrix4X4 view, DXMatrix4X4 proj);
	
	void AddObject(GameObject* obj);
	void AddModel(std::vector<GameObject*>& model);
	void AddTopObject(GameObject* obj);

	void ObjectUpdate();
	void SetHierarchy(std::vector<GameObject*>& objList);
	void SetBone(std::vector<GameObject*>& objList, GameObject* obj);
	void Picking(int x, int y, int width, int height);
	void ParentPick(bool pick);
	void ChildPick(Transform* parent, bool pick);

	void Release();

	GameObject* FindObject(string objName);
	Model FindModel(string objName);

	GameObject* GetNowObject();

private:
	Model m_TopObjectList;
	vector<Model> m_ModelList;

	GameObject* m_PickObject;

	std::string Model_Key;
};

