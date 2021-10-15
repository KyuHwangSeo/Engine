#pragma once
///
/// 2021/08/15 1:31
/// SeoKyuHwang
///
/// SphereCollider Class
///
/// - SphereCollider Class
/// - �浹 üũ�� CollisionManager���� üũ

class SphereCollider : public Collider
{
public:
	SphereCollider();
	~SphereCollider();

public:
	void LateUpdate(float dTime);
	bool CollisionCheck(Collider* col);

	void SetBoundSphere(VertexBuffer* vBuffer);
	ENGINE_DLL void SetBoundSphere(DXVector3 center, float radius);
	ENGINE_DLL void MovePosition(DXVector3 pos);
	ENGINE_DLL void SetRange(float range);

	void BuildGeometryBuffers(DirectX::BoundingSphere bounds);

private:
	DirectX::BoundingSphere m_OriginalBounds;
	DirectX::BoundingSphere m_NowBounds;
};

