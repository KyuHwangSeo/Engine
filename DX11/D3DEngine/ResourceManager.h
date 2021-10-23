#pragma once
#include "KHParser.h"
/// 부가기능들을 모아논 클래스
/// 오브젝트마다 다양하게 사용되는 것들을 포함시키기 싫어서 임시로 만든 클래스..

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

public:
	void Initialize();
	void Release();

	void CreateSamplerState();

	void SetShaderSamplerState();

	void LoadShader(std::string shaderName, std::string vSName, std::string pSName, std::string cSName);
	void LoadData_ASE(std::string objectName, std::string fileName = "");
	void LoadData_ASE_Animation(std::string objectName, std::string fileName = "");
	void LoadData_FBX(std::string objectName, std::string fileName = "", bool fbxScaling = true);
	void LoadData_FBX_Animation(std::string objectName, std::string fileName = "", bool fbxScaling = true);
	void LoadData_Terrain(std::string objectName, std::string fileName = "", bool fbxScaling = true);
	ENGINE_DLL void LoadData(eLoadType loadType, std::string objectName, std::string fileName, bool fbxScaling = true);
	void LoadData_MaterialList(std::string objectName);

	void LoadData_Text(std::string objectName, std::string fileName);
	void LoadData_Wave(UINT NumRows, UINT NumCols);
	void LoadData_Axis(std::string objectName, float length);
	void LoadData_Grid();
	void LoadData_GeoBox();
	void LoadData_GeoSphere();
	void LoadData_GeoCylinder();
	void LoadData_ColorBox();
	void LoadData_ScreenQuad();
	void LoadData_SsaoQuad();
	void LoadData_UI();
	void LoadData_Sky(float skySphereRadius);
	void LoadData_ASE_Gizmos(std::string objectName, std::string fileName);

	void LoadData_SkinMesh(std::string objectName, std::string key, ParserData::Mesh* meshData);
	void LoadData_Mesh(std::string objectName, std::string key, ParserData::Mesh* meshData);
	void LoadData_TerrainMesh(std::string objectName, std::string key, ParserData::Mesh* meshData);
	void LoadData_Animation(std::string objectName, std::string key, ParserData::Mesh* meshData);
	ENGINE_DLL void CreateBoneCollider(std::string objectName, eColliderType colType, float range);
	void SetBoxCollider(std::string objectName, std::string key, ParserData::Mesh* meshData, DXVector3 range);
	void SetSphereCollider(std::string objectName, std::string key, ParserData::Mesh* meshData, float range);

	void LoadData_Material(ParserData::CMaterial* materialData);
	void LoadData_Texture(std::string objectName, std::string fileName, ParserData::MaterialMap* materialmap = nullptr);
	ENGINE_DLL void ResetFBX();

public:
	ENGINE_DLL ID3D11ShaderResourceView* GetTexture(std::string meshName, eTextureType textureType);
	ENGINE_DLL ID3D11ShaderResourceView* GetTexture(std::string texName);
	CASEParser* GetASEParser(std::string objectName);
	FBXModel* GetFBXParser(std::string objectName);
	VertexBuffer* GetVertexBuffer(std::string key);
	ParserData::Mesh* GetMesh(std::string key);
	ParserData::Mesh* GetMesh(std::string objectName, int count);
	ParserData::OneAnimation* GetAnimation(std::string key);
	MaterialData GetMaterial(std::string key);
	std::string GetMeshName(std::string objectName, int count = 0);
	std::string GetMeshKey(std::string objectName, int count = 0);
	size_t GetMeshListSize(std::string objectName);
	Shader* GetShader(std::string name);
	unordered_map<std::string, Shader*> GetShaderList();


private:
	std::string m_TexRoute;
	std::string m_ModelRoute;

	FBXParser* m_FBXParser;
	ImageParser* m_ImgParser;

	///////////////////////////////////////////////////////////////////////
	// Game Object Resource
	///////////////////////////////////////////////////////////////////////
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;
	
	// Shader 리스트 변수..
	unordered_map<std::string, Shader*> m_ShaderList;

	// ASE Parser 리스트 변수..
	unordered_map<std::string, CASEParser*> m_ASEParserList;

	// FBX Parser 리스트 변수..
	unordered_map<std::string, FBXModel*> m_FBXParserList;

	// Texture 리스트 변수..
	unordered_map<std::string, ComPtr<ID3D11ShaderResourceView>> m_TexList;

	// VertexBuffer 리스트 변수..
	unordered_map<std::string, VertexBuffer*> m_VertexList;

	// Mesh 리스트 변수..
	unordered_map<std::string, ParserData::Mesh*> m_MeshList;

	// Animation 리스트 변수..
	unordered_map<std::string, ParserData::OneAnimation*> m_AnimationList;

	// Material 리스트 변수..
	unordered_map<std::string, MaterialData> m_MaterialList;


	///////////////////////////////////////////////////////////////////////
	// Device Resource
	///////////////////////////////////////////////////////////////////////

	// SamplerState 리스트 변수..
	unordered_map<std::string, ComPtr<ID3D11SamplerState>> m_SamplerList;
};

