#pragma once
class Scene
{
public:
	ENGINE_DLL Scene();
	ENGINE_DLL virtual ~Scene() = default;

public:
	void Start();
	void Update(float dTime);
	void FinalUpdate(float dTime);
	void Render();

	virtual void Create() abstract;
	virtual void SceneUpdate(float dTime) abstract;
	virtual void DebugDraw() {}

	void SetCameraLens(int screenWidth, int screenHeight);
	void Picking(int x, int y, int screenWidth, int screenHeight);

	void CreateSkyBox();
	void SetSkyBox(const char* mapName);

	void SetRenderer(D3DRenderer* renderer) { m_Renderer = renderer; }

	GameObject* FindObject(const char* objName, eObjectType objType);

	void Release();
	ENGINE_DLL ResourceManager* GetRM();

private:
	void ShadowRender();
	void MainRender();
	void LateRender();

private:
	friend class SceneManager;

protected:
	D3DRenderer* m_Renderer;

	ResourceManager* m_RsMG;
	CameraManager* m_CamMG;
	LightManager* m_LightMG;
	HelperManager* m_HelpMG;
	ObjectManager* m_ObjMG;
	MaterialManager* m_MatMG;
	CollisionManager* m_ColMG;
	UIManager* m_UiMG;

	SkyBox* m_SkyBox;

	DXMatrix4X4 view;
	DXMatrix4X4 proj;
	DXMatrix4X4 ortho;

	POINT* m_ClientSize;
};

