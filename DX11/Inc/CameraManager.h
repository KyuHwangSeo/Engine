#pragma once
class CameraManager : public IManager
{
public:
	CameraManager();
	~CameraManager();

public:
	void Start();
	void Update(float dTime);
	void Render(DXMatrix4X4 view, DXMatrix4X4 proj);
	void AddObject(GameObject* obj);
	void CameraUpdate(GameObject* cam, float dTime);
	void CameraSetLens(int screenWidth, int screenHeight);
	void CameraMove();

	void Release();

	GameObject* FindObject(string objName);

private:
	vector<GameObject*> m_CameraList;

	POINT* m_NowMousePos;
	POINT m_LastMousePos = { 0, };

	DXInput* m_Input;

	int Camera_Index;
};

