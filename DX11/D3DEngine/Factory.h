#pragma once
class Factory
{
public:
	Factory(CameraManager* camM, LightManager* lightM, HelperManager* helperM, ObjectManager* objM, MaterialManager* matM, CollisionManager* colM, UIManager* uiM);
	~Factory();

public: 
	/// 오브젝트 생성 함수
	GameObject* CreateObject(std::string objName, eModelType modelType, DXVector3 scale = DXVector3::One(), bool isCol = true, eModelCollider modelColType = eModelCollider::ALL);

	void SetRenderer(GameObject* obj, std::string meshKey, ParserData::Mesh* mesh, VertexBuffer* vBuffer);
	void SetCollider(GameObject* obj, VertexBuffer* vBuffer);

	void CreateBoxCollider(GameObject* obj, DXVector3 size);
	void CreateSphereCollider(GameObject* obj, float radius);

	GameObject* CreateUI(std::string objName, eUIType uiType, std::string texKey, DXVector2 scale, DXVector2 pos, bool isCol = false);
	
	/// 애니메이션 생성 함수 ver.1
	Animation* CreateAnimation(std::string objName, std::string aniName, GameObject* topNode, bool play = false);
	
	/// 애니메이션 생성 함수 ver.2
	Animation* CreateAnimation(std::string objName, std::string aniName, std::string nodeName, bool play = false);

private:
	ResourceManager* m_RsMG;
	CameraManager* m_CamMG;
	LightManager* m_LightMG;
	HelperManager* m_HelpMG;
	ObjectManager* m_ObjMG;
	MaterialManager* m_MatMG;
	CollisionManager* m_ColMG;
	UIManager* m_UiMG;
};

