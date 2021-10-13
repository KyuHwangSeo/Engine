#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "GeometryGenerator.h"
#include "Shader.h"
#include "Collider.h"
#include "CircleCollider2D.h"
#include "Camera.h"

CircleCollider2D::CircleCollider2D()
{
	m_ColType = eColliderType::Circle2D;
}

CircleCollider2D::~CircleCollider2D()
{

}

void CircleCollider2D::Start()
{
	DXVector3 scale = m_GameObject->GetTransform()->m_Scale;
	m_Offset = DXVector2(scale.x / 2, scale.y / 2);

	Collider::Start();
}

void CircleCollider2D::LateUpdate(float dTime)
{
	m_ColData.center = m_GameObject->GetPosition();

	m_ColData.center.x += m_Offset.x;
	m_ColData.center.y += m_Offset.y;

	m_Circle2D.center.x = m_ColData.center.x;
	m_Circle2D.center.y = m_ColData.center.y;
}

bool CircleCollider2D::CollisionCheck(Collider* col)
{
	switch (col->GetColliderType())
	{
	case eColliderType::Box2D:
		break;
	case eColliderType::Circle2D:
	{
		CollisionData colData = col->GetBounds();

		if (CircleToCircle(colData.GetCircle2D()))
			return true;
		else
			return false;
	}
	break;
	default:
		break;
	}

	return false;
}

bool CircleCollider2D::CircleToCircle(Circle& circle)
{
	float dist = KH_MATH::GetDistance(circle.center, m_Circle2D.center);

	if (dist >= m_Circle2D.radius + circle.radius)
		return false;

	return true;
}

void CircleCollider2D::SetBoundCircle2D(DXVector2 center, float radius)
{
	m_Circle2D.center = center;
	m_Circle2D.radius = radius;

	m_ColData.center = DXVector3(center.x, center.y, 0.0f);
	m_ColData.radius = radius;

	BuildGeometryBuffers();
}

DLL_DECLSPEC void CircleCollider2D::SetRange(float radius)
{
	m_Circle2D.radius = radius;
	m_ColData.radius = radius;
	BuildGeometryBuffers();
}

DLL_DECLSPEC void CircleCollider2D::SetCenter(DXVector2 center)
{
	m_Circle2D.center = center;
	m_ColData.center = DXVector3(center.x, center.y, 0.0f);
	BuildGeometryBuffers();
}

void CircleCollider2D::BuildGeometryBuffers()
{
	std::vector<Vertex> vertices(4);

	vertices[0].Pos = DXVector3(1.0f, 0.0f, 0.0f);
	vertices[1].Pos = DXVector3(0.0f, 1.0f, 0.0f);
	vertices[2].Pos = DXVector3(0.0f, 0.0f, 0.0f);
	vertices[3].Pos = DXVector3(1.0f, 1.0f, 0.0f);

	vertices[0].Color = DXVector4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[1].Color = DXVector4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[2].Color = DXVector4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[3].Color = DXVector4(0.0f, 1.0f, 0.0f, 1.0f);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 4;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(D3DEngine::GetInstance()->GetDevice()->CreateBuffer(&vbd, &vinitData, &m_VB));

	m_Stride = sizeof(Vertex);
	m_IndexCount = 6;

	UINT indices[6] =
	{
		0, 1, 2,
		1, 0, 3
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	HR(D3DEngine::GetInstance()->GetDevice()->CreateBuffer(&ibd, &iinitData, &m_IB));
}
