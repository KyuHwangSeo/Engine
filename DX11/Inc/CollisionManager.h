#pragma once
///
/// 2021/08/12 3:40
/// SeoKyuHwang
///
/// CollisionManager Class
///
/// - 동일한 Layer에 있고 Collision Check가 필요한 리스트들 끼리 충돌 체크
/// - ModelCollider List에 각종 Collider와 해당 모델의 최상위 오브젝트를 보관
/// - 사용시 충돌에 대한 후처리가 필요하면 GameObject의 Collision 변수를 이용하여 처리하자
/// - 충돌 대상에 대한 정보는 어디서 보관할지 고민중
///
/// - Unity에서는 Collider가 있는 오브젝트끼리 충돌 판정을 엔진 내부에서 한 후
///   충돌 시에 콜백함수를 호출해주는데 그 콜백 함수를 어떻게
///   엔진 외부에서 콜백함수를 오버라이딩 할 수 있을지 아직은 잘 모르겟다.. 좀더 고민이 필요 할듯
/// 
/// - 현재 구현 방식 -
/// -> 현재 프로젝트를 위해 생성시에 모델 데이터를 기반으로 최상위 노드로 묶기 때문에
///    충돌체크 자체를 최상위노드 기반 자식들을 체크하여 최상위 노드에 충돌 정보를 넘겨주는 상황
/// -> 추후에 각 노드 별로 체크하는 방식으로 변경할 필요 있음

struct ModelCollider
{
	ModelCollider(GameObject* obj) : m_TopObject(obj), m_CollisionState(eCollisionState::None) {}

	// 해당 모델의 노드 Collider List
	std::vector<Collider*> m_ColliderList;

	// 해당 모델의 최상위 노드
	GameObject* m_TopObject;

	// 현재 모델의 충돌 상태
	eCollisionState m_CollisionState;

	bool operator==(ModelCollider& m)
	{
		if (m_ColliderList == m.m_ColliderList && m_TopObject == m.m_TopObject)
			return true;
		else
			return false;
	}
};

struct UICollider
{
	UICollider(GameObject* obj, Collider* col) : m_Object(obj), m_Collider(col), m_CollisionState(eCollisionState::None) {}
	
	GameObject* m_Object;

	// 해당 UI의 Collider
	Collider* m_Collider;

	// 현재 UI의 충돌 상태
	eCollisionState m_CollisionState;

	bool operator==(UICollider& m)
	{
		if (m_Object == m.m_Object && m_Collider == m.m_Collider)
			return true;
		else
			return false;
	}
};

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

public:
	void Update(float dTime);
	void CollisionCheck3D(ModelCollider& model_1, ModelCollider& model_2);
	void CollisionCheck2D(UICollider& ui_1, UICollider& ui_2);
	
	void OnCollsion3D(ModelCollider& model_1, ModelCollider& model_2, GameObject* parts, GameObject* target);
	void NoneCollsion3D(ModelCollider& model_1, ModelCollider& model_2, GameObject* parts, GameObject* target);

	void OnCollsion2D(UICollider& ui_1, UICollider& ui_2);
	void NoneCollsion2D(UICollider& ui_1, UICollider& ui_2);

	void AddModel(std::vector<GameObject*>& model);
	void AddObject3D(GameObject* obj);
	void AddObject2D(GameObject* obj);

	void Release();

private:
	std::unordered_multimap<Layer*, ModelCollider> m_ModelCollider3DList;
	std::unordered_multimap<Layer*, UICollider> m_Collider2DList;
};

