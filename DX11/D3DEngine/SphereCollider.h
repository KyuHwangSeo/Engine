#pragma once
///
/// 2021/08/15 1:31
/// SeoKyuHwang
///
/// SphereCollider Class
///
/// - SphereCollider Class
/// - 충돌 체크는 CollisionManager에서 체크

class SphereCollider : public Collider
{
public:
	SphereCollider();
	~SphereCollider();

public:
	void LateUpdate(float dTime);
	bool CollisionCheck(Collider* col);

	void SetBoundSphere(VertexBuffer* vBuffer);
	DLL_DECLSPEC void SetBoundSphere(DXVector3 center, float radius);
	DLL_DECLSPEC void MovePosition(DXVector3 pos);
	DLL_DECLSPEC void SetRange(float range);

	void BuildGeometryBuffers(DirectX::BoundingSphere bounds);

private:
	DirectX::BoundingSphere m_OriginalBounds;
	DirectX::BoundingSphere m_NowBounds;
};

