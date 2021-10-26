#pragma once
#pragma warning(disable : 4251)

#include "SimpleMath.h"

namespace ParserData
{
	struct OneFrame
	{
		float							m_Time;
		DirectX::SimpleMath::Vector3	m_Pos;
		DirectX::SimpleMath::Quaternion m_RotQt;
		DirectX::SimpleMath::Vector3	m_Scale;
	};

	struct OneAnimation
	{
	public:
		OneAnimation();
		OneAnimation(const OneAnimation& ani) = default;
		~OneAnimation();

	public:
		std::vector<OneFrame*>	m_AniData;

		float	m_TicksPerFrame;
		float	m_FrameTime;
		int		m_TickFrame;
		int		m_TotalFrame;
		int		m_StartFrame;
		int		m_EndFrame;
		int		m_NowIndex;
		int		m_NextIndex;
	};
}

namespace ParserData
{
	struct IndexList
	{
		int m_Index[3];
	};

	struct Vertex
	{
	public:
		Vertex();
		~Vertex();

	public:
		DirectX::SimpleMath::Vector3 m_Pos;
		DirectX::SimpleMath::Vector3 m_Normal;
		DirectX::SimpleMath::Vector3 m_Tanget;
		float m_U, m_V;

		bool m_IsNormalSet;		// Normal Data Setting 여부
		bool m_IsTextureSet;	// Texture Data Setting 여부

		/// Skinned Data
		std::vector<float> m_BoneWeights;	// BoneWeight List
		std::vector<UINT> m_BoneIndices;	// BoneIndex List

		// 인덱스
		int m_Indices;
	};

	struct Face
	{
	public:
		Face();
		~Face() = default;

	public:
		int	m_VertexIndex[3];	// Face Vertex List Index
		int	m_TFace[3];			// Texture Coordinate
		
		DirectX::SimpleMath::Vector3 m_Normal;				// Face Normal

		DirectX::SimpleMath::Vector3 m_NormalVertex[3];		// Vertex Normal
		DirectX::SimpleMath::Vector3 m_TangentVertex[3];	// Vertex Tangent
		DirectX::SimpleMath::Vector2 m_UVvertex[3];			// Vertex UV
	};

	struct MaterialMap
	{
		std::string	m_MapName;
		std::string	m_BitMap;
	};

	struct CMaterial
	{
	public:
		CMaterial();
		~CMaterial();

	public:
		int	m_MaterialNumber;
		std::string	m_MaterialName;

		DirectX::SimpleMath::Vector3 m_Material_Ambient;
		DirectX::SimpleMath::Vector3 m_Material_Diffuse;
		DirectX::SimpleMath::Vector3 m_Material_Specular;
		DirectX::SimpleMath::Vector3 m_Material_Emissive;
		float m_Material_Shininess;
		float m_Material_Transparency;
		float m_Material_Reflectivity;

		MaterialMap* m_DiffuseMap;			// DiffuseMap Data
		MaterialMap* m_BumpMap;				// BumpMap Data
		MaterialMap* m_SpecularMap;			// SpecularMap Data
		MaterialMap* m_ShineMap;			// ShineMap Data

		bool m_IsDiffuseMap;
		bool m_IsBumpMap;
		bool m_IsSpecularMap;
		bool m_IsShineMap;

		std::vector<MaterialMap*> m_MapList;
		CMaterial* m_SubMaterial;
	};

	class Mesh
	{
	public:
		Mesh();
		Mesh(const Mesh& mesh) = default;
		virtual ~Mesh();

	public:
		std::string	m_NodeName; 
		std::string	m_ParentName;

		/// Object Type Data
		bool m_TopNode;
		bool m_IsSkinningObject;
		bool m_IsBone;

		/// Transform Matrix Data
		DirectX::SimpleMath::Vector3 m_tm_row0;
		DirectX::SimpleMath::Vector3 m_tm_row1;
		DirectX::SimpleMath::Vector3 m_tm_row2;
		DirectX::SimpleMath::Vector3 m_tm_row3;

		DirectX::SimpleMath::Matrix m_WorldTM;		// Mesh WorldTM
		DirectX::SimpleMath::Matrix m_LocalTM;		// Mesh LocalTM

		/// Material Data
		CMaterial*					m_MaterialData;		// Mesh Material Data

		/// Face Data (파싱용)
		std::vector<Face*>			m_MeshFace;			// Face List

		/// Animation Data
		OneAnimation*				m_Animation;		// Animation Data

		/// Skinning Data
		std::vector<DirectX::SimpleMath::Matrix>	m_BoneTMList;		// Bone Offset TM List
		std::vector<Mesh*>			m_BoneMeshList;		// Bone Mesh List

		/// Final Data
		std::vector<Vertex*>		m_VertexList;		// Vertex List
		std::vector<IndexList*>		m_IndexList;		/// 최적화 후 Final Index List
	};
}

namespace ParserData
{
	struct Scenedata
	{
	public:
		std::string			m_FileName;
		int					m_FirstFrame;
		int					m_LastFrame;
		int					m_FrameSpeed;
		float				m_TicksPerFrame;

	public:
		Scenedata();
		~Scenedata();
	};

	struct TVertex
	{
		TVertex();

		float m_U, m_V, m_W;
	};

	struct Bone
	{
		std::string	m_BoneName;
		int			m_BoneNumber;
		int			m_parent_bone_number;
	};

	class ASEMesh : public Mesh
	{
	public:
		ASEMesh();
		~ASEMesh();

	public:
		int	m_Type;
		
		int	m_Mesh_NumVertex;
		int	m_Mesh_NumFaces;

		int	m_Mesh_NumTVertex;						
		int	m_Mesh_SumTVertex;						
		std::vector<TVertex*> m_Mesh_TVertex;

		Mesh* m_Parent;
		
		std::vector<Bone*> m_BoneList;
	};
}

namespace ParserData
{
	struct Model
	{
		Model();
		~Model();

		bool m_isAnimation;											// Animation 유무

		std::vector<ParserData::CMaterial*> m_MaterialList;			// Material List
		std::vector<ParserData::Mesh*> m_MeshList;	 				// Mesh List
	};
}
