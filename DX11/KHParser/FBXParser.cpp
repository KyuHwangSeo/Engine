#define SAFE_DELETE(x) { if(x != nullptr) {delete x; x = nullptr;} }

#define FBXSDK_SHARED
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk")

using namespace fbxsdk;

#include <windows.h>
#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

#include "DLLDefine.h"
#include "KHMath.h"

#include "AnimationData.h"
#include "CParsingDataClass.h"
using namespace ParserData;

#include "BoneWeights.h"
#include "FBXParser.h"


FBXParser::FBXParser()
{
	Initalize();
}

FBXParser::~FBXParser()
{
	pManager->Destroy();
}

void FBXParser::Initalize()
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

void FBXParser::SceneSetting()
{

	if (!pImporter->Initialize(fbxFileName, -1, pManager->GetIOSettings()))
		throw std::exception("error: initialize importer\n");

	// fbx ���� ������ Scene���� �����´�..
	pImporter->Import(pScene);

	// Scene ���� �ִ� �����͵��� ������ �������ش�..
	if (m_Scaling)
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


void FBXParser::Release()
{
	pImporter->Destroy();
	SAFE_DELETE(pConverter);
	pScene->Destroy();
	pManager->Destroy();
}

void FBXParser::ResetModel()
{
	m_Model = new FBXModel();
	pMesh = nullptr;
	fbxMaterials.clear();
}

void FBXParser::LoadScene(fbxsdk::FbxString fileName, bool scaling, bool onlyAni)
{
	ResetModel();

	fbxFileName = fileName;
	m_Scaling = scaling;
	m_OnlyAni = onlyAni;

	// Scene ����..
	SceneSetting();

	// Scene���� RootNode ��������..
	fbxsdk::FbxNode* pRootNode = pScene->GetRootNode();

	// Ȥ�ö� RootNode�� ���� ��츦 ���..
	if (pRootNode == nullptr) return;

	/// Material Data Loading..
	LoadMaterial();

	/// Bone Data Loading..
	LoadNode(pRootNode, FbxNodeAttribute::eSkeleton);

	/// Mesh Data Loading..
	LoadNode(pRootNode, FbxNodeAttribute::eMesh);

	/// Mesh Data Optimizing..
	OptimizeData();
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
		m_materialdata = new CMaterial();
		m_materialdata->m_materialnumber = matIndex;

		// Material Data ����..
		SetMaterial(material);

		// Texture Data ����..
		SetTexture(material, FbxSurfaceMaterial::sDiffuse);
		SetTexture(material, FbxSurfaceMaterial::sNormalMap);
		//SetTexture(material, FbxSurfaceMaterial::sAmbient);
		//SetTexture(material, FbxSurfaceMaterial::sEmissive);
		//SetTexture(material, FbxSurfaceMaterial::sSpecular);

		m_Model->m_list_materialdata.push_back(m_materialdata);
		fbxMaterials.push_back(material);

		m_materialdata = nullptr;
		m_Model->m_materialcount++;
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

		m_Model->m_OneAnimation = new AnimationData();
		m_OneAnimation = m_Model->m_OneAnimation;

		// �� ������ ��� �ð�..
		m_OneAnimation->m_ticksperFrame = (float)ticksperFrame;

		// �ִϸ��̼� ���� ������..
		m_OneAnimation->m_startFrame = (int)(tempStart)*keyFrames;
		m_OneAnimation->m_endFrame = (int)(tempStop)*keyFrames;

		// �ִϸ��̼� �� ������..
		m_OneAnimation->m_totalFrame = keyFrames;

		ProcessAnimation(node);
	}
}

void FBXParser::ProcessSkeleton(fbxsdk::FbxNode* node)
{
	pMesh = node->GetMesh();

	// ���ο� Mesh ����..
	CreateMesh();

	m_OneMesh->m_nodename = node->GetName();
	m_OneMesh->m_IsBone = true;

	// �ִϸ��̼Ǹ� ���� ���..
	if (m_OnlyAni) return;

	// �� Node Parent ã��..
	std::string parentName = node->GetParent()->GetName();
	Mesh* parentMesh = FindMesh(parentName);
	m_OneMesh->m_nodeparent = parentName;
	m_OneMesh->m_parent = parentMesh;

	if (parentMesh != nullptr)
		parentMesh->m_childlist.push_back(m_OneMesh);

	// Node TRS ����..
	SetTransform(node);

	// ���ο� Bone ����..
	CreateBone();

	FbxSkeleton* fbxSkeleton = node->GetSkeleton();

	if (fbxSkeleton == nullptr) return;

	// �θ� Bone Index �ʱⰪ..
	// �θ� Bone Index�� -1 �̸� �ֻ��� Root Node..
	int parentBoneIndex = -1;

	// �ֻ��� ���(�θ���)���� Ȯ��..
	FbxNode* parentNode = node->GetParent();

	if (m_OneMesh->m_parent != nullptr)
	{
		string nodeName = parentNode->GetName();
		parentBoneIndex = FindBoneIndex(nodeName);
	}

	string boneName = node->GetName();
	m_OneBone->m_bone_name = boneName;
	m_OneBone->m_parent_bone_number = parentBoneIndex;
	m_OneBone->m_bone_number = m_Model->m_AllBoneList.size();
	m_OneBone->m_isCol = true;
	m_OneBone->m_Min = DXVector3(+KH_MATH::Infinity, +KH_MATH::Infinity, +KH_MATH::Infinity);
	m_OneBone->m_Max = DXVector3(-KH_MATH::Infinity, -KH_MATH::Infinity, -KH_MATH::Infinity);
	m_OneMesh->m_bone = m_OneBone;
	m_Model->m_AllBoneList.push_back(BonePair(boneName, m_OneBone));
}

void FBXParser::ProcessMesh(fbxsdk::FbxNode* node)
{
	pMesh = node->GetMesh();

	// ���ο� Mesh ����..
	CreateMesh();

	m_OneMesh->m_nodename = node->GetName();

	// �ִϸ��̼Ǹ� ���� ���..
	if (m_OnlyAni) return;

	// �� Node Parent ã��..
	string parentName = node->GetParent()->GetName();
	Mesh* parentMesh = FindMesh(parentName);
	m_OneMesh->m_nodeparent = parentName;
	m_OneMesh->m_parent = parentMesh;

	// �θ��� Mesh�� �����Ѵٸ� ChildList�� �߰�..
	if (parentMesh != nullptr)
		parentMesh->m_childlist.push_back(m_OneMesh);

	// Node TRS ����..
	SetTransform(node);

	int vertexTotalCount = pMesh->GetControlPointsCount();
	int polygonTotalCount = pMesh->GetPolygonCount();

	// ���� Vertex ������ ������� ó������ �ʱ� ����..
	if (vertexTotalCount < 1) return;

	// Vertex ������ŭ BoneWeight ����..
	// �ش� Vertex Index�� ������ Index�� ����ġ ��, Bone Index Data ����..
	vector<BoneWeights> boneWeights(vertexTotalCount);
	bool isSkin = ProcessBoneWeights(node, boneWeights);

	// Bone Data ���� ����� ���� Skinning Object �Ǻ�..
	m_OneMesh->m_IsSkinningObject = isSkin;

	// ���ο� ���ؽ� ����..
	CreateVertex(pMesh, boneWeights, vertexTotalCount);

	int vertexCount = 0; // ������ ����
	for (int pi = 0; pi < polygonTotalCount; pi++)
	{
		// Polygon ������ŭ Face ����..
		m_OneMesh->m_meshface.push_back(new Face);

		for (int vi = 0; vi < 3; vi++)
		{
			int vertexIndex = pMesh->GetPolygonVertex(pi, vi);
			int uvIndex = pMesh->GetTextureUVIndex(pi, vi);

			if (vertexIndex < 0 || vertexIndex >= vertexTotalCount)	continue;

			DXVector3 fbxNormal;
			DXVector2 fbxUV;

			fbxNormal = GetNormal(pMesh, vertexIndex, vertexCount);
			fbxUV = GetUV(pMesh, vertexIndex, uvIndex);

			// �ϴ� �ϳ��� Mesh �� �ϳ��� Material�� �����ϰ� ����..
			if (m_OneMesh->m_materialdata == nullptr)
				LinkMaterialByPolygon(pMesh, pi, vertexIndex);

			// Face Vertex Index Data
			m_OneMesh->m_meshface[pi]->m_vertexindex[vi] = vertexIndex;
			// Face Vertex Normal Data
			m_OneMesh->m_meshface[pi]->m_normalvertex[vi] = fbxNormal;
			// Face Vertex UV Data
			m_OneMesh->m_meshface[pi]->m_uvvertex[vi] = fbxUV;

			vertexCount++;
		}
	}
}


bool FBXParser::ProcessBoneWeights(fbxsdk::FbxNode* node, vector<BoneWeights>& meshBoneWeights)
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

			FbxCluster::ELinkMode linkMode = FbxCluster::eNormalize;

			int clusterCount = skin->GetClusterCount();

			// Skin Mesh üũ..
			Mesh* skinMesh = FindMesh(node->GetName());
			vector<BoneWeights> skinBoneWeights(meshBoneWeights.size());
			for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++)
			{
				FbxCluster* cluster = skin->GetCluster(clusterIndex);
				if (cluster == nullptr) continue;

				linkMode = cluster->GetLinkMode();
				FbxNode* pLinkNode = cluster->GetLink();

				if (pLinkNode == nullptr) continue;

				string lineNodeName = pLinkNode->GetName();

				int BoneIndex = FindBoneIndex(lineNodeName);

				if (BoneIndex < 0)
				{
					continue;
				}

				// Bone Index�� �ش��ϴ� Bone ����..
				Bone* nowBone = m_Model->m_AllBoneList[BoneIndex].second;

				// Bone Mesh üũ..
				Mesh* boneMesh = FindMesh(lineNodeName);

				FbxAMatrix matClusterTransformMatrix;
				FbxAMatrix matClusterLinkTransformMatrix;

				const FbxVector4 lT = node->GetGeometricTranslation(FbxNode::eDestinationPivot);
				const FbxVector4 lR = node->GetGeometricRotation(FbxNode::eDestinationPivot);
				const FbxVector4 lS = node->GetGeometricScaling(FbxNode::eDestinationPivot);

				FbxAMatrix geometryTransform = FbxAMatrix(lT, lR, lS);

				cluster->GetTransformMatrix(matClusterTransformMatrix);
				cluster->GetTransformLinkMatrix(matClusterLinkTransformMatrix);

				// Bone Matrix ����..
				DXMatrix4X4 clusterMatrix = ConvertMatrix(matClusterTransformMatrix);
				DXMatrix4X4 clusterlinkMatrix = ConvertMatrix(matClusterLinkTransformMatrix);
				DXMatrix4X4 geometryMatrix = ConvertMatrix(geometryTransform);

				DXMatrix4X4 offsetMatrix = clusterMatrix * clusterlinkMatrix.Inverse() * geometryMatrix;

				skinMesh->m_boneTMList.emplace_back(offsetMatrix);
				skinMesh->m_boneList.emplace_back(boneMesh);

				int c = cluster->GetControlPointIndicesCount();
				for (int j = 0; j < cluster->GetControlPointIndicesCount(); ++j)
				{
					int index = cluster->GetControlPointIndices()[j];
					double weight = cluster->GetControlPointWeights()[j];

					if (weight == 0) continue;

					skinBoneWeights[index].AddBoneWeight(clusterIndex, (float)weight);
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
		string nodeName = node->GetName();
		Mesh* mesh = FindMesh(nodeName);

		if (mesh != nullptr)
		{
			FbxTime::EMode timeMode = pScene->GetGlobalSettings().GetTimeMode();
			for (FbxLongLong index = 0; index < m_OneAnimation->m_totalFrame; index++)
			{
				FbxTime takeTime;
				takeTime.SetFrame(m_OneAnimation->m_startFrame + index, timeMode);

				// Local Transform = �θ� Bone�� Global Transform�� Inverse Transform * �ڽ� Bone�� Global Transform
				FbxAMatrix nodeTransform = node->EvaluateLocalTransform(takeTime);

				DXMatrix4X4 nodeTRS = ConvertMatrix(nodeTransform);

				XMVECTOR scale;
				XMVECTOR rot;
				XMVECTOR pos;

				XMMatrixDecompose(&scale, &rot, &pos, nodeTRS);

				CAnimation_pos* newPos = new CAnimation_pos;
				CAnimation_rot* newRot = new CAnimation_rot;
				CAnimation_scl* newScale = new CAnimation_scl;

				newPos->m_time = (float)index;
				newPos->m_pos = DXVector3(pos);

				newRot->m_time = (float)index;
				newRot->m_rotQT_accumulation = Quaternion(rot);

				newScale->m_time = (float)index;
				newScale->m_scale = DXVector3(scale);

				m_OneAnimation->m_position.push_back(newPos);
				m_OneAnimation->m_rotation.push_back(newRot);
				m_OneAnimation->m_scale.push_back(newScale);
			}

			m_OneAnimation->m_lastPosFrame = m_OneAnimation->m_totalFrame - 1;
			m_OneAnimation->m_lastRotFrame = m_OneAnimation->m_totalFrame - 1;
			m_OneAnimation->m_lastScaleFrame = m_OneAnimation->m_totalFrame - 1;
			m_OneAnimation->m_isPosAnimation = true;
			m_OneAnimation->m_isRotAnimation = true;
			m_OneAnimation->m_isScaleAnimation = true;
			m_Model->m_isAnimation = true;

			// �ش� Mesh�� �ִϸ��̼� ����..
			mesh->m_animation = m_OneAnimation;
			mesh->m_isAnimation = true;
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
	if (pMesh->m_meshvertex.empty()) return;

	bool new_VertexSet = false;
	unsigned int resize_VertexIndex = pMesh->m_meshvertex.size();

	// �⺻ Vertex �־�ΰ� ����..
	for (unsigned int i = 0; i < pMesh->m_meshvertex.size(); i++)
	{
		Vertex* newVertex = new Vertex;
		newVertex->m_Pos = pMesh->m_meshvertex[i]->m_Pos;
		newVertex->m_indices = pMesh->m_meshvertex[i]->m_indices;
		newVertex->m_boneIndices = pMesh->m_meshvertex[i]->m_boneIndices;
		newVertex->m_boneWeights = pMesh->m_meshvertex[i]->m_boneWeights;

		pMesh->m_opt_vertex.push_back(newVertex);
	}

	// ���� Face���� �����ϴ� 3���� Vertex ��..
	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			unsigned int vertexIndex = pMesh->m_meshface[i]->m_vertexindex[j];

			Vertex* nowVertex = pMesh->m_opt_vertex[vertexIndex];

			// �ؽ�ó�� �ְ�, �������� �ʾ����� �ؽ�ó u,v ����..
			if (nowVertex->m_istextureset == false)
			{
				nowVertex->u = pMesh->m_meshface[i]->m_uvvertex[j].x;
				nowVertex->v = pMesh->m_meshface[i]->m_uvvertex[j].y;
				nowVertex->m_istextureset = true;
			}

			// ���� �ε��� �븻�� �˻�ÿ� �־��ְ� ����..
			if (nowVertex->m_isnormalset == false)
			{
				nowVertex->m_Normal = pMesh->m_meshface[i]->m_normalvertex[j];
				nowVertex->m_isnormalset = true;
			}

			// Normal, U, V ���� �Ѱ��� �ٸ��� Vertex ���� ����..
			if ((pMesh->m_opt_vertex[vertexIndex]->m_Normal != pMesh->m_meshface[i]->m_normalvertex[j]))
			{
				new_VertexSet = true;
			}

			if ((pMesh->m_opt_vertex[vertexIndex]->u != pMesh->m_meshface[i]->m_uvvertex[j].x) ||
				(pMesh->m_opt_vertex[vertexIndex]->v != pMesh->m_meshface[i]->m_uvvertex[j].y))
			{
				new_VertexSet = true;
			}

			if (new_VertexSet)
			{
				// �߰��� Vertex�� �ִٸ� üũ..
				if (resize_VertexIndex > (int)pMesh->m_meshvertex.size())
				{
					for (unsigned int k = pMesh->m_meshvertex.size(); k < resize_VertexIndex; k++)
					{
						// ���� �߰��� Vertex�� ������ �����͸� �����ִ� Face ���� Vertex Index ����..
						if ((pMesh->m_opt_vertex[k]->m_indices == pMesh->m_meshface[i]->m_vertexindex[j]) &&
							(pMesh->m_opt_vertex[k]->m_Normal == pMesh->m_meshface[i]->m_normalvertex[j]))
						{
							if ((pMesh->m_opt_vertex[k]->u == pMesh->m_meshface[i]->m_uvvertex[j].x) &&
								(pMesh->m_opt_vertex[k]->v == pMesh->m_meshface[i]->m_uvvertex[j].y))
							{
								pMesh->m_meshface[i]->m_vertexindex[j] = (int)k;
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
				newVertex->m_indices = nowVertex->m_indices;
				newVertex->m_Normal = pMesh->m_meshface[i]->m_normalvertex[j];
				newVertex->m_boneIndices = nowVertex->m_boneIndices;
				newVertex->m_boneWeights = nowVertex->m_boneWeights;
				newVertex->m_isnormalset = true;

				newVertex->u = pMesh->m_meshface[i]->m_uvvertex[j].x;
				newVertex->v = pMesh->m_meshface[i]->m_uvvertex[j].y;
				newVertex->m_istextureset = true;

				pMesh->m_opt_vertex.push_back(newVertex);
				pMesh->m_meshface[i]->m_vertexindex[j] = resize_VertexIndex;
				resize_VertexIndex++;
				new_VertexSet = false;
			}
		}
	}

	// Tanget �� ����..
	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		int index0 = pMesh->m_meshface[i]->m_vertexindex[0];
		int index1 = pMesh->m_meshface[i]->m_vertexindex[1];
		int index2 = pMesh->m_meshface[i]->m_vertexindex[2];

		DXVector3 ep1 = pMesh->m_opt_vertex[index1]->m_Pos - pMesh->m_opt_vertex[index0]->m_Pos;
		DXVector3 ep2 = pMesh->m_opt_vertex[index2]->m_Pos - pMesh->m_opt_vertex[index0]->m_Pos;

		DXVector3 uv1 = { pMesh->m_opt_vertex[index1]->u - pMesh->m_opt_vertex[index0]->u,
						  pMesh->m_opt_vertex[index1]->v - pMesh->m_opt_vertex[index0]->v };
		DXVector3 uv2 = { pMesh->m_opt_vertex[index2]->u - pMesh->m_opt_vertex[index0]->u,
						  pMesh->m_opt_vertex[index2]->v - pMesh->m_opt_vertex[index0]->v };

		float den = 1.0f / (uv1.x * uv2.y - uv2.x * uv1.y);

		// ���� �ȼ� ���̴����� ������ ���� T, B, N�� ��µ�
		// �ȼ� ���̴� ���� ������ ���ؽ� ���̴��� ���꿡 ���� ȣ�� Ƚ���� ������ �ٸ��� ũ�Ƿ� ���ϰ� �� �� �ִ�..
		// �������� �ȼ��� ���� �ȼ� ���̴� ���� �ƴϸ� ������ ���� ������ �ȼ� ���̴����� ������ �Ѵٰ� �Ѵ�..
		/// ���� ��������� ���ؽ� ���̴��� �Űܵ�
		DXVector3 tangent = (ep1 * uv2.y - ep2 * uv1.y) * den;
		tangent.Normalize();

		// ���� ������ ���� �����Ͽ� ���̴����� ��հ��� ����ϵ��� ����..
		pMesh->m_opt_vertex[index0]->m_Tanget += tangent;
		pMesh->m_opt_vertex[index1]->m_Tanget += tangent;
		pMesh->m_opt_vertex[index2]->m_Tanget += tangent;
	}

	// �ε����� �׳� ����
	pMesh->m_opt_index = new IndexList[pMesh->m_meshface.size()];

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			switch (j)
			{
			case 0:
				pMesh->m_opt_index[i].index[j] = pMesh->m_meshface[i]->m_vertexindex[0];
				break;
			case 1:
				pMesh->m_opt_index[i].index[j] = pMesh->m_meshface[i]->m_vertexindex[2];
				break;
			case 2:
				pMesh->m_opt_index[i].index[j] = pMesh->m_meshface[i]->m_vertexindex[1];
				break;
			default:
				break;
			}
		}
	}
}

void FBXParser::RecombinationTM(ParserData::Mesh* pMesh)
{
	// WorldTM -> LocalTM ��ȯ..
	DXVector4 row0 = pMesh->m_tm_row0;
	DXVector4 row1 = pMesh->m_tm_row1;
	DXVector4 row2 = pMesh->m_tm_row2;
	DXVector4 row3 = pMesh->m_tm_row3;
	row3.w = 1;

	//
	/// Negative Scale Check
	// 3D Max�� �۾��� Mirroring���� ���� Scale���� ������ ���� ��츦 Negative Scale �̶�� �Ѵ�
	// ó���� ������ ��� Normal ó���� Animation ó���ÿ� ������ �ȴ�
	// Rotaion�� ��Ÿ���� �� Row1 ~ Row3 �� ������ ���� �� ���� �ٸ� �� ���� �����Ͽ� �����̸� ���� �ݴ��̰� ����̸� ���� ��ġ�Ѵ�
	//
	XMVECTOR crossVec = XMVector3Cross(pMesh->m_tm_row0, pMesh->m_tm_row1);
	XMVECTOR dotVec = XMVector3Dot(crossVec, pMesh->m_tm_row2);

	XMFLOAT3 resultVec;

	XMStoreFloat3(&resultVec, dotVec);

	// �� ������ �ٸ� ��츸 ó��������..
	if (resultVec.x < 0 || resultVec.y < 0 || resultVec.z < 0)
	{
		XMVECTOR nodeScale;
		XMVECTOR nodetRot;
		XMVECTOR nodePos;

		// ���� ��Ʈ�������� �и�..
		XMMatrixDecompose(&nodeScale, &nodetRot, &nodePos, pMesh->m_worldTM);

		// ������ ������..
		nodeScale *= -1;

		// �븻�� ������..
		for (auto& k : pMesh->m_opt_vertex)
		{
			k->m_Normal *= -1;
		}

		/// �ؽ��ĳ� ���� �����͵� ��ȯ�� ���־�� �ұ�..?

		// ��Ʈ���� ������..
		pMesh->m_worldTM = XMMatrixScalingFromVector(nodeScale) * XMMatrixRotationQuaternion(nodetRot) * XMMatrixTranslationFromVector(nodePos);
	}
}

DXVector3 FBXParser::GetPos(fbxsdk::FbxMesh* mesh, int vertexIndex)
{
	FbxVector4 fbxPos;

	fbxPos = mesh->GetControlPointAt(vertexIndex);

	return ConvertVector3(fbxPos);
}

DXVector3 FBXParser::GetNormal(fbxsdk::FbxMesh* mesh, int vertexIndex, int vertexCount)
{
	if (mesh->GetElementNormalCount() < 1) return DXVector3();

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
			int index = vertexNormal->GetIndexArray().GetAt(vertexIndex); // �ε����� ���´�. 
			fbxNormal = vertexNormal->GetDirectArray().GetAt(index);
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
			int index = vertexNormal->GetIndexArray().GetAt(vertexCount); // �ε����� ���´�. 
			fbxNormal = vertexNormal->GetDirectArray().GetAt(index);
		}
		break;
		}
	}
	break;
	}

	return ConvertVector3(fbxNormal);
}


DXVector2 FBXParser::GetUV(fbxsdk::FbxMesh* mesh, int vertexIndex, int uvIndex)
{
	if (mesh->GetElementUVCount() < 1) return DXVector2();

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
			int index = vertexUV->GetIndexArray().GetAt(vertexIndex); // �ε����� ���´�. 
			fbxUV = vertexUV->GetDirectArray().GetAt(index);
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

	// uv Vertex Count ����..
	m_OneMesh->m_mesh_numtvertex++;

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
			m_OneMesh->m_materialdata = m_Model->m_list_materialdata[materialIndex];
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
				m_OneMesh->m_materialdata = m_Model->m_list_materialdata[materialIndex];
			}
		}
	}
}

void FBXParser::SetTransform(fbxsdk::FbxNode* node)
{
	if (node == nullptr) return;

	FbxMatrix worldpos = pScene->GetAnimationEvaluator()->GetNodeGlobalTransform(node);
	FbxMatrix localpos = pScene->GetAnimationEvaluator()->GetNodeLocalTransform(node);

	DXMatrix4X4 world = ConvertMatrix(worldpos);
	DXMatrix4X4 local = ConvertMatrix(localpos);

	if (m_OneMesh->m_parent == nullptr)
	{
		const auto yaw = -90.0f * KH_MATH::Pi / 180.0f;

		Quaternion q = XMQuaternionRotationRollPitchYaw(yaw, 0.0f, 0.0f);

		world *= XMMatrixRotationQuaternion(q);
		local *= XMMatrixRotationQuaternion(q);
	}

	DXVector4 r1 = world.GetRow(0);
	DXVector4 r2 = world.GetRow(1);
	DXVector4 r3 = world.GetRow(2);
	DXVector4 r4 = world.GetRow(3);

	m_OneMesh->m_tm_row0 = DXVector3(r1);
	m_OneMesh->m_tm_row1 = DXVector3(r2);
	m_OneMesh->m_tm_row2 = DXVector3(r3);
	m_OneMesh->m_tm_row3 = DXVector3(r4);

	m_OneMesh->m_worldTM = world;
	m_OneMesh->m_localTM = local;
}

DXMatrix4X4 FBXParser::GetGlobalAnimationTransform(fbxsdk::FbxNode* node, fbxsdk::FbxTime time)
{
	if (node == nullptr) return DXMatrix4X4();

	FbxAMatrix matrix = node->EvaluateGlobalTransform(time);
	return ConvertMatrix(matrix);
}

int FBXParser::GetMaterialIndex(fbxsdk::FbxSurfaceMaterial* material)
{
	for (unsigned int index = 0; index < fbxMaterials.size(); index++)
	{
		if (fbxMaterials[index] == material)
			return index;
	}

	return -1;
}

void FBXParser::SetMaterial(fbxsdk::FbxSurfaceMaterial* material)
{
	m_materialdata->m_material_name = material->GetName();

	if (material->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		// Ambient Data
		m_materialdata->m_material_ambient.x = static_cast<float>(((FbxSurfacePhong*)material)->Ambient.Get()[0]) * 10.0f;
		m_materialdata->m_material_ambient.y = static_cast<float>(((FbxSurfacePhong*)material)->Ambient.Get()[1]) * 10.0f;
		m_materialdata->m_material_ambient.z = static_cast<float>(((FbxSurfacePhong*)material)->Ambient.Get()[2]) * 10.0f;

		// Diffuse Data
		m_materialdata->m_material_diffuse.x = static_cast<float>(((FbxSurfacePhong*)material)->Diffuse.Get()[0]);
		m_materialdata->m_material_diffuse.y = static_cast<float>(((FbxSurfacePhong*)material)->Diffuse.Get()[1]);
		m_materialdata->m_material_diffuse.z = static_cast<float>(((FbxSurfacePhong*)material)->Diffuse.Get()[2]);

		// Specular Data
		m_materialdata->m_material_specular.x = static_cast<float>(((FbxSurfacePhong*)material)->Specular.Get()[0]);
		m_materialdata->m_material_specular.y = static_cast<float>(((FbxSurfacePhong*)material)->Specular.Get()[1]);
		m_materialdata->m_material_specular.z = static_cast<float>(((FbxSurfacePhong*)material)->Specular.Get()[2]);

		// Emissive Data
		m_materialdata->m_material_emissive.x = static_cast<float>(((FbxSurfacePhong*)material)->Emissive.Get()[0]);
		m_materialdata->m_material_emissive.y = static_cast<float>(((FbxSurfacePhong*)material)->Emissive.Get()[1]);
		m_materialdata->m_material_emissive.z = static_cast<float>(((FbxSurfacePhong*)material)->Emissive.Get()[2]);

		// Transparecy Data
		m_materialdata->m_material_transparency = static_cast<float>(((FbxSurfacePhong*)material)->TransparencyFactor.Get());

		// Shininess Data
		m_materialdata->m_material_shininess = static_cast<float>(((FbxSurfacePhong*)material)->Shininess.Get());

		// Reflectivity Data
		m_materialdata->m_material_reflectivity = static_cast<float>(((FbxSurfacePhong*)material)->ReflectionFactor.Get());
	}
	else if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		// Ambient Data
		m_materialdata->m_material_ambient.x = static_cast<float>(((FbxSurfaceLambert*)material)->Ambient.Get()[0]);
		m_materialdata->m_material_ambient.y = static_cast<float>(((FbxSurfaceLambert*)material)->Ambient.Get()[1]);
		m_materialdata->m_material_ambient.z = static_cast<float>(((FbxSurfaceLambert*)material)->Ambient.Get()[2]);

		// Diffuse Data
		m_materialdata->m_material_diffuse.x = static_cast<float>(((FbxSurfaceLambert*)material)->Diffuse.Get()[0]);
		m_materialdata->m_material_diffuse.y = static_cast<float>(((FbxSurfaceLambert*)material)->Diffuse.Get()[1]);
		m_materialdata->m_material_diffuse.z = static_cast<float>(((FbxSurfaceLambert*)material)->Diffuse.Get()[2]);

		// Emissive Data
		m_materialdata->m_material_emissive.x = static_cast<float>(((FbxSurfaceLambert*)material)->Emissive.Get()[0]);
		m_materialdata->m_material_emissive.y = static_cast<float>(((FbxSurfaceLambert*)material)->Emissive.Get()[1]);
		m_materialdata->m_material_emissive.z = static_cast<float>(((FbxSurfaceLambert*)material)->Emissive.Get()[2]);

		// Transparecy Data
		m_materialdata->m_material_transparency = static_cast<float>(((FbxSurfaceLambert*)material)->TransparencyFactor.Get());
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
						std::string count = to_string(m_Model->m_materialcount);
						if (textureType == "DiffuseColor")
						{
							m_materialdata->m_DiffuseMap = new MaterialMap();
							m_materialdata->m_isDiffuseMap = true;
							m_materialdata->m_DiffuseMap->m_map_name = "Diffuse Map" + count;
							m_materialdata->m_DiffuseMap->m_bitmap = fileRoute;
							m_materialdata->m_MapList.push_back(m_materialdata->m_DiffuseMap);
						}
						else if (textureType == "SpecularColor")
						{
							m_materialdata->m_SpecularMap = new MaterialMap();
							m_materialdata->m_isSpecularMap = true;
							m_materialdata->m_SpecularMap->m_map_name = "Specular Map" + count;
							m_materialdata->m_SpecularMap->m_bitmap = fileRoute;
							m_materialdata->m_MapList.push_back(m_materialdata->m_SpecularMap);
						}
						else if (textureType == "NormalMap")
						{
							m_materialdata->m_BumpMap = new MaterialMap();
							m_materialdata->m_isBumpMap = true;
							m_materialdata->m_BumpMap->m_map_name = "Normal Map" + count;
							m_materialdata->m_BumpMap->m_bitmap = fileRoute;
							m_materialdata->m_MapList.push_back(m_materialdata->m_BumpMap);
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
		m_OneMesh->m_meshvertex.push_back(new Vertex);

		fbxPos = mesh->GetControlPointAt(vertexIndex);

		DXVector3 pos = ConvertVector3(fbxPos);

		// Bounding Box üũ�� Position
		DXVector3 nowPos = NoConvertVector3(fbxPos);

		m_OneMesh->m_meshvertex[vertexIndex]->m_Pos = pos;
		m_OneMesh->m_meshvertex[vertexIndex]->m_indices = vertexIndex;

		// Bone Weight Data
		for (unsigned int boneIndex = 0; boneIndex < boneWeights[vertexIndex].m_BoneWeights.size(); boneIndex++)
		{
			int iBone = boneWeights[vertexIndex].m_BoneWeights[boneIndex].first;

			// FBX�� Bone Mesh Data �� �����Ƿ� ����� Mesh Vertex ���Ͽ� BoundingBox ����..
			m_Model->m_AllBoneList[iBone].second->m_Min = XMVectorMin(m_Model->m_AllBoneList[iBone].second->m_Min, nowPos);
			m_Model->m_AllBoneList[iBone].second->m_Max = XMVectorMax(m_Model->m_AllBoneList[iBone].second->m_Max, nowPos);
			m_Model->m_AllBoneList[iBone].second->m_isCol = true;

			m_OneMesh->m_meshvertex[vertexIndex]->m_boneIndices.push_back(boneWeights[vertexIndex].m_BoneWeights[boneIndex].first);
			m_OneMesh->m_meshvertex[vertexIndex]->m_boneWeights.push_back(boneWeights[vertexIndex].m_BoneWeights[boneIndex].second);
		}
	}
}

void FBXParser::CreateMesh()
{
	m_OneMesh = nullptr;
	m_OneMesh = new ParserData::Mesh();
	m_Model->m_MeshList.push_back(m_OneMesh);
}

void FBXParser::CreateBone()
{
	m_OneBone = nullptr;
	m_OneBone = new Bone();
}

int FBXParser::FindBoneIndex(std::string boneName)
{
	for (BonePair bone : m_Model->m_AllBoneList)
	{
		if (bone.first == boneName)
			return bone.second->m_bone_number;
	}

	return -1;
}

ParserData::Bone* FBXParser::FindBone(std::string boneName)
{
	for (BonePair bone : m_Model->m_AllBoneList)
	{
		if (bone.first == boneName)
			return bone.second;
	}

	return nullptr;
}

ParserData::Mesh* FBXParser::FindMesh(std::string meshName)
{
	for (Mesh* mesh : m_Model->m_MeshList)
	{
		if (mesh->m_nodename == meshName)
			return mesh;
	}

	return nullptr;
}