#pragma once

class LightManager : public IManager
{
public:
	LightManager();
	~LightManager();

public:
	void Start();
	void Update(float dTime);
	void Render(DXMatrix4X4 view, DXMatrix4X4 proj);
	void AddObject(GameObject* obj);

	void SetLightData_Forward();
	void SetLightData_Deferred();

	void Release();

	GameObject* FindObject(string objName);

	bool m_IsLight = true;

private:
	vector<GameObject*> m_LightObjectList;

	cbLights m_LightData;
	cbLightList m_LightListData;
	cbCamera m_CameraData;
	cbPerObject m_ObjectData;
	cbTexViewProj m_TexViewProj;

	DXMatrix4X4 m_TextureSpace;

	Shader* m_LightShader;

	// Light 종류에 따른 Light Component List
	vector<Light*> m_PointLightList;
	vector<Light*> m_SpotLightList;
};

