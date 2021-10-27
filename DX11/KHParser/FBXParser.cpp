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
	// FBX SDK Manager 생성..
	pManager = FbxManager::Create();

	if (!pManager)
		throw std::exception("error: unable to create FBX manager!\n");

	// Scene 생성..
	pScene = FbxScene::Create(pManager, "My Scene");

	// IOSettings 객체 생성 및 설정
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	FbxString lExtension = "dll";
	FbxString lPath = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(lPath.Buffer(), lExtension.Buffer());

	// FbxImporter 객체 생성
	pImporter = FbxImporter::Create(pManager, "");

	// Convert 객체 생성
	pConverter = new FbxGeometryConverter(pManager);

	if (!pScene)
		throw std::exception("error: unable to create FBX scene\n");
}

void FBXParser::SetTextureRoute(std::string texRoute)
{
	// 기본 Texture 파일 루트 설정
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
	// Scene 설정..
	SceneSetting(fileName, scaling, onlyAni);

	// Scene에서 RootNode 가져오기..
	fbxsdk::FbxNode* pRootNode = pScene->GetRootNode();

	// 혹시라도 RootNode가 없을 경우를 대비..
	if (pRootNode == nullptr) return nullptr;

	/// Material Data Loading..
	LoadMaterial();

	/// Bone Data Loading..
	LoadNode(pRootNode, FbxNodeAttribute::eSkeleton);

	/// Mesh Data Loading..
	LoadNode(pRootNode, FbxNodeAttribute::eMesh);

	/// Mesh Data Optimizing..
	OptimizeData();

	// 필요없는 데이터 제거..
	ResetData();

	return m_Model;
}

void FBXParser::SceneSetting(std::string fileName, bool scaling, bool onlyAni)
{
	// Model 생성..
	CreateModel();

	// 파일 이름과 옵션 설정..
	fbxFileName = fileName;
	m_OnlyAni = onlyAni;

	if (!pImporter->Initialize(fbxFileName.c_str(), -1, pManager->GetIOSettings()))
		throw std::exception("error: initialize importer\n");

	// fbx 파일 내용을 Scene으로 가져온다..
	pImporter->Import(pScene);

	// Scene 내에 있는 데이터들의 단위를 변경해준다..
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

	// Scene 내에서 삼각형화 할 수 있는 모든 노드를 삼각형화 시킨다..
	// 3D Max 안에서 Editable poly 상태라면 이 작업을 안해야 한다..
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
	// 애니메이션만 뽑을경우..
	if (m_OnlyAni) return;

	// Scene에 존재하는 Material 개수만큼 생성..
	int mcount = pScene->GetMaterialCount();
	for (int matIndex = 0; matIndex < pScene->GetMaterialCount(); matIndex++)
	{
		FbxSurfaceMaterial* material = pScene->GetMaterial(matIndex);
		m_MaterialData = new CMaterial();
		m_MaterialData->m_MaterialNumber = matIndex;

		// Material Data 삽입..
		SetMaterial(material);

		// Texture Data 삽입..
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

	// 재귀 구조..
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
		// 구동시간 동안 총 몇 프레임이 수행될지를 keyFrames에 담아줌
		int keyFrames = (int)((tempStop - tempStart) * (double)frameRate);
		double ticksperFrame = (tempStop - tempStart) / keyFrames;

		// 새로운 Animaiton Data 생성..
		m_OneAnimation = new OneAnimation();

		// 한 프레임 재생 시간..
		m_OneAnimation->m_TicksPerFrame = (float)ticksperFrame;

		// 애니메이션 시작 프레임..
		m_OneAnimation->m_StartFrame = (int)(tempStart)*keyFrames;
		m_OneAnimation->m_EndFrame = keyFrames - 1;

		// 애니메이션 총 프레임..
		m_OneAnimation->m_TotalFrame = keyFrames;

		// 애니메이션 정보가 있을경우..
		m_Model->m_isAnimation = true;

		ProcessAnimation(node);
	}
}

void FBXParser::ProcessSkeleton(fbxsdk::FbxNode* node)
{
	pMesh = node->GetMesh();

	// 새로운 Mesh 생성..
	CreateMesh();

	m_OneMesh->m_NodeName = node->GetName();
	m_OneMesh->m_IsBone = true;

	// 애니메이션만 뽑을 경우..
	if (m_OnlyAni) return;

	// 현 Node Parent 찾기..
	const char* parentName = node->GetParent()->GetName();
	Mesh* parentMesh = FindMesh(parentName);
	m_OneMesh->m_ParentName = parentName;

	if (parentMesh == nullptr)
		m_OneMesh->m_TopNode = true;

	// Node TRS 설정..
	SetTransform(node);

	FbxSkeleton* fbxSkeleton = node->GetSkeleton();

	if (fbxSkeleton == nullptr) return;

	// 부모 Bone Index 초기값..
	// 부모 Bone Index가 -1 이면 최상위 Root Node..
	int parentBoneIndex = -1;

	// 최상위 노드(부모노드)인지 확인..
	FbxNode* parentNode = node->GetParent();

	if (m_OneMesh->m_TopNode)
	{
		std::string nodeName = parentNode->GetName();
		parentBoneIndex = FindBoneIndex(nodeName);
	}

	// 새로운 Bone 생성..
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

	// 새로운 Mesh 생성..
	CreateMesh();

	m_OneMesh->m_NodeName = node->GetName();

	// 애니메이션만 뽑을 경우..
	if (m_OnlyAni) return;

	// 현 Node Parent 찾기..
	const char* parentName = node->GetParent()->GetName();
	Mesh* parentMesh = FindMesh(parentName);
	m_OneMesh->m_ParentName = parentName;

	// 부모의 Mesh가 존재한다면 ChildList에 추가..
	if (parentMesh == nullptr)
		m_OneMesh->m_TopNode = true;

	// Node TRS 설정..
	SetTransform(node);

	int vertexTotalCount = pMesh->GetControlPointsCount();
	int polygonTotalCount = pMesh->GetPolygonCount();

	// 만약 Vertex 정보가 없을경우 처리하지 않기 위함..
	if (vertexTotalCount < 1) return;

	// Vertex 개수만큼 BoneWeight 생성..
	// 해당 Vertex Index와 동일한 Index에 가중치 값, Bone Index Data 설정..
	std::vector<BoneWeights> boneWeights(vertexTotalCount);
	bool isSkin = ProcessBoneWeights(node, boneWeights);

	// Bone Data 설정 결과에 따른 Skinning Object 판별..
	m_OneMesh->m_IsSkinningObject = isSkin;

	// 새로운 버텍스 생성..
	CreateVertex(pMesh, boneWeights, vertexTotalCount);

	int vertexCount = 0; // 정점의 개수
	for (int pi = 0; pi < polygonTotalCount; pi++)
	{
		// Polygon 개수만큼 Face 생성..
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

			// 일단 하나의 Mesh 당 하나의 Material로 생각하고 가자..
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

	// DeformerCount가 1보다 작으면 Bone Data가 없다고 가정..
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

			// Skin Mesh 체크..
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

				// Bone Mesh 체크..
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

				// Bone Matrix 설정..
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
				// 가중치 합을 1.0으로 만드는 작업..
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
	// 애니메이션만 뽑을경우..
	if (m_OnlyAni)
	{
		FbxMesh* mesh = node->GetMesh();

		if (mesh != nullptr)
		{
			int deformerCount = mesh->GetDeformerCount();

			// DeformerCount가 0보다 크면 Skinning Mesh..
			if (deformerCount > 0)
				return;
		}
	}

	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

	// 만약 스키닝 오브젝트라면 애니메이션 데이터는 Bone에 저장되어 있으므로..
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

				// Local Transform = 부모 Bone의 Global Transform의 Inverse Transform * 자신 Bone의 Global Transform
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

			// 해당 Mesh에 애니메이션 삽입..
			mesh->m_Animation = m_OneAnimation;
		}
	}
}

void FBXParser::OptimizeData()
{
	// 애니메이션만 뽑을경우..
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

	// 각각 Face마다 존재하는 3개의 Vertex 비교..
	for (unsigned int i = 0; i < pMesh->m_MeshFace.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			unsigned int vertexIndex = pMesh->m_MeshFace[i]->m_VertexIndex[j];

			Vertex* nowVertex = pMesh->m_VertexList[vertexIndex];

			// 텍스처가 있고, 설정하지 않았으면 텍스처 u,v 설정..
			if (nowVertex->m_IsTextureSet == false)
			{
				nowVertex->m_U = pMesh->m_MeshFace[i]->m_UVvertex[j].x;
				nowVertex->m_V = pMesh->m_MeshFace[i]->m_UVvertex[j].y;
				nowVertex->m_IsTextureSet = true;
			}

			// 최초 인덱스 노말값 검사시엔 넣어주고 시작..
			if (nowVertex->m_IsNormalSet == false)
			{
				nowVertex->m_Normal = pMesh->m_MeshFace[i]->m_NormalVertex[j];
				nowVertex->m_IsNormalSet = true;
			}

			// Normal, U, V 값중 한개라도 다르면 Vertex 새로 생성..
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
				// 추가된 Vertex가 있다면 체크..
				if (resize_VertexIndex > (int)pMesh->m_VertexList.size())
				{
					for (unsigned int k = pMesh->m_VertexList.size(); k < resize_VertexIndex; k++)
					{
						// 새로 추가한 Vertex와 동일한 데이터를 갖고있는 Face 내의 Vertex Index 수정..
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

			// 새로 추가해야할 Vertex..
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

	// Tanget 값 설정..
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

		// 현재 픽셀 쉐이더에서 연산을 통해 T, B, N을 얻는데
		// 픽셀 쉐이더 내의 연산은 버텍스 쉐이더의 연산에 비해 호출 횟수가 차원이 다르게 크므로 부하가 올 수 있다..
		// 법선맵의 픽셀의 색은 픽셀 쉐이더 안이 아니면 얻을수 없기 때문에 픽셀 쉐이더에서 연산을 한다고 한다..
		/// 현재 연산과정을 버텍스 쉐이더로 옮겨둠
		DirectX::SimpleMath::Vector3 tangent = (ep1 * uv2.y - ep2 * uv1.y) * den;
		tangent.Normalize();

		// 유사 정점은 값을 누적하여 쉐이더에서 평균값을 사용하도록 하자..
		pMesh->m_VertexList[index0]->m_Tanget += tangent;
		pMesh->m_VertexList[index1]->m_Tanget += tangent;
		pMesh->m_VertexList[index2]->m_Tanget += tangent;
	}

	// 인덱스는 그냥 복사
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
	// 3D Max의 작업상 Mirroring으로 인해 Scale값이 음수가 나올 경우를 Negative Scale 이라고 한다
	// 처리를 안해줄 경우 Normal 처리나 Animation 처리시에 문제가 된다
	// Rotaion을 나타내는 축 Row1 ~ Row3 중 두축을 외적 한 값과 다른 한 축을 내적하여 음수이면 축이 반대이고 양수이면 축이 일치한다
	//
	XMVECTOR crossVec = XMVector3Cross(pMesh->m_WorldTM.Right(), pMesh->m_WorldTM.Up());
	XMVECTOR dotVec = XMVector3Dot(crossVec, pMesh->m_WorldTM.Backward());

	XMFLOAT3 resultVec;

	XMStoreFloat3(&resultVec, dotVec);

	// 축 방향이 다른 경우만 처리해주자..
	if (resultVec.x < 0 || resultVec.y < 0 || resultVec.z < 0)
	{
		XMVECTOR nodeScale;
		XMVECTOR nodetRot;
		XMVECTOR nodePos;

		// 기존 매트릭스에서 분리..
		XMMatrixDecompose(&nodeScale, &nodetRot, &nodePos, pMesh->m_WorldTM);

		// 스케일 재조정..
		nodeScale *= -1;

		// 노말값 재조정..
		for (auto& k : pMesh->m_VertexList)
		{
			k->m_Normal *= -1;
		}

		/// 텍스쳐나 맵핑 데이터도 변환을 해주어야 할까..?

		// 매트릭스 재조합..
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

	switch (vertexNormal->GetMappingMode()) // 매핑 모드 
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
			int m_Index = vertexNormal->GetIndexArray().GetAt(vertexIndex); // 인덱스를 얻어온다. 
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
			int m_Index = vertexNormal->GetIndexArray().GetAt(vertexCount); // 인덱스를 얻어온다. 
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

	switch (vertexUV->GetMappingMode()) // 매핑 모드 
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
			int m_Index = vertexUV->GetIndexArray().GetAt(vertexIndex); // 인덱스를 얻어온다. 
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
			// materialIndex : 우리가 부여한 인덱스
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
				// materialIndex : 우리가 부여한 인덱스
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
						// 변환된 파일 경로..
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

	// Vertex 개수만큼 Vertex 생성..
	for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++)
	{
		m_OneMesh->m_VertexList.push_back(new Vertex);

		fbxPos = mesh->GetControlPointAt(vertexIndex);

		DirectX::SimpleMath::Vector3 pos = ConvertVector3(fbxPos);

		// Bounding Box 체크용 Position
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
