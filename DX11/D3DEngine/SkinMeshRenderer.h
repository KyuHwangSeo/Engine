#pragma once
/// <summary>
/// SkinMeshRenderer Class
/// </summary>
/// 
/// Skinning Object�� Rendering�ϱ� ���� Component
/// �ش� Mesh ���� Bone���� OffsetTM�� ���Ͽ� Shader�� �����͸� �־� �׸���

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

