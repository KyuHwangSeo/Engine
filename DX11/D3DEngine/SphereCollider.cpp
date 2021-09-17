#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "GeometryGenerator.h"
#include "Shader.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

SphereCollider::SphereCollider()
{
	m_ColType = eColliderType::Sphere;
}

SphereCollider::~SphereCollider()
{

}

void SphereCollider::LateUpdate(float dTime)
{
	m_OriginalBounds.Transform(m_NowBounds, m_GameObject->GetWorld());

	m_ColData.center = m_NowBounds.Center;
	m_ColData.radius = m_NowBounds.Radius;

	/// Debug Collider
	//BuildGeometryBuffers(m_NowBounds);
}

bool SphereCollider::CollisionCheck(Collider* col)
{
	switch (col->GetColliderType())
	{
	case eColliderType::Box:
	{
		CollisionData colData = col->GetBounds();

		if (m_NowBounds.Intersects(colData.GetBox3D()))
			return true;
		else
			return false;
	}
	break;
	case eColliderType::Sphere:
	{
		CollisionData colData = col->GetBounds();

		if (m_NowBounds.Intersects(colData.GetSphere3D()))
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

void SphereCollider::SetBoundSphere(VertexBuffer* vBuffer)
{
	if (vBuffer == nullptr) return;

	m_OriginalBounds = vBuffer->m_MeshSphere;

	BuildGeometryBuffers(m_OriginalBounds);
}

KH_API void SphereCollider::SetBoundSphere(DXVector3 center, float radius)
{
	m_OriginalBounds.Center = center;
	m_OriginalBounds.Radius = radius;

	BuildGeometryBuffers(m_OriginalBounds);
}

KH_API void SphereCollider::MovePosition(DXVector3 pos)
{
	m_OriginalBounds.Center = pos;
	BuildGeometryBuffers(m_OriginalBounds);
}

KH_API void SphereCollider::SetRange(float range)
{
	m_OriginalBounds.Radius = range;
	BuildGeometryBuffers(m_OriginalBounds);
}

void SphereCollider::BuildGeometryBuffers(DirectX::BoundingSphere bounds)
{
	GeometryGenerator::MeshData sphere;

	GeometryGenerator geoGen;

	geoGen.CreateGeosphere(bounds.Radius, 1, sphere);

	m_IndexCount = (UINT)sphere.Indices.size();
	m_Stride = sizeof(Vertex);
	m_Offset = 0;

	UINT totalVertexCount = (UINT)sphere.Vertices.size();

	std::vector<Vertex> vertices(totalVertexCount);
	for (size_t i = 0; i < sphere.Vertices.size(); ++i)
	{
		vertices[i].Pos = sphere.Vertices[i].Position + bounds.Center;
		vertices[i].Color.x = 0.0f;
		vertices[i].Color.y = 1.0f;
		vertices[i].Color.z = 0.0f;
		vertices[i].Color.w = 1.0f;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(D3DEngine::GetInstance()->GetDevice()->CreateBuffer(&vbd, &vinitData, &m_VB));

	std::vector<UINT> indices;
	indices.insert(indices.end(), sphere.Indices.begin(), sphere.Indices.end());

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * m_IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(D3DEngine::GetInstance()->GetDevice()->CreateBuffer(&ibd, &iinitData, &m_IB));
}
