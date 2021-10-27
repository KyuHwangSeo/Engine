#define SAFE_DELETE(x) { if(x != nullptr) {delete x; x = nullptr;} }

#include "ModelParser.h"

#define FBXSDK_SHARED
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk")

#include "BoneWeights.h"
#include "FBXParser.h"

using namespace fbxsdk;
using namespace ParserData;
using namespace DirectX;
using namespace SimpleMath;

void FBXParser::Initialize()
{
	// FBX SDK Manager ����..
	pManager = FbxManager::Create();

	if (!pManager)
		throw std::exception("error: unable to create FBX manager!\n");

	// Scene ����..
	pScene = FbxScene::Create(pManager, "My Scene");

	// IOSettings ��ü ���� �� ����
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	FbxString lExtension = "dll";
	FbxString lPath = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(lPath.Buffer(), lExtension.Buffer());

	// FbxImporter ��ü ����
	pImporter = FbxImporter::Create(pManager, "");

	// Convert ��ü ����
	pConverter = new FbxGeometryConverter(pManager);

	if (!pScene)
		throw std::exception("error: unable to create FBX scene\n");
}

void FBXParser::SetTextureRoute(std::string texRoute)
{
	// �⺻ Texture ���� ��Ʈ ����
	m_TextureRoute = texRoute;
}

void FBXParser::Release()
{
	pImporter->Destroy();
	SAFE_DELETE(pConverter);
	pScene->Destroy();
	pManager->Destroy();

	for (Model* model : m_ModelList)
	{
		for (Mesh* mesh : model->m_MeshList)
		{
			for (Face* face : mesh->m_MeshFace)
			{
				SAFE_DELETE(face);
			}
			for (Vertex* vertex : mesh->m_VertexList)
			{
				SAFE_DELETE(vertex);
			}
			for (IndexList* index : mesh->m_IndexList)
			{
				SAFE_DELETE(index);
			}
			mesh->m_MeshFace.clear();
			mesh->m_VertexList.clear();
			mesh->m_IndexList.clear();
		}
	}
}

ParserData::Model* FBXParser::LoadModel(std::string fileName, bool scaling, bool onlyAni)
{
	// Scene ����..
	SceneSetting(fileName, scaling, onlyAni);

	// Scene���� RootNode ��������..
	fbxsdk::FbxNode* pRootNode = pScene->GetRootNode();

	// Ȥ�ö� RootNode�� ���� ��츦 ���..
	if (pRootNode == nullptr) return nullptr;

	/// Material Data Loading..
	LoadMaterial();

	/// Bone Data Loading..
	LoadNode(pRootNode, FbxNodeAttribute::eSkeleton);

	/// Mesh Data Loading..
	LoadNode(pRootNode, FbxNodeAttribute::eMesh);

	/// Mesh Data Optimizing..
	OptimizeData();

	// �ʿ���� ������ ����..
	ResetData();

	return m_Model;
}

void FBXParser::SceneSetting(std::string fileName, bool scaling, bool onlyAni)
{
	// Model ����..
	CreateModel();

	// ���� �̸��� �ɼ� ����..
	fbxFileName = fileName;
	m_OnlyAni = onlyAni;

	if (!pImporter->Initialize(fbxFileName.c_str(), -1, pManager->GetIOSettings()))
		throw std::exception("error: initialize importer\n");

	// fbx ���� ������ Scene���� �����´�..
	pImporter->Import(pScene);

	// Scene ���� �ִ� �����͵��� ������ �������ش�..
	if (scaling)
	{
		FbxSystemUnit lFbxFileSystemUnit = pScene->GetGlobalSettings().GetSystemUnit();
		FbxSystemUnit lFbxOriginSystemUnit = pScene->GetGlobalSettings().GetOriginalSystemUnit();
		double factor = lFbxOriginSystemUnit.GetScaleFactor();
		const FbxSystemUnit::ConversionOptions lConversionOptions =
		{
		  true,
		  true,
		  true,
		  true,
		  true,
		  true
		};
		lFbxFileSystemUnit.m.ConvertScene(pScene, lConversionOptions);
		lFbxOriginSystemUnit.m.ConvertScene(pScene, lConversionOptions);
	}

	// Scene ������ �ﰢ��ȭ �� �� �ִ� ��� ��带 �ﰢ��ȭ ��Ų��..
	// 3D Max �ȿ��� Editable poly ���¶�� �� �۾��� ���ؾ� �Ѵ�..
	pConverter->Triangulate(pScene, true);
}

void FBXParser::CreateModel()
{
	m_Model = new Model();
	m_ModelList.push_back(m_Model);
}

void FBXParser::ResetData()
{
	pMesh = nullptr;
	fbxMaterials.clear();

	m_MaterialData = nullptr;
	m_OneAnimation = nullptr;

	m_AllBoneList.clear();
}

void FBXParser::LoadMaterial()
{
	// �ִϸ��̼Ǹ� �������..
	if (m_OnlyAni) return;

	// Scene�� �����ϴ� Material ������ŭ ����..
	int mcount = pScene->GetMaterialCount();
	for (int matIndex = 0; matIndex < pScene->GetMaterialCount(); matIndex++)
	{
		FbxSurfaceMaterial* material = pScene->GetMaterial(matIndex);
		m_MaterialData = new CMaterial();
		m_MaterialData->m_MaterialNumber = matIndex;

		// Material Data ����..
		SetMaterial(material);

		// Texture Data ����..
		SetTexture(material, FbxSurfaceMaterial::sDiffuse);
		SetTexture(material, FbxSurfaceMaterial::sNormalMap);
		//SetTexture(material, FbxSurfaceMaterial::sAmbient);
		//SetTexture(material, FbxSurfaceMaterial::sEmissive);
		//SetTexture(material, FbxSurfaceMaterial::sSpecular);

		m_Model->m_MaterialList.push_back(m_MaterialData);
		fbxMaterials.push_back(material);

		m_MaterialData = nullptr;
	}
}

void FBXParser::LoadNode(fbxsdk::FbxNode* node, fbxsdk::FbxNodeAttribute::EType attribute)
{
	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
	if (nodeAttribute != nullptr)
	{
		FbxString nodename = node->GetName();
		FbxNodeAttribute::EType attributeType = nodeAttribute->GetAttributeType();
		if (attributeType == attribute)
		{
			switch (nodeAttribute->GetAttributeType())
			{
			case FbxNodeAttribute::eSkeleton:
				ProcessSkeleton(node);
				LoadAnimation(node);
				break;
			case FbxNodeAttribute::eMesh:
				ProcessMesh(node);
				LoadAnimation(node);
				break;
			case FbxNodeAttribute::eMarker:
			case FbxNodeAttribute::eNurbs:
			case FbxNodeAttribute::ePatch:
			case FbxNodeAttribute::eCamera:
			case FbxNodeAttribute::eLight:
			case FbxNodeAttribute::eNull:
				break;
			default:
				break;
			}
		}
	}

	// ��� ����..
	for (int i = 0; i < node->GetChildCount(); ++i)
	{
		LoadNode(node->GetChild(i), attribute);
	}
}

void FBXParser::LoadAnimation(fbxsdk::FbxNode* node)
{
	float frameRate = (float)FbxTime::GetFrameRate(pScene->GetGlobalSettings().GetTimeMode());

	FbxTakeInfo* takeInfo = pImporter->GetTakeInfo(0);

	if (takeInfo == nullptr) return;

	FbxTime start = FbxTime(FbxLongLong(0x7fffffffffffffff));
	FbxTime stop = FbxTime(FbxLongLong(-0x7fffffffffffffff));

	FbxTimeSpan span = takeInfo->mLocalTimeSpan;

	double tempStart = span.GetStart().GetSecondDouble();
	double tempStop = span.GetStop().GetSecondDouble();

	if (tempStart < tempStop)
	{
		// �����ð� ���� �� �� �������� ��������� keyFrames�� �����
		int keyFrames = (int)((tempStop - tempStart) * (double)frameRate);
		double ticksperFrame = (tempStop - tempStart) / keyFrames;

		// ���ο� Animaiton Data ����..
		m_OneAnimation = new OneAnimation();

		// �� ������ ��� �ð�..
		m_OneAnimation->m_TicksPerFrame = (float)ticksperFrame;

		// �ִϸ��̼� ���� ������..
		m_OneAnimation->m_StartFrame = (int)(tempStart)*keyFrames;
		m_OneAnimation->m_EndFrame = keyFrames - 1;

		// �ִϸ��̼� �� ������..
		m_OneAnimation->m_TotalFrame = keyFrames;

		// �ִϸ��̼� ������ �������..
		m_Model->m_isAnimation = true;

		ProcessAnimation(node);
	}
}

void FBXParser::ProcessSkeleton(fbxsdk::FbxNode* node)
{
	pMesh = node->GetMesh();

	// ���ο� Mesh ����..
	CreateMesh();

	m_OneMesh->m_NodeName = node->GetName();
	m_OneMesh->m_IsBone = true;

	// �ִϸ��̼Ǹ� ���� ���..
	if (m_OnlyAni) return;

	// �� Node Parent ã��..
	const char* parentName = node->GetParent()->GetName();
	Mesh* parentMesh = FindMesh(parentName);
	m_OneMesh->m_ParentName = parentName;

	if (parentMesh == nullptr)
		m_OneMesh->m_TopNode = true;

	// Node TRS ����..
	SetTransform(node);

	FbxSkeleton* fbxSkeleton = node->GetSkeleton();

	if (fbxSkeleton == nullptr) return;

	// �θ� Bone Index �ʱⰪ..
	// �θ� Bone Index�� -1 �̸� �ֻ��� Root Node..
	int parentBoneIndex = -1;

	// �ֻ��� ���(�θ���)���� Ȯ��..
	FbxNode* parentNode = node->GetParent();

	if (m_OneMesh->m_TopNode)
	{
		std::string nodeName = parentNode->GetName();
		parentBoneIndex = FindBoneIndex(nodeName);
	}

	// ���ο� Bone ����..
	Bone newBone;
	const char* boneName = node->GetName();
	newBone.m_BoneName = boneName;
	newBone.m_parent_bone_number = parentBoneIndex;
	newBone.m_BoneNumber = m_AllBoneList.size();
	m_AllBoneList.push_back(BonePair(boneName, newBone));
}

void FBXParser::ProcessMesh(fbxsdk::FbxNode* node)
{
	pMesh = node->GetMesh();

	// ���ο� Mesh ����..
	CreateMesh();

	m_OneMesh->m_NodeName = node->GetName();

	// �ִϸ��̼Ǹ� ���� ���..
	if (m_OnlyAni) return;

	// �� Node Parent ã��..
	const char* parentName = node->GetParent()->GetName();
	Mesh* parentMesh = FindMesh(parentName);
	m_OneMesh->m_ParentName = parentName;

	// �θ��� Mesh�� �����Ѵٸ� ChildList�� �߰�..
	if (parentMesh == nullptr)
		m_OneMesh->m_TopNode = true;

	// Node TRS ����..
	SetTransform(node);

	int vertexTotalCount = pMesh->GetControlPointsCount();
	int polygonTotalCount = pMesh->GetPolygonCount();

	// ���� Vertex ������ ������� ó������ �ʱ� ����..
	if (vertexTotalCount < 1) return;

	// Vertex ������ŭ BoneWeight ����..
	// �ش� Vertex Index�� ������ Index�� ����ġ ��, Bone Index Data ����..
	std::vector<BoneWeights> boneWeights(vertexTotalCount);
	bool isSkin = ProcessBoneWeights(node, boneWeights);

	// Bone Data ���� ����� ���� Skinning Object �Ǻ�..
	m_OneMesh->m_IsSkinningObject = isSkin;

	// ���ο� ���ؽ� ����..
	CreateVertex(pMesh, boneWeights, vertexTotalCount);

	int vertexCount = 0; // ������ ����
	for (int pi = 0; pi < polygonTotalCount; pi++)
	{
		// Polygon ������ŭ Face ����..
		m_OneMesh->m_MeshFace.push_back(new Face);

		for (int vi = 0; vi < 3; vi++)
		{
			int vertexIndex = pMesh->GetPolygonVertex(pi, vi);
			int uvIndex = pMesh->GetTextureUVIndex(pi, vi);

			if (vertexIndex < 0 || vertexIndex >= vertexTotalCount)	continue;

			DirectX::SimpleMath::Vector3 fbxNormal;
			DirectX::SimpleMath::Vector2 fbxUV;

			fbxNormal = GetNormal(pMesh, vertexIndex, vertexCount);
			fbxUV = GetUV(pMesh, vertexIndex, uvIndex);

			// �ϴ� �ϳ��� Mesh �� �ϳ��� Material�� �����ϰ� ����..
			if (m_OneMesh->m_MaterialData == nullptr)
				LinkMaterialByPolygon(pMesh, pi, vertexIndex);

			// Face Vertex Index Data
			m_OneMesh->m_MeshFace[pi]->m_VertexIndex[vi] = vertexIndex;
			// Face Vertex Normal Data
			m_OneMesh->m_MeshFace[pi]->m_NormalVertex[vi] = fbxNormal;
			// Face Vertex UV Data
			m_OneMesh->m_MeshFace[pi]->m_UVvertex[vi] = fbxUV;

			vertexCount++;
		}
	}
}


bool FBXParser::ProcessBoneWeights(fbxsdk::FbxNode* node, std::vector<BoneWeights>& meshBoneWeights)
{
	FbxMesh* mesh = node->GetMesh();

	int deformerCount = mesh->GetDeformerCount();

	// DeformerCount�� 1���� ������ Bone Data�� ���ٰ� ����..
	if (deformerCount < 1) return false;

	for (int i = 0; i < deformerCount; ++i)
	{
		FbxDeformer* deformer = mesh->GetDeformer(i);

		if (deformer == nullptr) continue;

		if (deformer->GetDeformerType() == FbxDeformer::eSkin)
		{
			FbxSkin* skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));

			if (skin == nullptr) continue;

			FbxCluster::ELinkMode linkMode = FbxCluster::ELinkMode::eNormalize;

			int clusterCount = skin->GetClusterCount();

			// Skin Mesh üũ..
			Mesh* skinMesh = FindMesh(node->GetName());
			std::vector<BoneWeights> skinBoneWeights(meshBoneWeights.size());
			for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++)
			{
				FbxCluster* cluster = skin->GetCluster(clusterIndex);
				if (cluster == nullptr) continue;

				linkMode = cluster->GetLinkMode();
				FbxNode* pLinkNode = cluster->GetLink();

				if (pLinkNode == nullptr) continue;

				std::string lineNodeName = pLinkNode->GetName();

				// Bone Mesh üũ..
				Mesh* boneMesh = FindMesh(lineNodeName);

				if (boneMesh == nullptr) continue;
				if (boneMesh->m_IsBone == false) continue;

				FbxAMatrix matClusterTransformMatrix;
				FbxAMatrix matClusterLinkTransformMatrix;

				const FbxVector4 lT = node->GetGeometricTranslation(FbxNode::eDestinationPivot);
				const FbxVector4 lR = node->GetGeometricRotation(FbxNode::eDestinationPivot);
				const FbxVector4 lS = node->GetGeometricScaling(FbxNode::eDestinationPivot);

				FbxAMatrix geometryTransform = FbxAMatrix(lT, lR, lS);

				cluster->GetTransformMatrix(matClusterTransformMatrix);
				cluster->GetTransformLinkMatrix(matClusterLinkTransformMatrix);

				// Bone Matrix ����..
				DirectX::SimpleMath::Matrix clusterMatrix = ConvertMatrix(matClusterTransformMatrix);
				DirectX::SimpleMath::Matrix clusterlinkMatrix = ConvertMatrix(matClusterLinkTransformMatrix);
				DirectX::SimpleMath::Matrix geometryMatrix = ConvertMatrix(geometryTransform);

				DirectX::SimpleMath::Matrix offsetMatrix = clusterMatrix * clusterlinkMatrix.Invert() * geometryMatrix;

				skinMesh->m_BoneTMList.emplace_back(offsetMatrix);
				skinMesh->m_BoneMeshList.emplace_back(boneMesh);

				int c = cluster->GetControlPointIndicesCount();
				for (int j = 0; j < cluster->GetControlPointIndicesCount(); ++j)
				{
					int m_Index = cluster->GetControlPointIndices()[j];
					double weight = cluster->GetControlPointWeights()[j];

					if (weight == 0) continue;

					skinBoneWeights[m_Index].AddBoneWeight(clusterIndex, (float)weight);
				}
			}

			switch (linkMode)
			{
			case FbxCluster::eNormalize:
			{
				// ����ġ ���� 1.0���� ����� �۾�..
				for (int i = 0; i < (int)skinBoneWeights.size(); ++i)
					skinBoneWeights[i].Normalize();
			}
			break;

			case FbxCluster::eAdditive:
				break;

			case FbxCluster::eTotalOne:
				break;
			}

			for (size_t i = 0; i < meshBoneWeights.size(); i++)
				meshBoneWeights[i].AddBoneWeights(skinBoneWeights[i]);
		}
	}

	return true;
}

void FBXParser::ProcessAnimation(fbxsdk::FbxNode* node)
{
	// �ִϸ��̼Ǹ� �������..
	if (m_OnlyAni)
	{
		FbxMesh* mesh = node->GetMesh();

		if (mesh != nullptr)
		{
			int deformerCount = mesh->GetDeformerCount();

			// DeformerCount�� 0���� ũ�� Skinning Mesh..
			if (deformerCount > 0)
				return;
		}
	}

	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

	// ���� ��Ű�� ������Ʈ��� �ִϸ��̼� �����ʹ� Bone�� ����Ǿ� �����Ƿ�..
	if (m_OneMesh->m_IsSkinningObject) return;

	if (nodeAttribute != nullptr)
	{
		std::string nodeName = node->GetName();
		Mesh* mesh = FindMesh(nodeName);

		if (mesh != nullptr)
		{
			FbxTime::EMode timeMode = pScene->GetGlobalSettings().GetTimeMode();
			for (FbxLongLong m_Index = 0; m_Index < m_OneAnimation->m_TotalFrame; m_Index++)
			{
				FbxTime takeTime;
				takeTime.SetFrame(m_OneAnimation->m_StartFrame + m_Index, timeMode);

				// Local Transform = �θ� Bone�� Global Transform�� Inverse Transform * �ڽ� Bone�� Global Transform
				FbxAMatrix nodeTransform = node->EvaluateLocalTransform(takeTime);

				DirectX::SimpleMath::Matrix nodeTRS = ConvertMatrix(nodeTransform);

				XMVECTOR scale;
				XMVECTOR rot;
				XMVECTOR pos;

				XMMatrixDecompose(&scale, &rot, &pos, nodeTRS);

				OneFrame* newAni = new OneFrame;

				newAni->m_Time = (float)m_Index;
				newAni->m_Pos = DirectX::SimpleMath::Vector3(pos);
				newAni->m_RotQt = Quaternion(rot);
				newAni->m_Scale = DirectX::SimpleMath::Vector3(scale);

				m_OneAnimation->m_AniData.push_back(newAni);
			}

			// �ش� Mesh�� �ִϸ��̼� ����..
			mesh->m_Animation = m_OneAnimation;
		}
	}
}

void FBXParser::OptimizeData()
{
	// �ִϸ��̼Ǹ� �������..
	if (m_OnlyAni) return;

	// Optimize Data
	for (unsigned int i = 0; i < m_Model->m_MeshList.size(); i++)
	{
		OptimizeVertex(m_Model->m_MeshList[i]);
	}
}

void FBXParser::OptimizeVertex(ParserData::Mesh* pMesh)
{
	if (pMesh->m_VertexList.empty()) return;

	bool new_VertexSet = false;
	unsigned int resize_VertexIndex = pMesh->m_VertexList.size();

	// ���� Face���� �����ϴ� 3���� Vertex ��..
	for (unsigned int i = 0; i < pMesh->m_MeshFace.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			unsigned int vertexIndex = pMesh->m_MeshFace[i]->m_VertexIndex[j];

			Vertex* nowVertex = pMesh->m_VertexList[vertexIndex];

			// �ؽ�ó�� �ְ�, �������� �ʾ����� �ؽ�ó u,v ����..
			if (nowVertex->m_IsTextureSet == false)
			{
				nowVertex->m_U = pMesh->m_MeshFace[i]->m_UVvertex[j].x;
				nowVertex->m_V = pMesh->m_MeshFace[i]->m_UVvertex[j].y;
				nowVertex->m_IsTextureSet = true;
			}

			// ���� �ε��� �븻�� �˻�ÿ� �־��ְ� ����..
			if (nowVertex->m_IsNormalSet == false)
			{
				nowVertex->m_Normal = pMesh->m_MeshFace[i]->m_NormalVertex[j];
				nowVertex->m_IsNormalSet = true;
			}

			// Normal, U, V ���� �Ѱ��� �ٸ��� Vertex ���� ����..
			if ((pMesh->m_VertexList[vertexIndex]->m_Normal != pMesh->m_MeshFace[i]->m_NormalVertex[j]))
			{
				new_VertexSet = true;
			}

			if ((pMesh->m_VertexList[vertexIndex]->m_U != pMesh->m_MeshFace[i]->m_UVvertex[j].x) ||
				(pMesh->m_VertexList[vertexIndex]->m_V != pMesh->m_MeshFace[i]->m_UVvertex[j].y))
			{
				new_VertexSet = true;
			}

			if (new_VertexSet)
			{
				// �߰��� Vertex�� �ִٸ� üũ..
				if (resize_VertexIndex > (int)pMesh->m_VertexList.size())
				{
					for (unsigned int k = pMesh->m_VertexList.size(); k < resize_VertexIndex; k++)
					{
						// ���� �߰��� Vertex�� ������ �����͸� �����ִ� Face ���� Vertex Index ����..
						if ((pMesh->m_VertexList[k]->m_Indices == pMesh->m_MeshFace[i]->m_VertexIndex[j]) &&
							(pMesh->m_VertexList[k]->m_Normal == pMesh->m_MeshFace[i]->m_NormalVertex[j]))
						{
							if ((pMesh->m_VertexList[k]->m_U == pMesh->m_MeshFace[i]->m_UVvertex[j].x) &&
								(pMesh->m_VertexList[k]->m_V == pMesh->m_MeshFace[i]->m_UVvertex[j].y))
							{
								pMesh->m_MeshFace[i]->m_VertexIndex[j] = (int)k;
								new_VertexSet = false;
								break;
							}
						}
					}
				}
			}

			// ���� �߰��ؾ��� Vertex..
			if (new_VertexSet == true)
			{
				Vertex* newVertex = new Vertex;
				newVertex->m_Pos = nowVertex->m_Pos;
				newVertex->m_Indices = nowVertex->m_Indices;
				newVertex->m_Normal = pMesh->m_MeshFace[i]->m_NormalVertex[j];
				newVertex->m_BoneIndices = nowVertex->m_BoneIndices;
				newVertex->m_BoneWeights = nowVertex->m_BoneWeights;
				newVertex->m_IsNormalSet = true;

				newVertex->m_U = pMesh->m_MeshFace[i]->m_UVvertex[j].x;
				newVertex->m_V = pMesh->m_MeshFace[i]->m_UVvertex[j].y;
				newVertex->m_IsTextureSet = true;

				pMesh->m_VertexList.push_back(newVertex);
				pMesh->m_MeshFace[i]->m_VertexIndex[j] = resize_VertexIndex;
				resize_VertexIndex++;
				new_VertexSet = false;
			}
		}
	}

	// Tanget �� ����..
	for (unsigned int i = 0; i < pMesh->m_MeshFace.size(); i++)
	{
		int index0 = pMesh->m_MeshFace[i]->m_VertexIndex[0];
		int index1 = pMesh->m_MeshFace[i]->m_VertexIndex[1];
		int index2 = pMesh->m_MeshFace[i]->m_VertexIndex[2];

		DirectX::SimpleMath::Vector3 ep1 = pMesh->m_VertexList[index1]->m_Pos - pMesh->m_VertexList[index0]->m_Pos;
		DirectX::SimpleMath::Vector3 ep2 = pMesh->m_VertexList[index2]->m_Pos - pMesh->m_VertexList[index0]->m_Pos;

		DirectX::SimpleMath::Vector2 uv1 = { pMesh->m_VertexList[index1]->m_U - pMesh->m_VertexList[index0]->m_U,
						  pMesh->m_VertexList[index1]->m_V - pMesh->m_VertexList[index0]->m_V };
		DirectX::SimpleMath::Vector2 uv2 = { pMesh->m_VertexList[index2]->m_U - pMesh->m_VertexList[index0]->m_U,
						  pMesh->m_VertexList[index2]->m_V - pMesh->m_VertexList[index0]->m_V };

		float den = 1.0f / (uv1.x * uv2.y - uv2.x * uv1.y);

		// ���� �ȼ� ���̴����� ������ ���� T, B, N�� ��µ�
		// �ȼ� ���̴� ���� ������ ���ؽ� ���̴��� ���꿡 ���� ȣ�� Ƚ���� ������ �ٸ��� ũ�Ƿ� ���ϰ� �� �� �ִ�..
		// �������� �ȼ��� ���� �ȼ� ���̴� ���� �ƴϸ� ������ ���� ������ �ȼ� ���̴����� ������ �Ѵٰ� �Ѵ�..
		/// ���� ��������� ���ؽ� ���̴��� �Űܵ�
		DirectX::SimpleMath::Vector3 tangent = (ep1 * uv2.y - ep2 * uv1.y) * den;
		tangent.Normalize();

		// ���� ������ ���� �����Ͽ� ���̴����� ��հ��� ����ϵ��� ����..
		pMesh->m_VertexList[index0]->m_Tanget += tangent;
		pMesh->m_VertexList[index1]->m_Tanget += tangent;
		pMesh->m_VertexList[index2]->m_Tanget += tangent;
	}

	// �ε����� �׳� ����
	for (unsigned int i = 0; i < pMesh->m_MeshFace.size(); i++)
	{
		pMesh->m_IndexList.push_back(new IndexList);

		for (unsigned int j = 0; j < 3; j++)
		{
			switch (j)
			{
			case 0:
				pMesh->m_IndexList[i]->m_Index[j] = pMesh->m_MeshFace[i]->m_VertexIndex[0];
				break;
			case 1:
				pMesh->m_IndexList[i]->m_Index[j] = pMesh->m_MeshFace[i]->m_VertexIndex[2];
				break;
			case 2:
				pMesh->m_IndexList[i]->m_Index[j] = pMesh->m_MeshFace[i]->m_VertexIndex[1];
				break;
			default:
				break;
			}
		}
	}

	pMesh->m_MeshFace.clear();
}

void FBXParser::RecombinationTM(ParserData::Mesh* pMesh)
{
	/// Negative Scale Check
	// 3D Max�� �۾��� Mirroring���� ���� Scale���� ������ ���� ��츦 Negative Scale �̶�� �Ѵ�
	// ó���� ������ ��� Normal ó���� Animation ó���ÿ� ������ �ȴ�
	// Rotaion�� ��Ÿ���� �� Row1 ~ Row3 �� ������ ���� �� ���� �ٸ� �� ���� �����Ͽ� �����̸� ���� �ݴ��̰� ����̸� ���� ��ġ�Ѵ�
	//
	XMVECTOR crossVec = XMVector3Cross(pMesh->m_WorldTM.Right(), pMesh->m_WorldTM.Up());
	XMVECTOR dotVec = XMVector3Dot(crossVec, pMesh->m_WorldTM.Backward());

	XMFLOAT3 resultVec;

	XMStoreFloat3(&resultVec, dotVec);

	// �� ������ �ٸ� ��츸 ó��������..
	if (resultVec.x < 0 || resultVec.y < 0 || resultVec.z < 0)
	{
		XMVECTOR nodeScale;
		XMVECTOR nodetRot;
		XMVECTOR nodePos;

		// ���� ��Ʈ�������� �и�..
		XMMatrixDecompose(&nodeScale, &nodetRot, &nodePos, pMesh->m_WorldTM);

		// ������ ������..
		nodeScale *= -1;

		// �븻�� ������..
		for (auto& k : pMesh->m_VertexList)
		{
			k->m_Normal *= -1;
		}

		/// �ؽ��ĳ� ���� �����͵� ��ȯ�� ���־�� �ұ�..?

		// ��Ʈ���� ������..
		pMesh->m_WorldTM = XMMatrixScalingFromVector(nodeScale) * XMMatrixRotationQuaternion(nodetRot) * XMMatrixTranslationFromVector(nodePos);
	}
}

DirectX::SimpleMath::Vector3 FBXParser::GetPos(fbxsdk::FbxMesh* mesh, int vertexIndex)
{
	FbxVector4 fbxPos;

	fbxPos = mesh->GetControlPointAt(vertexIndex);

	return ConvertVector3(fbxPos);
}

DirectX::SimpleMath::Vector3 FBXParser::GetNormal(fbxsdk::FbxMesh* mesh, int vertexIndex, int vertexCount)
{
	if (mesh->GetElementNormalCount() < 1) return DirectX::SimpleMath::Vector3();

	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal();

	FbxVector4 fbxNormal;

	switch (vertexNormal->GetMappingMode()) // ���� ��� 
	{
	case FbxGeometryElement::eByControlPoint: // control point mapping 
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			fbxNormal = vertexNormal->GetDirectArray().GetAt(vertexIndex);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int m_Index = vertexNormal->GetIndexArray().GetAt(vertexIndex); // �ε����� ���´�. 
			fbxNormal = vertexNormal->GetDirectArray().GetAt(m_Index);
		}
		break;
		}
	}
	break;

	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			fbxNormal = vertexNormal->GetDirectArray().GetAt(vertexCount);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int m_Index = vertexNormal->GetIndexArray().GetAt(vertexCount); // �ε����� ���´�. 
			fbxNormal = vertexNormal->GetDirectArray().GetAt(m_Index);
		}
		break;
		}
	}
	break;
	}

	return ConvertVector3(fbxNormal);
}


DirectX::SimpleMath::Vector2 FBXParser::GetUV(fbxsdk::FbxMesh* mesh, int vertexIndex, int uvIndex)
{
	if (mesh->GetElementUVCount() < 1) return DirectX::SimpleMath::Vector2();

	FbxGeometryElementUV* vertexUV = mesh->GetLayer(0)->GetUVs();

	FbxVector4 fbxUV;

	switch (vertexUV->GetMappingMode()) // ���� ��� 
	{
	case FbxGeometryElement::eByControlPoint: // control point mapping 
	{
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			fbxUV = vertexUV->GetDirectArray().GetAt(vertexIndex);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int m_Index = vertexUV->GetIndexArray().GetAt(vertexIndex); // �ε����� ���´�. 
			fbxUV = vertexUV->GetDirectArray().GetAt(m_Index);
		}
		break;
		}
	}
	break;

	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			fbxUV = vertexUV->GetDirectArray().GetAt(uvIndex);
		}
		break;
		}
	}
	break;
	}

	return ConvertVector2(fbxUV);
}

void FBXParser::LinkMaterialByPolygon(fbxsdk::FbxMesh* mesh, int polygonIndex, int vertexIndex)
{
	FbxNode* node = mesh->GetNode();
	if (node == nullptr) return;

	FbxLayerElementMaterial* fbxMaterial = mesh->GetLayer(0)->GetMaterials();
	if (fbxMaterial == nullptr) return;

	int mappingIndex = 0;
	switch (fbxMaterial->GetMappingMode())
	{
	case FbxLayerElement::eAllSame:
		mappingIndex = 0;
		break;
	case FbxLayerElement::eByControlPoint:
		mappingIndex = vertexIndex;
		break;
	case FbxLayerElement::eByPolygonVertex:
		mappingIndex = polygonIndex * 3;
		break;
	case FbxLayerElement::eByPolygon:
		mappingIndex = polygonIndex;
		break;
	}

	if (mappingIndex < 0) return;

	FbxLayerElement::EReferenceMode refMode = fbxMaterial->GetReferenceMode();
	if (refMode == FbxLayerElement::eDirect)
	{
		if (mappingIndex < node->GetMaterialCount())
		{
			// materialIndex : �츮�� �ο��� �ε���
			int materialIndex = GetMaterialIndex(node->GetMaterial(mappingIndex));
			m_OneMesh->m_MaterialData = m_Model->m_MaterialList[materialIndex];
		}
	}
	else if (refMode == FbxLayerElement::eIndexToDirect)
	{
		FbxLayerElementArrayTemplate<int>& indexArr = fbxMaterial->GetIndexArray();

		if (mappingIndex < indexArr.GetCount())
		{
			int tempIndex = indexArr.GetAt(mappingIndex);

			if (tempIndex < node->GetMaterialCount())
			{
				// materialIndex : �츮�� �ο��� �ε���
				int materialIndex = GetMaterialIndex(node->GetMaterial(tempIndex));
				m_OneMesh->m_MaterialData = m_Model->m_MaterialList[materialIndex];
			}
		}
	}
}

void FBXParser::SetTransform(fbxsdk::FbxNode* node)
{
	if (node == nullptr) return;

	FbxMatrix worldpos = pScene->GetAnimationEvaluator()->GetNodeGlobalTransform(node);
	FbxMatrix localpos = pScene->GetAnimationEvaluator()->GetNodeLocalTransform(node);

	DirectX::SimpleMath::Matrix world = ConvertMatrix(worldpos);
	DirectX::SimpleMath::Matrix local = ConvertMatrix(localpos);

	if (m_OneMesh->m_TopNode)
	{
		const auto yaw = -90.0f * DirectX::XM_PI / 180.0f;

		DirectX::SimpleMath::Quaternion q = XMQuaternionRotationRollPitchYaw(yaw, 0.0f, 0.0f);

		world *= XMMatrixRotationQuaternion(q);
		local *= XMMatrixRotationQuaternion(q);
	}

	m_OneMesh->m_WorldTM = world;
	m_OneMesh->m_LocalTM = local;
}

DirectX::SimpleMath::Matrix FBXParser::GetGlobalAnimationTransform(fbxsdk::FbxNode* node, fbxsdk::FbxTime time)
{
	if (node == nullptr) return DirectX::SimpleMath::Matrix();

	FbxAMatrix matrix = node->EvaluateGlobalTransform(time);
	return ConvertMatrix(matrix);
}

int FBXParser::GetMaterialIndex(fbxsdk::FbxSurfaceMaterial* material)
{
	for (unsigned int m_Index = 0; m_Index < fbxMaterials.size(); m_Index++)
	{
		if (fbxMaterials[m_Index] == material)
			return m_Index;
	}

	return -1;
}

void FBXParser::SetMaterial(fbxsdk::FbxSurfaceMaterial* material)
{
	m_MaterialData->m_MaterialName = material->GetName();

	if (material->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		// Ambient Data
		m_MaterialData->m_Material_Ambient.x = static_cast<float>(((FbxSurfacePhong*)material)->Ambient.Get()[0]) * 10.0f;
		m_MaterialData->m_Material_Ambient.y = static_cast<float>(((FbxSurfacePhong*)material)->Ambient.Get()[1]) * 10.0f;
		m_MaterialData->m_Material_Ambient.z = static_cast<float>(((FbxSurfacePhong*)material)->Ambient.Get()[2]) * 10.0f;

		// Diffuse Data
		m_MaterialData->m_Material_Diffuse.x = static_cast<float>(((FbxSurfacePhong*)material)->Diffuse.Get()[0]);
		m_MaterialData->m_Material_Diffuse.y = static_cast<float>(((FbxSurfacePhong*)material)->Diffuse.Get()[1]);
		m_MaterialData->m_Material_Diffuse.z = static_cast<float>(((FbxSurfacePhong*)material)->Diffuse.Get()[2]);

		// Specular Data
		m_MaterialData->m_Material_Specular.x = static_cast<float>(((FbxSurfacePhong*)material)->Specular.Get()[0]);
		m_MaterialData->m_Material_Specular.y = static_cast<float>(((FbxSurfacePhong*)material)->Specular.Get()[1]);
		m_MaterialData->m_Material_Specular.z = static_cast<float>(((FbxSurfacePhong*)material)->Specular.Get()[2]);

		// Emissive Data
		m_MaterialData->m_Material_Emissive.x = static_cast<float>(((FbxSurfacePhong*)material)->Emissive.Get()[0]);
		m_MaterialData->m_Material_Emissive.y = static_cast<float>(((FbxSurfacePhong*)material)->Emissive.Get()[1]);
		m_MaterialData->m_Material_Emissive.z = static_cast<float>(((FbxSurfacePhong*)material)->Emissive.Get()[2]);

		// Transparecy Data
		m_MaterialData->m_Material_Transparency = static_cast<float>(((FbxSurfacePhong*)material)->TransparencyFactor.Get());

		// Shininess Data
		m_MaterialData->m_Material_Shininess = static_cast<float>(((FbxSurfacePhong*)material)->Shininess.Get());

		// Reflectivity Data
		m_MaterialData->m_Material_Reflectivity = static_cast<float>(((FbxSurfacePhong*)material)->ReflectionFactor.Get());
	}
	else if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		// Ambient Data
		m_MaterialData->m_Material_Ambient.x = static_cast<float>(((FbxSurfaceLambert*)material)->Ambient.Get()[0]);
		m_MaterialData->m_Material_Ambient.y = static_cast<float>(((FbxSurfaceLambert*)material)->Ambient.Get()[1]);
		m_MaterialData->m_Material_Ambient.z = static_cast<float>(((FbxSurfaceLambert*)material)->Ambient.Get()[2]);

		// Diffuse Data
		m_MaterialData->m_Material_Diffuse.x = static_cast<float>(((FbxSurfaceLambert*)material)->Diffuse.Get()[0]);
		m_MaterialData->m_Material_Diffuse.y = static_cast<float>(((FbxSurfaceLambert*)material)->Diffuse.Get()[1]);
		m_MaterialData->m_Material_Diffuse.z = static_cast<float>(((FbxSurfaceLambert*)material)->Diffuse.Get()[2]);

		// Emissive Data
		m_MaterialData->m_Material_Emissive.x = static_cast<float>(((FbxSurfaceLambert*)material)->Emissive.Get()[0]);
		m_MaterialData->m_Material_Emissive.y = static_cast<float>(((FbxSurfaceLambert*)material)->Emissive.Get()[1]);
		m_MaterialData->m_Material_Emissive.z = static_cast<float>(((FbxSurfaceLambert*)material)->Emissive.Get()[2]);

		// Transparecy Data
		m_MaterialData->m_Material_Transparency = static_cast<float>(((FbxSurfaceLambert*)material)->TransparencyFactor.Get());
	}
}

void FBXParser::SetTexture(fbxsdk::FbxSurfaceMaterial* material, const char* materialName)
{
	unsigned int textureIndex = 0;

	FbxProperty property;

	property = material->FindProperty(materialName);

	if (property.IsValid())
	{
		unsigned int textureCount = property.GetSrcObjectCount<FbxTexture>();
		for (unsigned int i = 0; i < textureCount; ++i)
		{
			FbxLayeredTexture* layeredTexture = property.GetSrcObject<FbxLayeredTexture>(i);
			if (layeredTexture)
			{
				throw std::exception("Layered Texture is currently unsupported\n");
			}
			else
			{
				FbxTexture* texture = property.GetSrcObject<FbxTexture>(i);
				if (texture)
				{
					std::string textureType = property.GetNameAsCStr();
					FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);

					if (fileTexture)
					{
						// ��ȯ�� ���� ���..
						std::string fileRoute = ConvertFileRoute(fileTexture->GetFileName());
						std::string count = std::to_string(m_Model->m_MaterialList.size());
						if (textureType == "DiffuseColor")
						{
							std::string mapName("Diffuse Map" + count);
							m_MaterialData->m_DiffuseMap = new MaterialMap();
							m_MaterialData->m_IsDiffuseMap = true;
							m_MaterialData->m_DiffuseMap->m_MapName = mapName;
							m_MaterialData->m_DiffuseMap->m_BitMap = fileRoute;
							m_MaterialData->m_MapList.push_back(m_MaterialData->m_DiffuseMap);
						}
						else if (textureType == "SpecularColor")
						{
							std::string mapName("Specular Map" + count);
							m_MaterialData->m_SpecularMap = new MaterialMap();
							m_MaterialData->m_IsSpecularMap = true;
							m_MaterialData->m_SpecularMap->m_MapName = mapName;
							m_MaterialData->m_SpecularMap->m_BitMap = fileRoute;
							m_MaterialData->m_MapList.push_back(m_MaterialData->m_SpecularMap);
						}
						else if (textureType == "NormalMap")
						{
							std::string mapName("Normal Map" + count);
							m_MaterialData->m_BumpMap = new MaterialMap();
							m_MaterialData->m_IsBumpMap = true;
							m_MaterialData->m_BumpMap->m_MapName = mapName;
							m_MaterialData->m_BumpMap->m_BitMap = fileRoute;
							m_MaterialData->m_MapList.push_back(m_MaterialData->m_BumpMap);
						}
						else if (textureType == "TransparentColor")	// MaskMap
						{
						}
					}
				}
			}
		}
	}
}

void FBXParser::CreateVertex(fbxsdk::FbxMesh* mesh, std::vector<BoneWeights>& boneWeights, int vertexCount)
{
	FbxVector4 fbxPos;

	// Vertex ������ŭ Vertex ����..
	for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++)
	{
		m_OneMesh->m_VertexList.push_back(new Vertex);

		fbxPos = mesh->GetControlPointAt(vertexIndex);

		DirectX::SimpleMath::Vector3 pos = ConvertVector3(fbxPos);

		// Bounding Box üũ�� Position
		DirectX::SimpleMath::Vector3 nowPos = NoConvertVector3(fbxPos);

		m_OneMesh->m_VertexList[vertexIndex]->m_Pos = pos;
		m_OneMesh->m_VertexList[vertexIndex]->m_Indices = vertexIndex;

		// Bone Weight Data
		for (unsigned int boneIndex = 0; boneIndex < boneWeights[vertexIndex].m_BoneWeights.size(); boneIndex++)
		{
			m_OneMesh->m_VertexList[vertexIndex]->m_BoneIndices.push_back(boneWeights[vertexIndex].m_BoneWeights[boneIndex]->m_BoneNumber);
			m_OneMesh->m_VertexList[vertexIndex]->m_BoneWeights.push_back(boneWeights[vertexIndex].m_BoneWeights[boneIndex]->m_BoneWeight);
		}
	}
}

void FBXParser::CreateMesh()
{
	m_OneMesh = nullptr;
	m_OneMesh = new ParserData::Mesh();
	m_Model->m_MeshList.push_back(m_OneMesh);
}

int FBXParser::FindBoneIndex(std::string boneName)
{
	for (BonePair bone : m_AllBoneList)
	{
		if (bone.first == boneName)
			return bone.second.m_BoneNumber;
	}

	return -1;
}

ParserData::Mesh* FBXParser::FindMesh(std::string meshName)
{
	for (Mesh* mesh : m_Model->m_MeshList)
	{
		if (mesh->m_NodeName == meshName)
			return mesh;
	}

	return nullptr;
}
