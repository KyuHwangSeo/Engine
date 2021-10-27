#pragma once
///
/// 2021/08/12 3:40
/// SeoKyuHwang
///
/// CollisionManager Class
///
/// - ������ Layer�� �ְ� Collision Check�� �ʿ��� ����Ʈ�� ���� �浹 üũ
/// - ModelCollider List�� ���� Collider�� �ش� ���� �ֻ��� ������Ʈ�� ����
/// - ���� �浹�� ���� ��ó���� �ʿ��ϸ� GameObject�� Collision ������ �̿��Ͽ� ó������
/// - �浹 ��� ���� ������ ��� �������� �����
///
/// - Unity������ Collider�� �ִ� ������Ʈ���� �浹 ������ ���� ���ο��� �� ��
///   �浹 �ÿ� �ݹ��Լ��� ȣ�����ִµ� �� �ݹ� �Լ��� ���
///   ���� �ܺο��� �ݹ��Լ��� �������̵� �� �� ������ ������ �� �𸣰ٴ�.. ���� ����� �ʿ� �ҵ�
/// 
/// - ���� ���� ��� -
/// -> ���� ������Ʈ�� ���� �����ÿ� �� �����͸� ������� �ֻ��� ���� ���� ������
///    �浹üũ ��ü�� �ֻ������ ��� �ڽĵ��� üũ�Ͽ� �ֻ��� ��忡 �浹 ������ �Ѱ��ִ� ��Ȳ
/// -> ���Ŀ� �� ��� ���� üũ�ϴ� ������� ������ �ʿ� ����

struct ModelCollider
{
	ModelCollider(GameObject* obj) : m_TopObject(obj), m_CollisionState(eCollisionState::None) {}

	// �ش� ���� ��� Collider List
	std::vector<Collider*> m_ColliderList;

	// �ش� ���� �ֻ��� ���
	GameObject* m_TopObject;

	// ���� ���� �浹 ����
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

	// �ش� UI�� Collider
	Collider* m_Collider;

	// ���� UI�� �浹 ����
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

