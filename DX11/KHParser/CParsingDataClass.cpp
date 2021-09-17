#include "DLLDefine.h"
#include "KHMath.h"

#include <vector>
#include <string>
using namespace std;

#include "AnimationData.h"
#include "CParsingDataClass.h"

namespace ParserData
{
	Vertex::Vertex()
	{
		m_Pos				= DXVector3::Zero();
		m_Normal			= DXVector3::Zero();
		m_isnormalset		= false;
		m_istextureset		= false;
		u					= 0.0f;
		v					= 0.0f;
	}

	Face::Face()
	{
		m_vertexindex[0] = 0;
		m_vertexindex[1] = 0;
		m_vertexindex[2] = 0;

		m_normal = DXVector3::Zero();
	}

	COneTVertex::COneTVertex()
	{
		m_u = 0;
		m_v = 0;
		m_w = 0;
	}

	ShapeLine::ShapeLine()
	{
		m_line_number		= 0;
		m_shape_closed		= false;
		m_shape_vertexcount = 0;	
	}

	Scenedata::Scenedata()
	{
		m_firstframe				= 0;
		m_lastframe					= 0;
		m_framespeed				= 0;
		m_ticksperframe				= 0;
		m_meshframestep				= 0;
		m_keyframestep				= 0;
		m_scene_background_static	= DXVector3::Zero();
		m_scene_ambient_static		= DXVector3::Zero();
	}

	Scenedata::~Scenedata()
	{

	}

	MaterialMap::MaterialMap()
	{
		m_subno					= 0;
		m_map_amount			= 0;
		m_uvw_u_offset			= 0;
		m_uvw_v_offset			= 0;
		m_uvw_u_tiling			= 0;
		m_uvw_v_tiling			= 0;
		m_uvw_angle				= 0;
		m_uvw_blur				= 0;
		m_uvw_blur_offset		= 0;
		m_uvw_noise_amt			= 0;
		m_uvw_noise_size		= 0;
		m_uvw_noise_level = 0;
		m_uvw_noise_phase = 0;
	}

	CMaterial::CMaterial()
	{
		m_materialnumber			= 0;	// Material의 번호. 이것을 기준으로 오브젝트에서 참조한다.
		m_material_ambient			= DXVector3::Zero();
		m_material_diffuse			= DXVector3::Zero();
		m_material_specular			= DXVector3::Zero();
		m_material_emissive			= DXVector3::Zero();
		m_material_shine			= 0.0f;
		m_material_shininess		= 0.0f;
		m_material_transparency		= 0.0f;
		m_material_reflectivity		= 0.0f;
		m_material_wiresize			= 0;
		m_material_xp_falloff		= 0.0f;
		m_material_selfillum		= 0.0f;

		m_numsubmtls				= 0;

		m_istextureexist			= false;
		m_isDiffuseMap				= false;
		m_isBumpMap					= false;
		m_isSpecularMap				= false;
		m_isShineMap				= false;
		m_isGenricMap				= false;
	}

	CMaterial::~CMaterial()
	{

	}

	Light_AnimationData::Light_AnimationData()
	{
		// Light Setting
		m_timevalue			= -1;// 이건 애니메이션이 없는경우의 셋팅이다. 그런데 시간이 -1인 경우가 있으면 어쩌지?
		m_light_color		= DXVector3::One();
		m_light_intens		= 0.0f;
		m_light_aspect		= 0.0f;
		m_light_hotspot		= 0.0f;		
		m_light_falloff		= 0.0f;		
		m_light_tdist		= 0.0f;
		m_light_mapbias		= 0.0f;
		m_light_maprange	= 0.0f;
		m_light_mapsize		= 0;
		m_light_raybias		= 0.0f;
	}

	Mesh::Mesh()
	{
		///----------------------------------
		/// Type, Check 변수
		///----------------------------------
		m_type				= 0;
		m_isparentexist		= false;					// 기본적으로 부모 노드는 존재치 않는다.

		///----------------------------------
		/// *NODE_TM (Transform Matrix)
		///----------------------------------
		m_inherit_pos		= DXVector3::Zero();
		m_inherit_rot		= DXVector3::Zero();
		m_inherit_scl		= DXVector3::Zero();
		m_tm_row0			= DXVector3::Zero();
		m_tm_row1			= DXVector3::Zero();
		m_tm_row2			= DXVector3::Zero();
		m_tm_row3			= DXVector3::Zero();
		m_tm_pos			= DXVector3::Zero();
		m_tm_rotaxis		= DXVector3::Zero();
		m_tm_rotangle		= 0.0f;
		m_tm_scale			= DXVector3::Zero();
		m_tm_scaleaxis		= DXVector3::Zero();
		m_tm_scaleaxisang	= 0.0f;

		///----------------------------------
		/// 추가 데이터
		///----------------------------------
		m_parent			= nullptr;
		m_materialdata		= nullptr;

		///----------------------------------
		/// *MESH
		///----------------------------------
		m_timevalue			= 0;
		m_numbone			= 0;				
		m_numskinweight		= 0;				
		m_mesh_numvertex	= 0;
		m_mesh_numfaces		= 0;
		m_IsHelper			= false;

		///----------------------------------
		/// *MESH_TVERTEX
		///----------------------------------
		m_mesh_numtvertex	= 0;
		m_mesh_sumtvertex	= 0;
		m_mesh_tvfaces		= 0;
		m_mesh_numcvertex	= 0;

		///----------------------------------
		/// Shape Object
		///----------------------------------
		m_shape_linecount	= 0;

		///----------------------------------
		/// 나머지 정보들
		///----------------------------------
		m_prop_motionblur	= 0;
		m_prop_castshadow	= 0;
		m_prop_recvshadow	= 0;
		m_material_ref		= 0;  
		m_wireframe_color	= DXVector3::Zero();

		///----------------------------------
		/// Animation
		///----------------------------------
		m_animation			= nullptr; 
		m_isAnimation		= false;
		///----------------------------------
		/// Camera
		///----------------------------------
		m_camera_near			= 0.0f;
		m_camera_far			= 0.0f;
		m_camera_fov			= 0.0f;
		m_camera_tdist			= 0.0f;

		m_camera_isloadTarget	= false;


		///----------------------------------
		/// Light
		///----------------------------------
		m_light_uselight		= false;
		m_light_useglobal		= false;
		m_light_absmapbias		= false;
		m_light_overshoot		= false;

		// Light Setting
		m_light_isloadTarget	= false;
		m_light_isAnimate		= false;


		///----------------------------------
		/// Skinning Object
		///----------------------------------
		m_IsSkinningObject		= false;
		m_IsBone				= false;
	}

	Mesh::~Mesh()
	{
		// 소멸자에서 생성되었던 메시데이터 (버텍스리스트, 페이스 리스트) 삭제해야함.
	}

}