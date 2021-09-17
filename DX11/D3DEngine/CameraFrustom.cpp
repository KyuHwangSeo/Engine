#include "D3DDefine.h"
#include "Object.h"
#include "CameraFrustom.h"

CameraFrustom::CameraFrustom()
{
	
}

CameraFrustom::~CameraFrustom()
{

}

void CameraFrustom::ComputeFrustum(XMMATRIX proj)
{
	ComputeFrustumFromProjection(&m_Frustom, &proj);
}

void CameraFrustom::BuildGeometryBuffers(XMMATRIX viewproj)
{
	//DXVector4 HomogenousPoints[5] =
	//{
	//	{ -1.0f, 1.0f, 1.0f, 1.0f },	// left top

	//	{ 1.0f, 1.0f, 1.0f, 1.0f },		// right top 

	//	{ 1.0f, -1.0f, 1.0f, 1.0f },	// right bottom

	//	{ -1.0f, -1.0f, 1.0f, 1.0f },	// left	bottom

	//	{ 0.0f, 0.0f, 0.0f, 1.0f }, 	// near
	//};

	//XMVECTOR Determinant;

	//XMMATRIX matInverse = XMMatrixInverse(&Determinant, viewproj);

	//// Compute the frustum corners in world space.

	//DXVector4 Points[5];

	//for (INT i = 0; i < 5; i++)
	//{
	//	// Transform point.
	//	Points[i] = XMVector4Transform(HomogenousPoints[i], matInverse);
	//}

	//std::vector<Vertex>vertices(5);

	//for (UINT i = 0; i < vertices.size(); i++)
	//{
	//	vertices[i].Pos.x = Points[i].x / Points[i].w;
	//	vertices[i].Pos.y = Points[i].y / Points[i].w;
	//	vertices[i].Pos.z = Points[i].z / Points[i].w;

	//	vertices[i].Color.x = 1.0f;
	//	vertices[i].Color.y = 1.0f;
	//	vertices[i].Color.z = 1.0f;
	//	vertices[i].Color.w = 1.0f;
	//}

	//D3D11_BUFFER_DESC vbd;
	//vbd.Usage = D3D11_USAGE_IMMUTABLE;
	//vbd.ByteWidth = sizeof(Vertex) * 5;
	//vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//vbd.CPUAccessFlags = 0;
	//vbd.MiscFlags = 0;
	//vbd.StructureByteStride = 0;
	//D3D11_SUBRESOURCE_DATA vinitData;
	//vinitData.pSysMem = &vertices[0];
	//HR(m_Device->CreateBuffer(&vbd, &vinitData, &m_VB));

	//UINT indices[] = {
	//	4,0,
	//	4,1,
	//	4,2,
	//	4,3,
	//	0,1,
	//	1,2,
	//	2,3,
	//	3,0,
	//};

	//m_IndexCount = 2 * 8;

	//D3D11_BUFFER_DESC ibd;
	//ibd.Usage = D3D11_USAGE_IMMUTABLE;
	//ibd.ByteWidth = sizeof(UINT) * m_IndexCount;
	//ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//ibd.CPUAccessFlags = 0;
	//ibd.MiscFlags = 0;
	//ibd.StructureByteStride = 0;
	//D3D11_SUBRESOURCE_DATA iinitData;
	//iinitData.pSysMem = indices;
	//HR(m_Device->CreateBuffer(&ibd, &iinitData, &m_IB));
}