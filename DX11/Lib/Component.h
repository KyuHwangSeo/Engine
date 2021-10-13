#pragma once
#include "Object.h"
#include "Collision.h"

/// <summary>
/// Component Class
/// </summary>
/// 
/// Component Base Design�� �ϱ� ���� Component Class
/// �� Component�� ����� GameObject�� Component�� �˰��ִ�

class GameObject;
class Component : public Object
{
public:
	Component() = default;
	virtual ~Component() = default;

protected:
	GameObject* m_GameObject = nullptr;

	bool m_Enabled = true;

public:
	virtual void Start() {}
	virtual void Update(float dTime) {}
	virtual void LateUpdate(float dTime) {}
	virtual void FinalUpdate(float dTime) {}

	virtual void CollisionStart(Collision collsion) {}
	virtual void CollisionStay(Collision collsion) {}
	virtual void CollisionExit(Collision collsion) {}

	virtual void ShadowRender(DXMatrix4X4 view, DXMatrix4X4 proj) {}
	virtual void Render(DXMatrix4X4 view, DXMatrix4X4 proj) {}

	virtual void Release() abstract;

	// GameObject Get & Set
	virtual void SetGameObject(GameObject* obj)		{ m_GameObject = obj; }
	GameObject* GetGameObject()						{ return m_GameObject; }

	// Component On & Off
	bool GetEnabled() { return m_Enabled; }
	void SetEnabled(bool enabled) { m_Enabled = enabled; }
};

