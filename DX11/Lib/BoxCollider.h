#pragma once
///
/// 2021/08/12 3:46
/// SeoKyuHwang
///
/// - BoxCollider Class
/// - 충돌 체크는 CollisionManager에서 체크

class BoxCollider : public Collider
{
public:
	BoxCollider();
	~BoxCollider();

public:
	void LateUpdate(float dTime);
	bool CollisionCheck(Collider* col);

	void SetBoundBox(VertexBuffer* vBuffer);
	void SetBoundBox(DXVector3 center, DXVector3 size);
	KH_API void SetPosition(DXVector3 pos);
	KH_API void SetPositionX(float x);
	KH_API void SetPositionY(float y);
	KH_API void SetPositionZ(float z);
	KH_API void SetRange(DXVector3 range);
	KH_API void SetRangeX(float range);
	KH_API void SetRangeY(float range);
	KH_API void SetRangeZ(float range);

	void BuildGeometryBuffers(DirectX::BoundingBox bounds);

private:
	DirectX::BoundingBox m_OriginalBounds;
	DirectX::BoundingBox m_NowBounds;
};

