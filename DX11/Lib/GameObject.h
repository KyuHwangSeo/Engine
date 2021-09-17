#pragma once
#include "Layer.h"

/// <summary>
/// GameObject Class
/// </summary>
/// 
/// - ���� -
/// 1. ������Ʈ�� ������ִ� ������Ʈ�� type_index�� Ű������ ����Ѵ� (unordered_multimap�� �������� ���� -> ���� ������ ����)
/// 2. AddComponent -> �ش� ������Ʈ�� GameObject�� ���� GameObject�� ���� �� ������Ʈ ����Ʈ�� �־�д�
/// 3. GetComponent -> ����� ��� ������Ʈ�� dynamic_cast �ϴ°��� ����� �ʹ� ���� ��� ���� Ű������ ã�� �ش� ������Ʈ�� ĳ�����Ѵ�
/// 4. enable_if -> ù ��° ���ø� �Ű� ������ ���� true�� ��� �ش� type���� Ȱ��ȭ
/// 5. is_base_of -> �� ��° ���ø� �Ű� ������ ù ��° ���ø� �Ű� ������ �⺻ Ŭ�����̸� true, �ƴҰ�� false
///					 ���� �Ű������� type�� Component�� ��츸 ������ ����
/// 
/// - �����ؾ� �� �� -
/// 1. AddComponent�� �ߺ��� ������Ʈ�� ó��
/// 2. Component Update�� �̺�Ʈ ť�� ���� Update ������ ������ �� �ְ� �ؾ� �� �� ���� ( Unity ������ MonoBehaviour Class�� ó�����ִ� �� ���� )
///    �̺�Ʈ �޼ҵ� �̸��� �˸� �ش� �޼ҵ带 ���� ��ų �� �ְ� �غ��� ������ ����

class Renderer;
class MeshRenderer;
class SkinMeshRenderer;
class CanvasRenderer;
class Transform;
class Collision;
class GameObject : public Object
{
public:
	GameObject();
	virtual ~GameObject();

protected:
	// ������Ʈ��
	unordered_multimap<type_index, Component*> m_Components;

	// Tag
	eTag m_Tag;

	// Layer
	Layer* m_Layer;

	// Transform
	Transform* m_Transform;

	// Picking
	bool m_Picking;

	// Enable
	bool m_Enable;

	// Mesh Rendering Type
	// dynamic_cast�� �ּ�ȭ �ϱ� ���� �߰�
	eMeshType m_MeshType;
	
public:
	// ������Ʈ �߰�
	template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type = std::is_base_of<Component, T>::value>
	void AddComponent(T* newCo);

	// ������Ʈ ����
	template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type = std::is_base_of<Component, T>::value>
	void RemoveComponent();

	// �ش� ������Ʈ ã��
	template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type = std::is_base_of<Component, T>::value>
	T* GetComponent();

	void Start();
	void Update(float dTime);
	void LateUpdate(float dTime);
	void FinalUpdate(float dTime);

	void CollisionStart(Collision collsion);
	void CollisionStay(Collision collsion);
	void CollisionExit(Collision collsion);

	void ShadowRender(DXMatrix4X4 view, DXMatrix4X4 proj);
	void Render(DXMatrix4X4 view, DXMatrix4X4 proj);
	void Release();
	bool Picking(DXMatrix4X4 view, DXMatrix4X4 proj, int sw, int sh, int x, int y);

	//bool operator== (const GameObject* obj) { return (m_Name == obj.m_Name) ? true : false; }
	bool operator== (string name)			{ return (m_Name == name) ? true : false; }

	// MeshType Get & Set
	void SetMeshType(eMeshType meshType)	{ m_MeshType = meshType; }
	eMeshType GetMeshType()					{ return m_MeshType; }

	// Tag Get & Set
	void SetTag(eTag tag)					{ m_Tag = tag; }
	eTag GetTag()							{ return m_Tag; }

	// Layer Get & Set
	void SetLayer(Layer* layer)				{ m_Layer = layer; }
	Layer* GetLayer()						{ return m_Layer; }
	string GetLayerName()					{ return m_Layer->m_LayerName; }
	int GetLayerKey()						{ return m_Layer->m_LayerKey; }
	bool CompareLayer(Layer* layer)			{ return m_Layer == layer; }

	// Enable Get & Set
	void SetEnabled(bool enabled)			{ m_Enable = enabled; }
	bool GetEnabled()						{ return m_Enable; }

	// Picking Get & Set
	void SetPick(bool pick)					{ m_Picking = pick; }
	bool GetPick()							{ return m_Picking; }

	// Transform Get & Set
	Transform* GetTransform()				{ return m_Transform; };
	DXVector3 GetPosition()					{ return m_Transform->GetPosition(); }
	DXMatrix4X4 GetWorld()					{ return m_Transform->GetWorldTM(); }
	DXMatrix4X4 GetPrevWorld()				{ return m_Transform->GetPrevWorldTM(); }
	DXMatrix4X4 GetLocal()					{ return m_Transform->GetLocalTM(); }

	void SetTransform(Transform* newTransform) { delete m_Transform; m_Transform = newTransform; }

	// Hierarchy Get & Set
	Transform* GetParent() { return m_Transform->m_Parent; }
	vector<Transform*> GetAllChild() { return m_Transform->m_Child; }
	void SetParent(Transform* parent) { m_Transform->m_Parent = parent; }
	void AddChild(Transform* child) { m_Transform->m_Child.push_back(child); }
};

template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type>
inline void GameObject::AddComponent(T* newCo)
{
	Component* _newCo = dynamic_cast<Component*>(newCo);
	_newCo->SetGameObject(this);
	_newCo->SetName<T>();

	m_Components.insert(make_pair(type_index(typeid(T)), _newCo));

	// �߰��� ������Ʈ�� ���������� üũ �� ����..
	if (std::is_base_of<MeshRenderer, T>::value == true)
	{
		m_MeshType = eMeshType::NormalMesh;
	}
	if (std::is_base_of<SkinMeshRenderer, T>::value == true)
	{
		m_MeshType = eMeshType::SkinMesh;
	}
	if (std::is_base_of<CanvasRenderer, T>::value == true)
	{
		m_MeshType = eMeshType::Canvas;
	}
}

template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type>
inline void GameObject::RemoveComponent()
{
	m_Components.erase(type_index(typeid(T)));

	// ������ ������Ʈ�� �����Ұ�� �ʱ�ȭ..
	if (std::is_base_of<Renderer, T>::value == true)
	{
		m_MeshType = eMeshType::None;
	}
}

template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type>
inline T* GameObject::GetComponent()
{
	unordered_multimap<type_index, Component*>::iterator it = m_Components.find(type_index(typeid(T)));
	if (it != m_Components.end())
		return dynamic_cast<T*>(it->second);
	else
		return nullptr;
}

