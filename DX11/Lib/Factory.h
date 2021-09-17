#pragma once
class Factory
{
public:
	Factory();
	~Factory();

public:
	void SetManager(CameraManager* camM, LightManager* lightM, HelperManager* helperM, ObjectManager* objM, MaterialManager* matM, CollisionManager* colM, UIManager* uiM);

public: 
	/// <summary>
	/// 오브젝트 생성 함수
	/// </summary>
	/// <param name="objName">Resource Name(Key)</param>
	/// <param name="modelType">Create Model Type</param>
	/// <param name="shaderType">Apply Shader Type</param>
	/// <param name="scale">Object Scale</param>
	/// <returns>Final GameObject</returns>
	GameObject* CreateObject(std::string objName, eModelType modelType, DXVector3 scale = DXVector3::One(), bool isCol = true, eModelCollider modelColType = eModelCollider::ALL);

	void SetRenderer(GameObject* obj, std::string meshKey, ParserData::Mesh* mesh, VertexBuffer* vBuffer);
	void SetCollider(GameObject* obj, VertexBuffer* vBuffer);

	void CreateBoxCollider(GameObject* obj, DXVector3 size);
	void CreateSphereCollider(GameObject* obj, float radius);

	GameObject* CreateUI(std::string objName, eUIType uiType, std::string texKey, DXVector2 scale, DXVector2 pos, bool isCol = false);
	
	/// <summary>
	/// 애니메이션 생성 함수 ver.1
	/// </summary>
	/// <param name="objName">File Key Name</param>
	/// <param name="topNode">Top Object</param>
	/// <returns>Final Animation Component</returns>
	Animation* CreateAnimation(std::string objName, std::string aniName, GameObject* topNode, bool play = false);
	
	/// <summary>
	/// 애니메이션 생성 함수 ver.2
	/// </summary>
	/// <param name="objName">File Key Name</param>
	/// <param name="nodeName">Top Object Name</param>
	/// <returns></returns>
	Animation* CreateAnimation(std::string objName, std::string aniName, std::string nodeName, bool play = false);

private:
	CameraManager* m_CamMG;
	LightManager* m_LightMG;
	HelperManager* m_HelpMG;
	ObjectManager* m_ObjMG;
	MaterialManager* m_MatMG;
	CollisionManager* m_ColMG;
	UIManager* m_UiMG;
	ResourceManager* m_RsMG;
};

