#pragma once
#pragma warning(disable : 4251)

namespace ParserData
{
	struct PARSER_DLL OneFrame
	{
		float		 m_Time;
		DXVector3	 m_Pos;
		DXQuaternion m_RotQt;
		DXVector3	 m_Scale;
	};

	struct PARSER_DLL OneAnimation
	{
		OneAnimation() {}
		OneAnimation(const OneAnimation& ani) = default;

		std::vector<OneFrame*>	m_AniData;

		float	m_TicksPerFrame = 0.0f;
		float	m_FrameTime = 0.0f;
		int		m_TickFrame = 0;
		int		m_TotalFrame = 0;
		int		m_StartFrame = 0;
		int		m_EndFrame = 0;
		int		m_NowIndex = 0;
		int		m_NextIndex = 1;

		bool	m_isScaleAnimation = false;
		bool	m_isPosAnimation = false;
		bool	m_isRotAnimation = false;
	};
}

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

		// Normal Data Setting ����
		bool m_IsNormalSet;

		// Texture Data Setting ����
		bool m_IsTextureSet;

		/// Skinned Data
		std::vector<float> m_BoneWeights;
		std::vector<UINT> m_BoneIndices;

		// �ε���
		int m_Indices;
	};

	struct Face
	{
	public:
		// �� Face�� �̷�� ���ؽ��� �ε���
		int					m_VertexIndex[3];
		// Face�� Normal��
		DXVector3			m_Normal;
		// Vertex�� Normal��
		DXVector3			m_NormalVertex[3];
		// Vertex�� Tangent��
		DXVector3			m_TangentVertex[3];
		// Vertex�� UV��
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

		MaterialMap* m_DiffuseMap;			// DiffuseMap Data
		MaterialMap* m_BumpMap;				// BumpMap Data
		MaterialMap* m_SpecularMap;			// SpecularMap Data
		MaterialMap* m_ShineMap;				// ShineMap Data

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
		ParserData::CMaterial* m_MaterialData;		// Mesh Material Data

		/// Vertex Data
		std::vector<Vertex*>		m_MeshVertex;		// Vertex List
		std::vector<Face*>			m_MeshFace;			// Face List

		/// Animation Data
		ParserData::OneAnimation* m_Animation;		// Animation Data

		/// Skinning Data
		std::vector<DXMatrix4X4>	m_BoneTMList;		// Bone Offset TM List
		std::vector<Mesh*>			m_BoneMeshList;		// Bone Mesh List

		/// Final Data
		std::vector<Vertex*>		m_Final_Vertex;		/// ����ȭ �� Final Vertex List
		ParserData::IndexList* m_Final_Index;		/// ����ȭ �� Final Index List

	public:
		Mesh();
		~Mesh();
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

	class PARSER_DLL ASEMesh : public Mesh
	{
	public:
		int	m_Type;
		
		int	m_Mesh_NumVertex;
		int	m_Mesh_NumFaces;

		// �ؽ��Ŀ� �����͵�
		int	m_Mesh_NumTVertex;						// �ؽ��Ŀ� ���ؽ� ����
		int	m_Mesh_SumTVertex;						// �ؽ��Ŀ� ���ؽ� �� ����
		std::vector<TVertex*> m_Mesh_TVertex;		// �ؽ��Ŀ� ���ؽ�

		Mesh* m_Parent;								// Parent Mesh
		
		std::vector<Bone*> m_BoneList;
	};
}



