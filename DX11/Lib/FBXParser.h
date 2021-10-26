#pragma once

typedef std::pair<std::string, ParserData::Bone*> BonePair;

class FBXParser : public IParser
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

	std::string fbxFileName;
	std::vector<fbxsdk::FbxSurfaceMaterial*> fbxMaterials;

private:
	ParserData::Model* m_Model;

	ParserData::CMaterial* m_MaterialData;			// Material Data Struct

	ParserData::Mesh* m_OneMesh;					// Mesh Data Struct

	ParserData::Bone* m_OneBone;					// Bone Data Struct
	std::vector<BonePair> m_AllBoneList;			// Skinning Object Bone List

	ParserData::OneAnimation* m_OneAnimation;		// Object One Animation Data

	bool m_OnlyAni = false;

	std::string m_TextureRoute;

public:
	void Initialize();
	void SetTextureRoute(std::string texRoute);
	void Release();
	ParserData::Model* LoadModel(std::string fileName, bool scaling, bool onlyAni = false);
	
	void SceneSetting(std::string fileName, bool scaling, bool onlyAni);
	void CreateModel();
	void ResetData();

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

	DirectX::SimpleMath::Vector2 ConvertVector2(fbxsdk::FbxVector2 v2);
	DirectX::SimpleMath::Vector2 ConvertVector2(fbxsdk::FbxVector4 v4);
	DirectX::SimpleMath::Vector3 ConvertVector3(fbxsdk::FbxVector4 v4);
	DirectX::SimpleMath::Vector3 NoConvertVector3(fbxsdk::FbxVector4 v4);
	DirectX::SimpleMath::Vector4 ConvertVector4(fbxsdk::FbxVector4 v4);
	DirectX::SimpleMath::Vector4 NoConvertVector4(fbxsdk::FbxVector4 v4);
	DirectX::SimpleMath::Matrix ConvertMatrix(fbxsdk::FbxMatrix matrix);
	DirectX::SimpleMath::Matrix NoConvertMatrix(fbxsdk::FbxMatrix matrix);
	std::string ConvertFileRoute(const char* fileName);

	DirectX::SimpleMath::Vector3 GetPos(fbxsdk::FbxMesh* mesh, int vertexIndex);
	DirectX::SimpleMath::Vector3 GetNormal(fbxsdk::FbxMesh* mesh, int vertexIndex, int vertexCount);
	DirectX::SimpleMath::Vector2 GetUV(fbxsdk::FbxMesh* mesh, int vertexIndex, int uvIndex);
	void LinkMaterialByPolygon(fbxsdk::FbxMesh* mesh, int polygonIndex, int vertexIndex);

	void SetTransform(fbxsdk::FbxNode* node);
	DirectX::SimpleMath::Matrix GetGlobalAnimationTransform(fbxsdk::FbxNode* node, fbxsdk::FbxTime time);

	int GetMaterialIndex(fbxsdk::FbxSurfaceMaterial* material);
	void SetMaterial(fbxsdk::FbxSurfaceMaterial* material);
	void SetTexture(fbxsdk::FbxSurfaceMaterial* material, const char* materialName);

	void CreateVertex(fbxsdk::FbxMesh* mesh, std::vector<BoneWeights>& boneWeights, int vertexCount);
	void CreateMesh();
	void CreateBone();

	int FindBoneIndex(std::string boneName);
	ParserData::Bone* FindBone(std::string boneName);
	ParserData::Mesh* FindMesh(std::string meshName);

	PARSER_DLL ParserData::Model* GetModel() { return m_Model; }
};

inline DirectX::SimpleMath::Vector2 FBXParser::ConvertVector2(fbxsdk::FbxVector2 v2)
{
	return DirectX::SimpleMath::Vector2
	(
		static_cast<float>(v2.mData[0]),
		1.0f - static_cast<float>(v2.mData[1])
	);
}

inline DirectX::SimpleMath::Vector2 FBXParser::ConvertVector2(fbxsdk::FbxVector4 v4)
{
	return DirectX::SimpleMath::Vector2
	(
		static_cast<float>(v4.mData[0]),
		1.0f - static_cast<float>(v4.mData[1])
	);
}

inline DirectX::SimpleMath::Vector3 FBXParser::ConvertVector3(fbxsdk::FbxVector4 v4)
{
	// xyz -> xzy
	return DirectX::SimpleMath::Vector3
	(
		static_cast<float>(v4.mData[0]),
		static_cast<float>(v4.mData[2]),
		static_cast<float>(v4.mData[1])
	);
}

inline DirectX::SimpleMath::Vector3 FBXParser::NoConvertVector3(fbxsdk::FbxVector4 v4)
{
	// xyz -> xzy
	return DirectX::SimpleMath::Vector3
	(
		static_cast<float>(v4.mData[0]),
		static_cast<float>(v4.mData[1]),
		static_cast<float>(v4.mData[2])
	);
}

inline DirectX::SimpleMath::Vector4 FBXParser::ConvertVector4(fbxsdk::FbxVector4 v4)
{
	// xyzw -> xzyw
	return DirectX::SimpleMath::Vector4
	(
		static_cast<float>(v4.mData[0]),
		static_cast<float>(v4.mData[2]),
		static_cast<float>(v4.mData[1]),
		static_cast<float>(v4.mData[3])
	);
}

inline DirectX::SimpleMath::Vector4 FBXParser::NoConvertVector4(fbxsdk::FbxVector4 v4)
{
	// xyzw -> xzyw
	return DirectX::SimpleMath::Vector4
	(
		static_cast<float>(v4.mData[0]),
		static_cast<float>(v4.mData[1]),
		static_cast<float>(v4.mData[2]),
		static_cast<float>(v4.mData[3])
	);
}

inline DirectX::SimpleMath::Matrix FBXParser::ConvertMatrix(fbxsdk::FbxMatrix matrix)
{
	FbxVector4 r1 = matrix.GetRow(0);
	FbxVector4 r2 = matrix.GetRow(1);
	FbxVector4 r3 = matrix.GetRow(2);
	FbxVector4 r4 = matrix.GetRow(3);

	// 2행, 3행 변경..
	return DirectX::SimpleMath::Matrix
	(
		ConvertVector4(r1),
		ConvertVector4(r3),
		ConvertVector4(r2),
		ConvertVector4(r4)
	);
}

inline DirectX::SimpleMath::Matrix FBXParser::NoConvertMatrix(fbxsdk::FbxMatrix matrix)
{
	FbxVector4 r1 = matrix.GetRow(0);
	FbxVector4 r2 = matrix.GetRow(1);
	FbxVector4 r3 = matrix.GetRow(2);
	FbxVector4 r4 = matrix.GetRow(3);

	// 2행, 3행 변경..
	return DirectX::SimpleMath::Matrix
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
