#pragma once
#include "KHMath.h"

///
/// 2021/08/15 21:54
/// SeoKyuHwang
///
/// Collision Class
///
/// - 충돌시 충돌한 대상을 넘겨줄때 사용하는 클래스
/// - 일단 Collider가 붙은 컴포넌트들은 모두 충돌한 대상을 알고있다
/// - Collider가 없더라도 최상위 부모는 편리성을 위해 자식들의 충돌리스트를 공유받는다

class GameObject;
class Collision
{
public:
	Collision(GameObject* targetModel, GameObject* target, GameObject* parts, eCollisionState state) 
		: m_TargetModel(targetModel), m_Target(target), m_Parts(parts), m_Collision(state) {}
	Collision(GameObject* target, GameObject* parts, eCollisionState state)
		: m_TargetModel(nullptr), m_Target(target), m_Parts(parts), m_Collision(state) {}

	~Collision() = default;

	GameObject* m_TargetModel;
	GameObject* m_Target;

	GameObject* m_Parts;

	eCollisionState m_Collision;
};

/////////////////////////////////////////////////////////////////////////
// 2D Collider Struct
/////////////////////////////////////////////////////////////////////////

struct Square
{
	Square(DXVector2 _center, DXVector2 _size) : center(_center), size(_size) {}
	Square() = default;

	DXVector2 center;
	DXVector2 size;
};

struct Circle
{
	Circle(DXVector2 _center, float _radius) : center(_center), radius(_radius) {}
	Circle() = default;

	DXVector2 center;
	float radius;
};

// 모든 Collider 정보를 담고있는 구조체
struct CollisionData
{
	DXVector3 center;

	DXVector3 size3D;
	DXVector2 size2D;

	float radius;

	// 3D Collider
	DirectX::BoundingBox GetBox3D() { return DirectX::BoundingBox(center, size3D); }
	DirectX::BoundingSphere GetSphere3D() { return DirectX::BoundingSphere(center, radius); }

	// 2D Collider
	Square GetBox2D() { return Square(DXVector2(center.x, center.y), size2D); }
	Circle GetCircle2D() { return Circle(DXVector2(center.x, center.y), radius); }
};