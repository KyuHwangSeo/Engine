#pragma once
/// <summary>
/// SkinMeshRenderer Class
/// </summary>
/// 
/// Skinning Object를 Rendering하기 위한 Component
/// 해당 Mesh 기준 Bone들의 OffsetTM을 구하여 Shader에 데이터를 넣어 그린다

class SkinMeshRenderer : public Renderer
{
public:
	SkinMeshRenderer(eRasterizerType renderType, eTopologyType topolType = eTopologyType::Triangle);
	~SkinMeshRenderer();

public:
	void ShadowRender(DXMatrix4X4 view, DXMatrix4X4 proj);
	void Render(DXMatrix4X4 view, DXMatrix4X4 proj);

	void Release();
	void SetMesh(ParserData::Mesh* mesh);
	void SetBone(GameObject* bone);
	void BoneUpdate();

private:
	cbSkinned m_BoneData;
	cbPerObject m_ObjectData;

	unordered_map<string, GameObject*> m_BoneList;
};

