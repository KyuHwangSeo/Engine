/// <summary>
/// CParsingDataClass.h
#pragma once

namespace ParserData
{
	struct IndexList
	{
		int index[3];
	};

	///--------------------------------------------------
	/// Vertex
	///--------------------------------------------------
	struct Vertex
	{
		Vertex();

		// 좌표상의 위치값
		DXVector3 m_Pos;

		// 노말값
		DXVector3 m_Normal;

		// 노말값이 셋팅됐는가?
		bool m_isnormalset;

		// 텍스쳐 좌표
		float u, v;

		// 탄젠트
		DXVector3 m_Tanget;

		// 텍스쳐값이 셋팅됐는가?
		bool m_istextureset;

		/// Skinned Mesh 에서 추가
		std::vector<float> m_boneWeights;
		std::vector<UINT> m_boneIndices;

		// 인덱스
		int m_indices;
	};


	///--------------------------------------------------
	/// 한 개의 Texture - Vertex
	///
	/// 이것은 Raw데이터이며 최적화가 이뤄져서 결국은
	/// COneVertex에 들어가게 된다.
	///--------------------------------------------------
	struct COneTVertex
	{
		COneTVertex();

		float m_u, m_v, m_w;
	};


	///--------------------------------------------------
	/// 한 개의 Face
	///
	/// 3DSMAX의 ASE출력을 그대로 가져오기 위한 것이다.
	///
	/// 파싱 초기에 Face의 갯수는 정해져 있으며
	/// 중복되는 VertexNormal값, 텍스쳐 코디네이트 값이
	/// 있을 수 있다. 그것을 나눠줘야 한다.
	///--------------------------------------------------
	struct Face
	{
		// 이 Face를 이루는 버텍스의 인덱스
		int				m_vertexindex[3];
		// Face의 Normal값
		DXVector3			m_normal;
		// Vertex의 Normal값
		DXVector3			m_normalvertex[3];
		// Vertex의 Tangent값
		DXVector3			m_tangentvertex[3];
		// Vertex의 UV값
		DXVector2			m_uvvertex[3];
		// Texture Coordinate
		int				m_TFace[3];

		// 초기화를 위한 생성자.
		Face();
	};


	///--------------------------------------------------
	/// 한 개의 Shape Vertex의 정보를 저장하기 위한 구조체
	///
	/// 3DSMAX의 ASE출력을 그대로 가져오기 위한 것이다.
	///--------------------------------------------------
	struct ShapeVertex
	{
		int				m_indexnum;			// 인덱스 넘버. 그냥 일련번호다
		bool				m_isknot;			// KNOT인가 INTERP인가?
		DXVector3			m_pos;				// 포지션 데이터
	};


	///--------------------------------------------------
	/// 한 개의 Shape Object의 정보를 저장하기 위한 구조체
	///
	/// 3DSMAX의 ASE출력을 그대로 가져오기 위한 것이다.
	///--------------------------------------------------
	struct ShapeLine
	{
		int	m_line_number;		// 몇번째 라인인가?
		bool m_shape_closed;		// 닫힌 Shape인가?
		int	m_shape_vertexcount;// 몇 개의 Vertex가 있는가?
		std::vector<ShapeVertex*> m_shapevertex;	// 버텍스들의 vector

		// 초기화를 위한 생성자
		ShapeLine();
	};


	/// <summary>
	/// 한 개의 Scene을 저장하기 위한 구조체
	/// ASE파일에서 헤더, 코멘트 다음 나오는 SCENE부분을 읽기 위한 구조체.
	/// </summary>
	struct Scenedata
	{
	public:
		std::string			m_filename;					// 파일 이름
		int					m_firstframe;
		int					m_lastframe;
		int					m_framespeed;
		float				m_ticksperframe;
		int					m_meshframestep;
		int					m_keyframestep;
		DXVector3			m_scene_background_static;
		DXVector3			m_scene_ambient_static;

	public:
		Scenedata();
		~Scenedata();
	};


	///--------------------------------------------------
	/// 한 개의 Map을 저장하기 위한 클래스
	///--------------------------------------------------
	struct MaterialMap
	{
		std::string	m_map_name;
		std::string	m_map_class;
		int			m_subno;
		float		m_map_amount;
		std::string	m_bitmap;
		std::string	m_map_type;
		float		m_uvw_u_offset;
		float		m_uvw_v_offset;
		float		m_uvw_u_tiling;
		float		m_uvw_v_tiling;
		float		m_uvw_angle;
		float		m_uvw_blur;
		float		m_uvw_blur_offset;
		float		m_uvw_noise_amt;
		float		m_uvw_noise_size;
		float		m_uvw_noise_level;
		float		m_uvw_noise_phase;
		std::string	m_bitmap_filter;

		//
		MaterialMap();
	};


	///--------------------------------------------------
	/// 한 개의 Material을 저장하기 위한 클래스
	///--------------------------------------------------
	struct CMaterial
	{
		//--------------------
		//	멤버 변수들
		//--------------------
	public:
		// 지금은 *MATERIAL_COUNT 0 하나밖에 없더라
		int						m_materialnumber;			// Material의 번호. 이것을 기준으로 오브젝트에서 참조한다.
		std::string				m_material_name;
		std::wstring			m_material_key;
		std::string				m_material_class;
		DXVector3				m_material_ambient;
		DXVector3				m_material_diffuse;
		DXVector3				m_material_specular;
		DXVector3				m_material_emissive;
		float					m_material_shine;
		float					m_material_shininess;
		float					m_material_transparency;
		float					m_material_reflectivity;
		float					m_material_wiresize;
		std::string				m_material_shading;
		float					m_material_xp_falloff;
		float					m_material_selfillum;
		std::string				m_material_falloff;
		std::string				m_material_xp_type;

		int					m_numsubmtls;
		CMaterial*			m_submaterial;				// 서브매트리얼은 포인터만 가지고 있자

		/// 추후에 여러가지 Map을 받으려면 List로 관리해야 할듯..
		std::vector<MaterialMap*> m_MapList;				// Mesh에 들어있는 모든 Map 정보들

		MaterialMap*		m_DiffuseMap;			// DiffuseMap Data
		MaterialMap*		m_BumpMap;				// BumpMap Data
		MaterialMap*		m_SpecularMap;			// SpecularMap Data
		MaterialMap*		m_ShineMap;				// ShineMap Data
		MaterialMap*		m_GenricMap;			// ??

		bool				m_istextureexist;	// 텍스쳐가 존재하는가?
		bool				m_isDiffuseMap;
		bool				m_isBumpMap;
		bool				m_isSpecularMap;
		bool				m_isShineMap;
		bool				m_isGenricMap;
		//--------------------
		//	Method
		//--------------------
	public:
		// 생성자 소멸자
		CMaterial();
		~CMaterial();

	};


	// Light의 Animation ( Setting의 변화의 Animation )
	struct Light_AnimationData
	{
		// Light Setting
		int					m_timevalue;		// 역시 mesh에 있는데.. 이거 어떻게 하지 이런식이면.
		DXVector3			m_light_color;
		float				m_light_intens;
		float				m_light_aspect;
		float				m_light_hotspot;		///
		float				m_light_falloff;		/// 이 두가지는 SpotLight인 경우
		float				m_light_tdist;
		float				m_light_mapbias;
		float				m_light_maprange;
		int					m_light_mapsize;
		float				m_light_raybias;

	public:
		Light_AnimationData();

	};

	// Bone 하나의 구조체
	struct Bone
	{
		int			m_bone_number;		// 어차피 벡터로 잡을거지만 나중에 참조시 필요, 검색시 필요?
		int			m_parent_bone_number;
		std::string	m_bone_name;
		
		bool m_isCol;

		DXVector3 m_Min;
		DXVector3 m_Max;

		DXMatrix4X4 m_WorldTM;	// Global Bind TM
		DXMatrix4X4 m_AniTM;
	};


	// bone하나와 연결된 가중치 한 개
	struct Weight
	{
		int			m_bone_number;		// weight가 적용된 Bone의 IndexNumber;
		float		m_bone_weight;		// 가중치
	};


	// Vertex 하나에 주어진 가중치들을 포함하는 구조체 (구조체 하나가 버텍스 하나와 일대일 대응이다)
	struct VertexWeight
	{
		int		m_wvertex_number;	// 버텍스의 번호. 역시 벡터로 관리하므로 알 수 있지만...

		// 마찬가지로 아래 리스트의 임시 공간
		Weight* m_temp_bone_blend_weight;
		// 버텍스 하나는 여러 개의 Bone-Weight를 가질 수 있다.
		std::vector<Weight*>	m_bone_blending_weight;	// bone하나와 연결된 가중치
	};



	///--------------------------------------------------
	/// 한 개의 메시를 파싱 - 저장하기 위한 클래스
	/// 복수 개의 메시가 있을 수 있기 때문에 따로 이 부분만 구조체로 만들었다.
	///--------------------------------------------------
	
	struct Mesh
	{
	public:
		///----------------------------------
		/// 타입, 애니메이션이 있는가? 등등
		///----------------------------------
		int	m_type;					// 타입
		Scenedata m_scenedata;		// (3DS Max의) 씬 데이터

		///----------------------------------
		/// *NODE_NAME
		///----------------------------------
		std::string	m_nodename;		// 어떤 식인지는 모르지만 아래의 m_meshname과 같은 이름이 한번 더 들어간다. 아마도 메쉬의 이름 리스트라던지... 뭐 그런게 아닐까 여튼 지금은 버전 1이므로 이정도로 만족을 하자.
		std::string	m_nodeparent;	// 부모 노드의 이름. 이것이 있다면 부모가 있는 것이다.
		bool m_isparentexist;		// 그래도 확실히 bool형으로 가지고 있자.

		///----------------------------------
		/// *NODE_TM (Transform Matrix)
		///----------------------------------
		DXVector3	m_inherit_pos;
		DXVector3	m_inherit_rot;
		DXVector3	m_inherit_scl;
		DXVector3	m_tm_row0;
		DXVector3	m_tm_row1;
		DXVector3	m_tm_row2;
		DXVector3	m_tm_row3;
		DXVector3	m_tm_pos;
		DXVector3	m_tm_rotaxis;
		float m_tm_rotangle;
		DXVector3	m_tm_scale;
		DXVector3	m_tm_scaleaxis;
		float m_tm_scaleaxisang;

		///----------------------------------
		/// 추가 데이터
		///----------------------------------
		DXMatrix4X4 m_worldTM;				// Mesh 의 WorldTM
		DXMatrix4X4 m_localTM;				// Mesh 의 LocalTM

		Mesh* m_parent;						// 부모의 포인터
		std::vector<Mesh*> m_childlist;			// 자식의 리스트

		///----------------------------------
		/// *Material
		///----------------------------------
		ParserData::CMaterial* m_materialdata;	// 현 매쉬에 해당하는 메트리얼

		///----------------------------------
		/// *MESH
		///----------------------------------
		float m_timevalue;					// 여러 곳에서 여러가지로 쓰인다. 여튼 시간값.
		int	m_numbone;						/// 본의 갯수. 이것이 있다면 스키닝 오브젝트로 간주한다.
		int	m_numskinweight;				/// 스킨 가중치. 뭔소린지?
		int	m_mesh_numvertex;				// (버텍스의 좌표데이터)
		int	m_mesh_numfaces;				// (버텍스의 인덱스)
		bool m_IsHelper;					// Helper object

		// 메시 안의 vertex 배열을 선언하기 위한 포인터
		/// *MESH_VERTEX_LIST
		std::vector<Vertex*>	m_meshvertex;		// 버텍스
		/// *MESH_FACE_LIST
		std::vector<Face*> m_meshface;			// 인덱스

		// 텍스쳐용 데이터들
		/// *MESH_NUMTVERTEX
		int	m_mesh_numtvertex;						// 텍스쳐용 버텍스 갯수
		int	m_mesh_sumtvertex;						// 텍스쳐용 버텍스 총 갯수
		std::vector<COneTVertex*> m_mesh_tvertex;		// 텍스쳐용 버텍스

		/// *MESH_RVERTLIST
		int	m_mesh_tvfaces;			// 텍스쳐용 페이스 갯수 - (갯수는 위의 Face와 같고 이미 위에서 만들었으므로 내용만 넣어주면 된다)
		int	m_mesh_numcvertex;		// ※ 이건 어디에 쓰는것?

		std::vector<Vertex*> m_opt_vertex;	/// 최적화 후의 버텍스들
		IndexList* m_opt_index;			/// 최적화 후의 인덱스들

		///----------------------------------
		/// Shape Object
		///----------------------------------
		int				m_shape_linecount;			// 라인의 수
		std::vector<ShapeLine*>	m_vector_shape_line;	// 쉐이프 라인의 데이터


		/// 나머지 정보들
		int				m_prop_motionblur;			//*PROP_MOTIONBLUR 0
		int				m_prop_castshadow;			//*PROP_CASTSHADOW 1
		int				m_prop_recvshadow;			//*PROP_RECVSHADOW 1
		int				m_material_ref;				//*MATERIAL_REF			// 이 메쉬의 매트리얼의 번호
		DXVector3		m_wireframe_color;			//*WIREFRAME_COLOR 0.89803922	0.65098041	0.84313726

		///----------------------------------
		/// Animation
		///----------------------------------
		AnimationData*	m_animation;
		bool			m_isAnimation;

		///----------------------------------
		/// Camera
		///----------------------------------
		std::string			m_camera_type;
		float				m_camera_near;
		float				m_camera_far;
		float				m_camera_fov;
		float				m_camera_tdist;
		bool				m_camera_isloadTarget;	// 카메라는 NodeTM이 두개가 있는데, 재귀구조내에서 문제가 생긴다 
													// (항상 뒤쪽의 Target을 읽어버릴 수 있다 그래서 그것을 피해가야 한다)

		///----------------------------------
		/// Light
		///----------------------------------
		std::string			m_light_type;

		// Light Property
		std::string			m_light_shadows;
		bool				m_light_uselight;
		std::string			m_light_spotshape;
		bool				m_light_useglobal;
		bool				m_light_absmapbias;
		bool				m_light_overshoot;

		// Light Setting & Animation
		Light_AnimationData* m_lightanimation;		// Light Setting
		std::vector<Light_AnimationData*>	m_list_lightanimation;	// 그것의 Animation

		bool				m_light_isloadTarget;	// 빛도 NodeTM이 두 개 있고, 카메라와 같은 이유로 이 변수가 있다.
													/// ※(아무래도 좋지 않은 구조다. 바꾸자.)
		bool				m_light_isAnimate;		// Animation이 되는가?

		///----------------------------------
		/// Skinning Object
		///----------------------------------
		// SKIN_INITTM은 무시하고
		// TM_ROW0~3
		// BONE_LIST
		bool					m_IsSkinningObject;	// 스키닝 오브젝트인가?
		bool					m_IsBone;

		Bone* m_bone;									// Bone 한개의 포인터 (파서 내부에서 임시 공간으로 쓰임)
		std::vector<Bone*>			m_vector_bone_list;		// 이건 그냥 번호와 이름 관계이다. 이 안에 포인터를 넣어놓아도 괜찮겠구나(참조하기 편하게 - 그렇지 않으면 매번 이름으로 검색을 해야될지도. 아니면 인덱스가 있으니 상수 시간대로 접근을 할수도?)
		std::vector<DXMatrix4X4>	m_boneTMList;
		std::vector<Mesh*>			m_boneList;

	public:
		Mesh();
		~Mesh();
	};
}



