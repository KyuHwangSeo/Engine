#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Shader.h"
#include "../ShareData/ParserData.h"
#include "Material.h"
#include "Renderer.h"

Renderer::Renderer(eRasterizerType renderType, eTopologyType topolType)
	:m_RasterizerType(renderType), m_TopologyType(topolType)
{
	// 기본 Material 생성..
	AddMaterial();

	m_MaterialKey = m_MaterialList.begin()->first;
	m_Material = m_MaterialList.begin()->second;

	m_DeviceContext = D3DEngine::GetInstance()->GetDeviceContext();
}

Renderer::~Renderer()
{

}

void Renderer::Render()
{
	// Topology Type
	switch (m_TopologyType)
	{
	case eTopologyType::Triangle:
		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	case eTopologyType::Line:
		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		break;
	default:
		break;
	}
	
	m_DeviceContext->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &m_Stride, &m_Offset);
	m_DeviceContext->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Draw..
	m_DeviceContext->DrawIndexed(m_IndexCount, 0, 0);

	Shader::ResetVSResources();
	Shader::ResetPSResources();
}

void Renderer::Release()
{
	m_Material = nullptr;

	for (auto& k : m_MaterialList)
	{
		SAFE_DELETE(k.second);
	}

	m_MaterialList.clear();
}

void Renderer::SetVertexBuffer(VertexBuffer* vBuffer)
{
	if (vBuffer == nullptr)	return;

	m_VB = vBuffer->VB;
	m_IB = vBuffer->IB;

	m_Stride = vBuffer->Stride;
	m_IndexCount = vBuffer->IndexCount;
	m_Offset = vBuffer->Offset;

	m_MeshBox = vBuffer->m_MeshBox;

	m_VertexPos = vBuffer->m_VertexPos;
	m_MeshIndices = vBuffer->m_MeshIndices;
}

void Renderer::AddMaterial()
{
	Material* newMat = new Material();
	newMat->m_Name = typeid(Material).name() + to_string(m_MaterialList.size());

	m_MaterialList.insert(make_pair(newMat->m_Name, new Material()));
}

void Renderer::SetMaterial(string matName)
{
	m_MaterialKey = matName;

	m_Material = m_MaterialList.find(m_MaterialKey)->second;
}

bool Renderer::Pick(DXMatrix4X4 view, DXMatrix4X4 proj, int sw, int sh, int x, int y)
{
	return false;

	// Compute picking ray in view space.
	float vx = (+2.0f * x / sw - 1.0f) / proj._11;
	float vy = (-2.0f * y / sh + 1.0f) / proj._22;

	// Ray definition in view space.
	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	// Tranform ray to local space of Mesh.
	XMMATRIX V = view;
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);

	XMMATRIX W = m_GameObject->GetWorld();
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	rayDir = XMVector3TransformNormal(rayDir, toLocal);

	// Make the ray direction unit length for the intersection tests.
	rayDir = XMVector3Normalize(rayDir);

	// If we hit the bounding box of the Mesh, then we might have picked a Mesh triangle,
	// so do the ray/triangle tests.
	//
	// If we did not hit the bounding box, then it is impossible that we hit 
	// the Mesh, so do not waste effort doing ray/triangle tests.

	// Assume we have not picked anything yet, so init to -1.
	float tmin = 0.0f;
	if (XNA::IntersectRayAxisAlignedBox(rayOrigin, rayDir, &m_MeshBox, &tmin))
	{
		// Find the nearest ray/triangle intersection.
		tmin = MathHelper::Infinity;
		for (UINT i = 0; i < m_MeshIndices.size() / 3; ++i)
		{
			// Indices for this triangle.
			UINT i0 = m_MeshIndices[i * 3 + 0];
			UINT i1 = m_MeshIndices[i * 3 + 1];
			UINT i2 = m_MeshIndices[i * 3 + 2];

			// Vertices for this triangle.
			XMVECTOR v0 = m_VertexPos[i0];
			XMVECTOR v1 = m_VertexPos[i1];
			XMVECTOR v2 = m_VertexPos[i2];

			// We have to iterate over all the triangles in order to find the nearest intersection.
			float t = 0.0f;
			if (XNA::IntersectRayTriangle(rayOrigin, rayDir, v0, v1, v2, &t))
			{
				return true;
			}
		}
	}

	return false;
}
