#pragma once

typedef std::pair<std::string, ParserData::Bone*> BonePair;

struct PARSER_DLL FBXModel
{
	bool m_isAnimation = false;									// Animation 유무

	int	m_materialcount = 0;									// Material 개수
	std::vector<ParserData::CMaterial*> m_list_materialdata;	// Material List

	std::vector<ParserData::Mesh*> m_MeshList;	 				// Mesh List
};

class FBXParser
{
public:
	FBXParser() = default;
	~FBXParser() = default;

private:
	fbxsdk::FbxManager* pManager;
	fbxsdk::FbxImporter* pImporter;
	fbxsdk::FbxScene* pScene;
	fbxsdk::FbxGeometryConverter* pConverter;

	fbxsdk::FbxMesh* pMesh;

	fbxsdk::FbxString fbxFileName;
	std::vector<fbxsdk::FbxSurfaceMaterial*> fbxMaterials;

private:
	FBXModel* m_Model;

	ParserData::CMaterial* m_MaterialData;			// Material Data Struct

	ParserData::Mesh* m_OneMesh;					// Mesh Data Struct

	ParserData::Bone* m_OneBone;					// Bone Data Struct
	std::vector<BonePair> m_AllBoneList;			// Skinning Object Bone List

	ParserData::OneAnimation* m_OneAnimation;		// Object One Animation Data

	bool m_OnlyAni = false;

	std::string m_TextureRoute;

public:
	PARSER_DLL void Initialize(std::string texRoute);
	PARSER_DLL void Release();
	void SceneSetting(fbxsdk::FbxString fileName, bool scaling, bool onlyAni);
	void ResetModel();

	PARSER_DLL void LoadScene(fbxsdk::FbxString fileName, bool scaling, bool onlyAni = false);
	void LoadMaterial();
	void LoadNode(fbxsdk::FbxNode* node, fbxsdk::FbxNodeAttribute::EType attribute);
	void LoadAnimation(fbxsdk::FbxNode* node);

	void ProcessSkeleton(fbxsdk::FbxNode* node);
	void ProcessMesh(fbxsdk::FbxNode* node);

	bool ProcessBoneWeights(fbxsdk::FbxNode* node, std::vector<BoneWeights>& meshBoneWeights);

	void ProcessAnimation(fbxsdk::FbxNode* node);

	void OptimizeData();
	void OptimizeVertex(ParserData::Mesh* pMesh);
	void RecombinationTM(ParserData::Mesh* pMesh);

	DXVector2 ConvertVector2(FbxVector2 v2);
	DXVector2 ConvertVector2(FbxVector4 v4);
	DXVector3 ConvertVector3(FbxVector4 v4);
	DXVector3 NoConvertVector3(FbxVector4 v4);
	DXVector4 ConvertVector4(FbxVector4 v4);
	DXVector4 NoConvertVector4(FbxVector4 v4);
	DXMatrix4X4 ConvertMatrix(FbxMatrix matrix);
	DXMatrix4X4 NoConvertMatrix(FbxMatrix matrix);
	std::string ConvertFileRoute(const char* fileName);

	DXVector3 GetPos(fbxsdk::FbxMesh* mesh, int vertexIndex);
	DXVector3 GetNormal(fbxsdk::FbxMesh* mesh, int vertexIndex, int vertexCount);
	DXVector2 GetUV(fbxsdk::FbxMesh* mesh, int vertexIndex, int uvIndex);
	void LinkMaterialByPolygon(fbxsdk::FbxMesh* mesh, int polygonIndex, int vertexIndex);

	void SetTransform(fbxsdk::FbxNode* node);
	DXMatrix4X4 GetGlobalAnimationTransform(fbxsdk::FbxNode* node, fbxsdk::FbxTime time);

	int GetMaterialIndex(fbxsdk::FbxSurfaceMaterial* material);
	void SetMaterial(fbxsdk::FbxSurfaceMaterial* material);
	void SetTexture(fbxsdk::FbxSurfaceMaterial* material, const char* materialName);

	void CreateVertex(fbxsdk::FbxMesh* mesh, std::vector<BoneWeights>& boneWeights, int vertexCount);
	void CreateMesh();
	void CreateBone();

	int FindBoneIndex(std::string boneName);
	ParserData::Bone* FindBone(std::string boneName);
	ParserData::Mesh* FindMesh(std::string meshName);

	PARSER_DLL FBXModel* GetModel() { return m_Model; }
};

inline DXVector2 FBXParser::ConvertVector2(FbxVector2 v2)
{
	return DXVector2
	(
		static_cast<float>(v2.mData[0]),
		1.0f - static_cast<float>(v2.mData[1])
	);
}

inline DXVector2 FBXParser::ConvertVector2(FbxVector4 v4)
{
	return DXVector2
	(
		static_cast<float>(v4.mData[0]),
		1.0f - static_cast<float>(v4.mData[1])
	);
}

inline DXVector3 FBXParser::ConvertVector3(FbxVector4 v4)
{
	// xyz -> xzy
	return DXVector3
	(
		static_cast<float>(v4.mData[0]),
		static_cast<float>(v4.mData[2]),
		static_cast<float>(v4.mData[1])
	);
}

inline DXVector3 FBXParser::NoConvertVector3(FbxVector4 v4)
{
	// xyz -> xzy
	return DXVector3
	(
		static_cast<float>(v4.mData[0]),
		static_cast<float>(v4.mData[1]),
		static_cast<float>(v4.mData[2])
	);
}

inline DXVector4 FBXParser::ConvertVector4(FbxVector4 v4)
{
	// xyzw -> xzyw
	return DXVector4
	(
		static_cast<float>(v4.mData[0]),
		static_cast<float>(v4.mData[2]),
		static_cast<float>(v4.mData[1]),
		static_cast<float>(v4.mData[3])
	);
}

inline DXVector4 FBXParser::NoConvertVector4(FbxVector4 v4)
{
	// xyzw -> xzyw
	return DXVector4
	(
		static_cast<float>(v4.mData[0]),
		static_cast<float>(v4.mData[1]),
		static_cast<float>(v4.mData[2]),
		static_cast<float>(v4.mData[3])
	);
}

inline DXMatrix4X4 FBXParser::ConvertMatrix(FbxMatrix matrix)
{
	FbxVector4 r1 = matrix.GetRow(0);
	FbxVector4 r2 = matrix.GetRow(1);
	FbxVector4 r3 = matrix.GetRow(2);
	FbxVector4 r4 = matrix.GetRow(3);

	// 2행, 3행 변경..
	return DXMatrix4X4
	(
		ConvertVector4(r1),
		ConvertVector4(r3),
		ConvertVector4(r2),
		ConvertVector4(r4)
	);
}

inline DXMatrix4X4 FBXParser::NoConvertMatrix(FbxMatrix matrix)
{
	FbxVector4 r1 = matrix.GetRow(0);
	FbxVector4 r2 = matrix.GetRow(1);
	FbxVector4 r3 = matrix.GetRow(2);
	FbxVector4 r4 = matrix.GetRow(3);

	// 2행, 3행 변경..
	return DXMatrix4X4
	(
		ConvertVector4(r1),
		ConvertVector4(r3),
		ConvertVector4(r2),
		NoConvertVector4(r4)
	);
}

inline std::string FBXParser::ConvertFileRoute(const char* fileName)
{
	std::string filePath = fileName;

	size_t indexSlash = filePath.rfind("\\") + 1;
	size_t indexDot = filePath.rfind(".");

	// 파일 임시경로..
	std::string fileRoute = m_TextureRoute;

	std::string fileame = filePath.substr(indexSlash, indexDot - indexSlash);
	fileRoute += filePath.substr(indexSlash, filePath.size() - indexSlash);

	return fileRoute;
}
