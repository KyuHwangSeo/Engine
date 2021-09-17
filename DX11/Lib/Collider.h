#pragma once
///
/// 2021/08/12 3:45
/// SeoKyuHwang
///
/// Collider Class
///
/// - Collision Check�� ���� �� Collider�� ���� Ŭ����
/// - Collision ������ ���� �浹 üũ�� Collision Manager���� ���ش�
///
/// - ���� Collider Ÿ�Ժ� ������Ʈ���� �Լ��� �����Ͽ� �浹 üũ�� �ϴµ�
///   ����Ƽ������ ���� ���ο��� �浹�ÿ� OnCollision ���� �Լ����� ȣ�����ִ� �� ������
///   ����ν� �������°��� ���ο��� ó�����ִ� ���..
/// - ��ȯ���� �������� ������� �� �Լ����� ����� �� ������ �𸣰ٴ�..
/// 
/// - �ϴ� �ð��� ��� �� ������Ʈ ���ο��� ó���� true, false ��ȯ���� �浹 üũ�� �ϴµ�
///   ���Ŀ� std::variant & std::visit Ű���带 �� �� ã�ƺ���..

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

	ComPtr<ID3D11Buffer> m_VB;		// ���ؽ�����
	ComPtr<ID3D11Buffer> m_IB;		// �ε�������

	UINT m_IndexCount;
	UINT m_VertexCount;
	UINT m_Stride;
	UINT m_Offset;

	eColliderType m_ColType;

	Shader* m_Shader;

	cbPerObject m_ObjectData;

	CollisionData m_ColData;
};

