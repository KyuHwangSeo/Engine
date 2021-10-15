#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "GeometryGenerator.h"
#include "Shader.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

BoxCollider::BoxCollider()
{
	m_ColType = eColliderType::Box;
}

BoxCollider::~BoxCollider()
{

}

void BoxCollider::LateUpdate(float dTime)
{
	m_OriginalBounds.Transform(m_NowBounds, m_GameObject->GetWorld());

	m_ColData.center = m_NowBounds.Center;
	m_ColData.size3D = m_NowBounds.Extents;

	/// Debug Collider
	//BuildGeometryBuffers(m_NowBounds);
}

bool BoxCollider::CollisionCheck(Collider* col)
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

	return true;
}

void BoxCollider::SetBoundBox(VertexBuffer* vBuffer)
{
	if (vBuffer == nullptr) return;

	m_OriginalBounds = vBuffer->m_MeshBox;

	BuildGeometryBuffers(m_OriginalBounds);
}

void BoxCollider::SetBoundBox(DXVector3 center, DXVector3 size)
{
	m_OriginalBounds.Center = center;
	m_OriginalBounds.Extents = size;

	BuildGeometryBuffers(m_OriginalBounds);
}

ENGINE_DLL void BoxCollider::SetPosition(DXVector3 pos)
{
	m_OriginalBounds.Center = pos;
	BuildGeometryBuffers(m_OriginalBounds);
}

ENGINE_DLL void BoxCollider::SetPositionX(float x)
{
	m_OriginalBounds.Center.x = x;
	BuildGeometryBuffers(m_OriginalBounds);
}

ENGINE_DLL void BoxCollider::SetPositionY(float y)
{
	m_OriginalBounds.Center.x = y;
	BuildGeometryBuffers(m_OriginalBounds);
}

ENGINE_DLL void BoxCollider::SetPositionZ(float z)
{
	m_OriginalBounds.Center.x = z;
	BuildGeometryBuffers(m_OriginalBounds);
}

ENGINE_DLL void BoxCollider::SetRange(DXVector3 range)
{
	m_OriginalBounds.Extents = range;
	BuildGeometryBuffers(m_OriginalBounds);
}

ENGINE_DLL void BoxCollider::SetRangeX(float range)
{
	m_OriginalBounds.Extents.x = range;
	BuildGeometryBuffers(m_OriginalBounds);
}

ENGINE_DLL void BoxCollider::SetRangeY(float range)
{
	m_OriginalBounds.Extents.y = range;
	BuildGeometryBuffers(m_OriginalBounds);
}

ENGINE_DLL void BoxCollider::SetRangeZ(float range)
{
	m_OriginalBounds.Extents.z = range;
	BuildGeometryBuffers(m_OriginalBounds);
}

void BoxCollider::BuildGeometryBuffers(DirectX::BoundingBox bounds)
{
	GeometryGenerator::MeshData box;

	GeometryGenerator geoGen;

	geoGen.CreateBox(bounds.Center, bounds.Extents, box);

	m_IndexCount = (UINT)box.Indices.size();
	m_Stride = sizeof(Vertex);
	m_Offset = 0;

	UINT totalVertexCount = (UINT)box.Vertices.size();

	std::vector<Vertex> vertices(totalVertexCount);

	for (size_t i = 0; i < box.Vertices.size(); ++i)
	{
		vertices[i].Pos = box.Vertices[i].Position;
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
	indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());

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
