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
	ENGINE_DLL void SetPosition(DXVector3 pos);
	ENGINE_DLL void SetPositionX(float x);
	ENGINE_DLL void SetPositionY(float y);
	ENGINE_DLL void SetPositionZ(float z);
	ENGINE_DLL void SetRange(DXVector3 range);
	ENGINE_DLL void SetRangeX(float range);
	ENGINE_DLL void SetRangeY(float range);
	ENGINE_DLL void SetRangeZ(float range);

	void BuildGeometryBuffers(DirectX::BoundingBox bounds);

private:
	DirectX::BoundingBox m_OriginalBounds;
	DirectX::BoundingBox m_NowBounds;
};

