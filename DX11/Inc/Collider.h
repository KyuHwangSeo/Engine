#pragma once
///
/// 2021/08/12 3:45
/// SeoKyuHwang
///
/// Collider Class
///
/// - Collision Check를 위한 각 Collider의 상위 클래스
/// - Collision 유무에 따른 충돌 체크는 Collision Manager에서 해준다
///
/// - 현재 Collider 타입별 컴포넌트마다 함수를 실행하여 충돌 체크를 하는데
///   유니티에서는 엔진 내부에서 충돌시에 OnCollision 관련 함수들을 호출해주는 것 같으나
///   현재로썬 생각나는것이 내부에서 처리해주는 방식..
/// - 반환형을 여러가지 방법으로 한 함수에서 사용할 수 있을지 모르겟다..
/// 
/// - 일단 시간이 없어서 각 컴포넌트 내부에서 처리후 true, false 반환으로 충돌 체크를 하는데
///   추후에 std::variant & std::visit 키워드를 좀 더 찾아보자..

//#include <variant>
//using ReturnType = std::variant<DirectX::BoundingBox, DirectX::BoundingSphere>;

#include "Collision.h"

class Collider : public Component
{
public:
	Collider();
	virtual ~Collider();

public:
	void Start();
	void Render(DXMatrix4X4 view, DXMatrix4X4 proj);
	void Release();

	virtual bool CollisionCheck(Collider* col) abstract;

	eColliderType GetColliderType() { return m_ColType; }
	CollisionData GetBounds() { return m_ColData; }

protected:
	ComPtr<ID3D11DeviceContext> m_DeviceContext;

	ComPtr<ID3D11Buffer> m_VB;		// 버텍스버퍼
	ComPtr<ID3D11Buffer> m_IB;		// 인덱스버퍼

	UINT m_IndexCount;
	UINT m_VertexCount;
	UINT m_Stride;
	UINT m_Offset;

	eColliderType m_ColType;

	Shader* m_Shader;

	cbPerObject m_ObjectData;

	CollisionData m_ColData;
};

