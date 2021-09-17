#pragma once
#include "Layer.h"

/// <summary>
/// GameObject Class
/// </summary>
/// 
/// - 구성 -
/// 1. 오브젝트에 적용되있는 컴포넌트의 type_index를 키값으로 사용한다 (unordered_multimap은 정렬하지 않음 -> 삽입 순서로 보관)
/// 2. AddComponent -> 해당 컴포넌트의 GameObject를 현재 GameObject로 설정 후 컴포넌트 리스트에 넣어둔다
/// 3. GetComponent -> 적용된 모든 컴포넌트를 dynamic_cast 하는것은 비용이 너무 많이 들것 같아 키값으로 찾아 해당 컴포넌트만 캐스팅한다
/// 4. enable_if -> 첫 번째 템플릿 매개 변수에 따라 true일 경우 해당 type으로 활성화
/// 5. is_base_of -> 두 번째 템플릿 매개 변수는 첫 번째 템플릿 매개 변수가 기본 클래스이면 true, 아닐경우 false
///					 따라서 매개변수의 type이 Component일 경우만 컴파일 가능
/// 
/// - 수정해야 할 것 -
/// 1. AddComponent시 중복된 컴포넌트의 처리
/// 2. Component Update시 이벤트 큐를 만들어서 Update 순서를 지정할 수 있게 해야 할 것 같다 ( Unity 내에선 MonoBehaviour Class가 처리해주는 것 같다 )
///    이벤트 메소드 이름만 알면 해당 메소드를 실행 시킬 수 있게 해보면 좋을것 같다

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
	// 컴포넌트들
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
	// dynamic_cast를 최소화 하기 위해 추가
	eMeshType m_MeshType;
	
public:
	// 컴포넌트 추가
	template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type = std::is_base_of<Component, T>::value>
	void AddComponent(T* newCo);

	// 컴포넌트 제거
	template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type = std::is_base_of<Component, T>::value>
	void RemoveComponent();

	// 해당 컴포넌트 찾기
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

	// 추가된 컴포넌트가 렌더러인지 체크 및 설정..
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

	// 렌더러 컴포넌트를 제거할경우 초기화..
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

