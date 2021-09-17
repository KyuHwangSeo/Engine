#pragma once
/// <summary>
/// MeshRenderer Class
/// </summary>
/// 
/// �Ϲ� Mesh�� Rendering �ϱ� ���� Component
/// Mesh Buffer �������� �������� ���ش�

class MeshRenderer : public Renderer
{
public:
	MeshRenderer(eRasterizerType renderType = eRasterizerType::Solid, eTopologyType topolType = eTopologyType::Triangle);
	~MeshRenderer();

public:
	void ShadowRender(DXMatrix4X4 view, DXMatrix4X4 proj);
	void Render(DXMatrix4X4 view, DXMatrix4X4 proj);

private:
	cbPerObject m_ObjectData;

};

