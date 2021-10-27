#pragma once
#include "../ShareData/ParserData.h"

/// <summary>
/// Renderer Class
/// </summary>
/// 
/// 모든 Renderer에 기반이 되는 Base Class
/// Renderer는 오직 Rendering만 하는것이 목표
/// 

class Renderer : public Component
{
public:
	Renderer(eRasterizerType renderType, eTopologyType topolType);
	virtual ~Renderer();

public:
	virtual void Render();
	virtual void Release();
	virtual void SetVertexBuffer(VertexBuffer* vBuffer);

	void AddMaterial();
	void SetMaterial(std::string matName);

	void SetRenderType(eRasterizerType renderType)			{ m_RasterizerType = renderType; };
	void SetTopologyType(eTopologyType topologyType)	{ m_TopologyType = topologyType; };

	virtual bool Pick(DXMatrix4X4 view, DXMatrix4X4 proj, int sw, int sh, int x, int y);
	virtual void SetMesh(ParserData::Mesh* mesh) { m_Mesh = mesh; }

	ParserData::Mesh* GetMesh() { return m_Mesh; }
	Material* GetMaterial() { return m_Material; }
	ID3D11Buffer* GetVertexBuffer() { return m_VB.Get(); }
	UINT GetBufferSize() { return m_Stride; }

public:
	std::unordered_map<std::string, Material*> m_MaterialList;
	Material* m_Material;

	std::string m_MaterialKey;

protected:
	ComPtr<ID3D11DeviceContext> m_DeviceContext;

	// Vertex & Index Buffer
	ComPtr<ID3D11Buffer> m_VB;
	ComPtr<ID3D11Buffer> m_IB;

	UINT m_IndexCount;
	UINT m_Stride;
	UINT m_Offset;

	// Shader
	Shader* m_ShadowShader;

	// Shader Constant Buffer
	cbPerObject m_ShadowObjectData;

	// Rendering Type
	eRasterizerType m_RasterizerType;
	eTopologyType m_TopologyType;

	// Mesh Data
	ParserData::Mesh* m_Mesh;
	DirectX::BoundingBox m_MeshBox;
	std::vector<DXVector3> m_VertexPos;
	std::vector<UINT> m_MeshIndices;
};

