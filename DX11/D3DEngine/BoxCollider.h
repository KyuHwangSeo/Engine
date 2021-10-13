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
	DLL_DECLSPEC void SetPosition(DXVector3 pos);
	DLL_DECLSPEC void SetPositionX(float x);
	DLL_DECLSPEC void SetPositionY(float y);
	DLL_DECLSPEC void SetPositionZ(float z);
	DLL_DECLSPEC void SetRange(DXVector3 range);
	DLL_DECLSPEC void SetRangeX(float range);
	DLL_DECLSPEC void SetRangeY(float range);
	DLL_DECLSPEC void SetRangeZ(float range);

	void BuildGeometryBuffers(DirectX::BoundingBox bounds);

private:
	DirectX::BoundingBox m_OriginalBounds;
	DirectX::BoundingBox m_NowBounds;
};

