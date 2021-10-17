#pragma once

namespace ParserData
{
	struct PARSER_DLL IndexList
	{
		int m_Index[3];
	};

	struct PARSER_DLL Vertex
	{
		Vertex();

		DXVector3 m_Pos;
		DXVector3 m_Normal;
		DXVector3 m_Tanget;
		float m_U, m_V;

		// Normal Data Setting 여부
		bool m_IsNormalSet;

		// Texture Data Setting 여부
		bool m_IsTextureSet;

		/// Skinned Data
		std::vector<float> m_BoneWeights;
		std::vector<UINT> m_BoneIndices;

		// 인덱스
		int m_Indices;
	};

	struct Face
	{
	public:
		// 이 Face를 이루는 버텍스의 인덱스
		int					m_VertexIndex[3];
		// Face의 Normal값
		DXVector3			m_Normal;
		// Vertex의 Normal값
		DXVector3			m_NormalVertex[3];
		// Vertex의 Tangent값
		DXVector3			m_TangentVertex[3];
		// Vertex의 UV값
		DXVector2			m_UVvertex[3];
		// Texture Coordinate
		int					m_TFace[3];

	public:
		Face();
	};

	struct PARSER_DLL MaterialMap
	{
		std::string	m_MapName;
		std::string	m_BitMap;
	};

	struct PARSER_DLL CMaterial
	{
	public:
		int					m_MaterialNumber;
		std::string			m_MaterialName;

		DXVector3			m_Material_Ambient;
		DXVector3			m_Material_Diffuse;
		DXVector3			m_Material_Specular;
		DXVector3			m_Material_Emissive;
		float				m_Material_Shininess;
		float				m_Material_Transparency;
		float				m_Material_Reflectivity;

		MaterialMap*		m_DiffuseMap;			// DiffuseMap Data
		MaterialMap*		m_BumpMap;				// BumpMap Data
		MaterialMap*		m_SpecularMap;			// SpecularMap Data
		MaterialMap*		m_ShineMap;				// ShineMap Data

		bool				m_IsDiffuseMap;
		bool				m_IsBumpMap;
		bool				m_IsSpecularMap;
		bool				m_IsShineMap;

		std::vector<MaterialMap*> m_MapList;	
		CMaterial* m_SubMaterial;				

	public:
		CMaterial();
		~CMaterial();
	};

	// Bone 하나의 구조체
	struct PARSER_DLL Bone
	{
		std::string	m_BoneName;
		int			m_BoneNumber;
		int			m_parent_bone_number;
	};

	class PARSER_DLL Mesh
	{
	public:
		std::string	m_NodeName;
		std::string	m_ParentName;

		/// Object Type Data
		bool m_TopNode;
		bool m_IsSkinningObject;
		bool m_IsBone;

		/// Transform Matrix Data
		DXVector3	m_tm_row0;
		DXVector3	m_tm_row1;
		DXVector3	m_tm_row2;
		DXVector3	m_tm_row3;

		DXMatrix4X4 m_WorldTM;		// Mesh WorldTM
		DXMatrix4X4 m_LocalTM;		// Mesh LocalTM

		/// Material Data
		ParserData::CMaterial*		m_MaterialData;		// Mesh Material Data

		/// Vertex Data
		std::vector<Vertex*>		m_MeshVertex;		// Vertex List
		std::vector<Face*>			m_MeshFace;			// Face List

		/// Animation Data
		OneAnimation*				m_Animation;		// Animation Data

		/// Skinning Data
		std::vector<DXMatrix4X4>	m_BoneTMList;		// Bone Offset TM List
		std::vector<Mesh*>			m_BoneMeshList;		// Bone Mesh List

		/// Final Data
		std::vector<Vertex*>		m_Final_Vertex;		/// 최적화 후 Final Vertex List
		ParserData::IndexList*		m_Final_Index;		/// 최적화 후 Final Index List

	public:
		Mesh();
		~Mesh();
	};

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

	class PARSER_DLL ASEMesh : public Mesh
	{
	public:
		int	m_Type;
		
		int	m_Mesh_NumVertex;
		int	m_Mesh_NumFaces;

		// 텍스쳐용 데이터들
		int	m_Mesh_NumTVertex;						// 텍스쳐용 버텍스 갯수
		int	m_Mesh_SumTVertex;						// 텍스쳐용 버텍스 총 갯수
		std::vector<TVertex*> m_Mesh_TVertex;		// 텍스쳐용 버텍스

		Mesh* m_Parent;								// Parent Mesh
		
		std::vector<Bone*> m_BoneList;
	};
}



