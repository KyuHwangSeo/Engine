#pragma once
///
/// 2021/08/18 22:42
/// SeoKyuHwang
///
/// BoxCollider2D Class
///
/// - 2D Box Collider
/// - 2D 간의 충돌체크

class BoxCollider2D : public Collider
{
public:
	BoxCollider2D();
	~BoxCollider2D();

public:
	void LateUpdate(float dTime);
	bool CollisionCheck(Collider* col);

	void SetBoundBox2D(DXVector2 center, DXVector2 size);

private:
	Square m_Box2D;
};

