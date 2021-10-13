#pragma once
///
/// 2021/08/18 22:42
/// SeoKyuHwang
///
/// CircleCollider2D Class
///
/// - 2D Circle Collider
/// - 2D 간의 충돌체크

class CircleCollider2D : public Collider
{
public:
	CircleCollider2D();
	~CircleCollider2D();

public:
	void Start();
	void LateUpdate(float dTime);
	bool CollisionCheck(Collider* col);

	bool CircleToCircle(Circle& circle);
	void SetBoundCircle2D(DXVector2 center, float radius);
	DLL_DECLSPEC void SetRange(float radius);
	DLL_DECLSPEC void SetCenter(DXVector2 center);

	void BuildGeometryBuffers();

private:
	Circle m_Circle2D;

	DXVector2 m_Offset;
};

