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

		// ��ǥ���� ��ġ��
		DXVector3 m_Pos;

		// �븻��
		DXVector3 m_Normal;

		// �븻���� ���õƴ°�?
		bool m_isnormalset;

		// �ؽ��� ��ǥ
		float u, v;

		// ź��Ʈ
		DXVector3 m_Tanget;

		// �ؽ��İ��� ���õƴ°�?
		bool m_istextureset;

		/// Skinned Mesh ���� �߰�
		std::vector<float> m_boneWeights;
		std::vector<UINT> m_boneIndices;

		// �ε���
		int m_indices;
	};


	///--------------------------------------------------
	/// �� ���� Texture - Vertex
	///
	/// �̰��� Raw�������̸� ����ȭ�� �̷����� �ᱹ��
	/// COneVertex�� ���� �ȴ�.
	///--------------------------------------------------
	struct COneTVertex
	{
		COneTVertex();

		float m_u, m_v, m_w;
	};


	///--------------------------------------------------
	/// �� ���� Face
	///
	/// 3DSMAX�� ASE����� �״�� �������� ���� ���̴�.
	///
	/// �Ľ� �ʱ⿡ Face�� ������ ������ ������
	/// �ߺ��Ǵ� VertexNormal��, �ؽ��� �ڵ����Ʈ ����
	/// ���� �� �ִ�. �װ��� ������� �Ѵ�.
	///--------------------------------------------------
	struct Face
	{
		// �� Face�� �̷�� ���ؽ��� �ε���
		int				m_vertexindex[3];
		// Face�� Normal��
		DXVector3			m_normal;
		// Vertex�� Normal��
		DXVector3			m_normalvertex[3];
		// Vertex�� Tangent��
		DXVector3			m_tangentvertex[3];
		// Vertex�� UV��
		DXVector2			m_uvvertex[3];
		// Texture Coordinate
		int				m_TFace[3];

		// �ʱ�ȭ�� ���� ������.
		Face();
	};


	///--------------------------------------------------
	/// �� ���� Shape Vertex�� ������ �����ϱ� ���� ����ü
	///
	/// 3DSMAX�� ASE����� �״�� �������� ���� ���̴�.
	///--------------------------------------------------
	struct ShapeVertex
	{
		int				m_indexnum;			// �ε��� �ѹ�. �׳� �Ϸù�ȣ��
		bool				m_isknot;			// KNOT�ΰ� INTERP�ΰ�?
		DXVector3			m_pos;				// ������ ������
	};


	///--------------------------------------------------
	/// �� ���� Shape Object�� ������ �����ϱ� ���� ����ü
	///
	/// 3DSMAX�� ASE����� �״�� �������� ���� ���̴�.
	///--------------------------------------------------
	struct ShapeLine
	{
		int	m_line_number;		// ���° �����ΰ�?
		bool m_shape_closed;		// ���� Shape�ΰ�?
		int	m_shape_vertexcount;// �� ���� Vertex�� �ִ°�?
		std::vector<ShapeVertex*> m_shapevertex;	// ���ؽ����� vector

		// �ʱ�ȭ�� ���� ������
		ShapeLine();
	};


	/// <summary>
	/// �� ���� Scene�� �����ϱ� ���� ����ü
	/// ASE���Ͽ��� ���, �ڸ�Ʈ ���� ������ SCENE�κ��� �б� ���� ����ü.
	/// </summary>
	struct Scenedata
	{
	public:
		std::string			m_filename;					// ���� �̸�
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
	/// �� ���� Map�� �����ϱ� ���� Ŭ����
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
	/// �� ���� Material�� �����ϱ� ���� Ŭ����
	///--------------------------------------------------
	struct CMaterial
	{
		//--------------------
		//	��� ������
		//--------------------
	public:
		// ������ *MATERIAL_COUNT 0 �ϳ��ۿ� ������
		int						m_materialnumber;			// Material�� ��ȣ. �̰��� �������� ������Ʈ���� �����Ѵ�.
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
		CMaterial*			m_submaterial;				// �����Ʈ������ �����͸� ������ ����

		/// ���Ŀ� �������� Map�� �������� List�� �����ؾ� �ҵ�..
		std::vector<MaterialMap*> m_MapList;				// Mesh�� ����ִ� ��� Map ������

		MaterialMap*		m_DiffuseMap;			// DiffuseMap Data
		MaterialMap*		m_BumpMap;				// BumpMap Data
		MaterialMap*		m_SpecularMap;			// SpecularMap Data
		MaterialMap*		m_ShineMap;				// ShineMap Data
		MaterialMap*		m_GenricMap;			// ??

		bool				m_istextureexist;	// �ؽ��İ� �����ϴ°�?
		bool				m_isDiffuseMap;
		bool				m_isBumpMap;
		bool				m_isSpecularMap;
		bool				m_isShineMap;
		bool				m_isGenricMap;
		//--------------------
		//	Method
		//--------------------
	public:
		// ������ �Ҹ���
		CMaterial();
		~CMaterial();

	};


	// Light�� Animation ( Setting�� ��ȭ�� Animation )
	struct Light_AnimationData
	{
		// Light Setting
		int					m_timevalue;		// ���� mesh�� �ִµ�.. �̰� ��� ���� �̷����̸�.
		DXVector3			m_light_color;
		float				m_light_intens;
		float				m_light_aspect;
		float				m_light_hotspot;		///
		float				m_light_falloff;		/// �� �ΰ����� SpotLight�� ���
		float				m_light_tdist;
		float				m_light_mapbias;
		float				m_light_maprange;
		int					m_light_mapsize;
		float				m_light_raybias;

	public:
		Light_AnimationData();

	};

	// Bone �ϳ��� ����ü
	struct Bone
	{
		int			m_bone_number;		// ������ ���ͷ� ���������� ���߿� ������ �ʿ�, �˻��� �ʿ�?
		int			m_parent_bone_number;
		std::string	m_bone_name;
		
		bool m_isCol;

		DXVector3 m_Min;
		DXVector3 m_Max;

		DXMatrix4X4 m_WorldTM;	// Global Bind TM
		DXMatrix4X4 m_AniTM;
	};


	// bone�ϳ��� ����� ����ġ �� ��
	struct Weight
	{
		int			m_bone_number;		// weight�� ����� Bone�� IndexNumber;
		float		m_bone_weight;		// ����ġ
	};


	// Vertex �ϳ��� �־��� ����ġ���� �����ϴ� ����ü (����ü �ϳ��� ���ؽ� �ϳ��� �ϴ��� �����̴�)
	struct VertexWeight
	{
		int		m_wvertex_number;	// ���ؽ��� ��ȣ. ���� ���ͷ� �����ϹǷ� �� �� ������...

		// ���������� �Ʒ� ����Ʈ�� �ӽ� ����
		Weight* m_temp_bone_blend_weight;
		// ���ؽ� �ϳ��� ���� ���� Bone-Weight�� ���� �� �ִ�.
		std::vector<Weight*>	m_bone_blending_weight;	// bone�ϳ��� ����� ����ġ
	};



	///--------------------------------------------------
	/// �� ���� �޽ø� �Ľ� - �����ϱ� ���� Ŭ����
	/// ���� ���� �޽ð� ���� �� �ֱ� ������ ���� �� �κи� ����ü�� �������.
	///--------------------------------------------------
	
	struct Mesh
	{
	public:
		///----------------------------------
		/// Ÿ��, �ִϸ��̼��� �ִ°�? ���
		///----------------------------------
		int	m_type;					// Ÿ��
		Scenedata m_scenedata;		// (3DS Max��) �� ������

		///----------------------------------
		/// *NODE_NAME
		///----------------------------------
		std::string	m_nodename;		// � �������� ������ �Ʒ��� m_meshname�� ���� �̸��� �ѹ� �� ����. �Ƹ��� �޽��� �̸� ����Ʈ�����... �� �׷��� �ƴұ� ��ư ������ ���� 1�̹Ƿ� �������� ������ ����.
		std::string	m_nodeparent;	// �θ� ����� �̸�. �̰��� �ִٸ� �θ� �ִ� ���̴�.
		bool m_isparentexist;		// �׷��� Ȯ���� bool������ ������ ����.

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
		/// �߰� ������
		///----------------------------------
		DXMatrix4X4 m_worldTM;				// Mesh �� WorldTM
		DXMatrix4X4 m_localTM;				// Mesh �� LocalTM

		Mesh* m_parent;						// �θ��� ������
		std::vector<Mesh*> m_childlist;			// �ڽ��� ����Ʈ

		///----------------------------------
		/// *Material
		///----------------------------------
		ParserData::CMaterial* m_materialdata;	// �� �Ž��� �ش��ϴ� ��Ʈ����

		///----------------------------------
		/// *MESH
		///----------------------------------
		float m_timevalue;					// ���� ������ ���������� ���δ�. ��ư �ð���.
		int	m_numbone;						/// ���� ����. �̰��� �ִٸ� ��Ű�� ������Ʈ�� �����Ѵ�.
		int	m_numskinweight;				/// ��Ų ����ġ. ���Ҹ���?
		int	m_mesh_numvertex;				// (���ؽ��� ��ǥ������)
		int	m_mesh_numfaces;				// (���ؽ��� �ε���)
		bool m_IsHelper;					// Helper object

		// �޽� ���� vertex �迭�� �����ϱ� ���� ������
		/// *MESH_VERTEX_LIST
		std::vector<Vertex*>	m_meshvertex;		// ���ؽ�
		/// *MESH_FACE_LIST
		std::vector<Face*> m_meshface;			// �ε���

		// �ؽ��Ŀ� �����͵�
		/// *MESH_NUMTVERTEX
		int	m_mesh_numtvertex;						// �ؽ��Ŀ� ���ؽ� ����
		int	m_mesh_sumtvertex;						// �ؽ��Ŀ� ���ؽ� �� ����
		std::vector<COneTVertex*> m_mesh_tvertex;		// �ؽ��Ŀ� ���ؽ�

		/// *MESH_RVERTLIST
		int	m_mesh_tvfaces;			// �ؽ��Ŀ� ���̽� ���� - (������ ���� Face�� ���� �̹� ������ ��������Ƿ� ���븸 �־��ָ� �ȴ�)
		int	m_mesh_numcvertex;		// �� �̰� ��� ���°�?

		std::vector<Vertex*> m_opt_vertex;	/// ����ȭ ���� ���ؽ���
		IndexList* m_opt_index;			/// ����ȭ ���� �ε�����

		///----------------------------------
		/// Shape Object
		///----------------------------------
		int				m_shape_linecount;			// ������ ��
		std::vector<ShapeLine*>	m_vector_shape_line;	// ������ ������ ������


		/// ������ ������
		int				m_prop_motionblur;			//*PROP_MOTIONBLUR 0
		int				m_prop_castshadow;			//*PROP_CASTSHADOW 1
		int				m_prop_recvshadow;			//*PROP_RECVSHADOW 1
		int				m_material_ref;				//*MATERIAL_REF			// �� �޽��� ��Ʈ������ ��ȣ
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
		bool				m_camera_isloadTarget;	// ī�޶�� NodeTM�� �ΰ��� �ִµ�, ��ͱ��������� ������ ����� 
													// (�׻� ������ Target�� �о���� �� �ִ� �׷��� �װ��� ���ذ��� �Ѵ�)

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
		std::vector<Light_AnimationData*>	m_list_lightanimation;	// �װ��� Animation

		bool				m_light_isloadTarget;	// ���� NodeTM�� �� �� �ְ�, ī�޶�� ���� ������ �� ������ �ִ�.
													/// ��(�ƹ����� ���� ���� ������. �ٲ���.)
		bool				m_light_isAnimate;		// Animation�� �Ǵ°�?

		///----------------------------------
		/// Skinning Object
		///----------------------------------
		// SKIN_INITTM�� �����ϰ�
		// TM_ROW0~3
		// BONE_LIST
		bool					m_IsSkinningObject;	// ��Ű�� ������Ʈ�ΰ�?
		bool					m_IsBone;

		Bone* m_bone;									// Bone �Ѱ��� ������ (�ļ� ���ο��� �ӽ� �������� ����)
		std::vector<Bone*>			m_vector_bone_list;		// �̰� �׳� ��ȣ�� �̸� �����̴�. �� �ȿ� �����͸� �־���Ƶ� �����ڱ���(�����ϱ� ���ϰ� - �׷��� ������ �Ź� �̸����� �˻��� �ؾߵ�����. �ƴϸ� �ε����� ������ ��� �ð���� ������ �Ҽ���?)
		std::vector<DXMatrix4X4>	m_boneTMList;
		std::vector<Mesh*>			m_boneList;

	public:
		Mesh();
		~Mesh();
	};
}



