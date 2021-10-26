#define SAFE_DELETE(x) { if(x != nullptr) {delete x; x = nullptr;} }

#include "ParserData.h"
using namespace std;

namespace ParserData
{
	//OneAnimation::OneAnimation()
	//{
	//	m_TicksPerFrame = 0.0f;
	//	m_FrameTime = 0.0f;
	//	m_TickFrame = 0;
	//	m_TotalFrame = 0;
	//	m_StartFrame = 0;
	//	m_EndFrame = 0;
	//	m_NowIndex = 0;
	//	m_NextIndex = 1;
	//}
	//OneAnimation::~OneAnimation()
	//{
	//	for (OneFrame* frame : m_AniData)
	//	{
	//		SAFE_DELETE(frame);
	//	}
	//
	//	m_AniData.clear();
	//}

	//Vertex::Vertex()
	//{
	//	m_Pos				= DirectX::SimpleMath::Vector3();
	//	m_Normal			= DirectX::SimpleMath::Vector3();
	//	m_IsNormalSet		= false;
	//	m_IsTextureSet		= false;
	//	m_U					= 0.0f;
	//	m_V					= 0.0f;
	//}

	//Vertex::~Vertex()
	//{
	//	m_BoneWeights.clear();
	//	m_BoneIndices.clear();
	//}

	//Face::Face()
	//{
	//	m_VertexIndex[0] = 0;
	//	m_VertexIndex[1] = 0;
	//	m_VertexIndex[2] = 0;
	//
	//	m_Normal		 = DirectX::SimpleMath::Vector3();
	//}

	//TVertex::TVertex()
	//{
	//	m_U = 0;
	//	m_V = 0;
	//	m_W = 0;
	//}

	//Scenedata::Scenedata()
	//{
	//	m_FirstFrame				= 0;
	//	m_LastFrame					= 0;
	//	m_FrameSpeed				= 0;
	//	m_TicksPerFrame				= 0;
	//}

	//CMaterial::CMaterial()
	//{
	//	m_MaterialNumber			= 0;
	//	m_Material_Ambient			= DirectX::SimpleMath::Vector3();
	//	m_Material_Diffuse			= DirectX::SimpleMath::Vector3();
	//	m_Material_Specular			= DirectX::SimpleMath::Vector3();
	//	m_Material_Emissive			= DirectX::SimpleMath::Vector3();
	//	m_Material_Shininess		= 0.0f;
	//	m_Material_Transparency		= 0.0f;
	//	m_Material_Reflectivity		= 0.0f;
	//
	//	m_IsDiffuseMap				= false;
	//	m_IsBumpMap					= false;
	//	m_IsSpecularMap				= false;
	//	m_IsShineMap				= false;
	//}

	//CMaterial::~CMaterial()
	//{
	//	m_DiffuseMap = nullptr;
	//	m_BumpMap = nullptr;
	//	m_SpecularMap = nullptr;
	//	m_ShineMap = nullptr;
	//	m_SubMaterial = nullptr;
	//
	//	for (MaterialMap* matMap : m_MapList)
	//	{
	//		SAFE_DELETE(matMap);
	//	}
	//
	//	m_MapList.clear();
	//}

	//Mesh::Mesh()
	//{
	//	///----------------------------------
	//	/// *NODE_TM (Transform Matrix)
	//	///----------------------------------
	//	m_tm_row0			= DirectX::SimpleMath::Vector3();
	//	m_tm_row1			= DirectX::SimpleMath::Vector3();
	//	m_tm_row2			= DirectX::SimpleMath::Vector3();
	//	m_tm_row3			= DirectX::SimpleMath::Vector3();
	//
	//	///----------------------------------
	//	/// 추가 데이터
	//	///----------------------------------
	//	m_MaterialData		= nullptr;
	//	m_TopNode			= false;
	//
	//	///----------------------------------
	//	/// Animation
	//	///----------------------------------
	//	m_Animation			= nullptr; 
	//
	//	///----------------------------------
	//	/// Skinning Object
	//	///----------------------------------
	//	m_IsSkinningObject		= false;
	//	m_IsBone				= false;
	//}

	//Mesh::~Mesh()
	//{
	//	for (Face* face : m_MeshFace)
	//	{
	//		SAFE_DELETE(face);
	//	}
	//
	//	for (Vertex* vertex : m_VertexList)
	//	{
	//		SAFE_DELETE(vertex);
	//	}
	//
	//	for (IndexList* index : m_IndexList)
	//	{
	//		SAFE_DELETE(index);
	//	}
	//
	//	SAFE_DELETE(m_Animation);
	//
	//	m_MaterialData = nullptr;
	//
	//	m_BoneTMList.clear();
	//	m_BoneMeshList.clear();
	//	m_MeshFace.clear();
	//	m_VertexList.clear();
	//	m_IndexList.clear();
	//}

	//ASEMesh::ASEMesh()
	//{
	//	m_Type				= 0;
	//	m_Mesh_NumVertex	= 0;
	//	m_Mesh_NumFaces		= 0;
	//	m_Mesh_NumTVertex	= 0;
	//	m_Mesh_SumTVertex	= 0;
	//
	//	m_Parent			= nullptr;
	//}

	//ASEMesh::~ASEMesh()
	//{
	//	m_Parent = nullptr;
	//
	//	for (TVertex* tV : m_Mesh_TVertex)
	//	{
	//		SAFE_DELETE(tV);
	//	}
	//
	//	for (Bone* bone : m_BoneList)
	//	{
	//		SAFE_DELETE(bone);
	//	}
	//
	//	m_Mesh_TVertex.clear();
	//	m_BoneList.clear();
	//}

	//Model::Model()
	//{
	//	m_isAnimation = false;
	//}

	//Model::~Model()
	//{
	//	for (CMaterial* mat : m_MaterialList)
	//	{
	//		SAFE_DELETE(mat);
	//	}
	//
	//	for (Mesh* mesh : m_MeshList)
	//	{
	//		SAFE_DELETE(mesh);
	//	}
	//
	//	m_MaterialList.clear();
	//	m_MeshList.clear();
	//}
}


