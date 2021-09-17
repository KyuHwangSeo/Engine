#pragma once
/// <summary>
/// MeshRenderer Class
/// </summary>
/// 
/// 일반 Mesh를 Rendering 하기 위한 Component
/// Mesh Buffer 기준으로 렌더링을 해준다

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

