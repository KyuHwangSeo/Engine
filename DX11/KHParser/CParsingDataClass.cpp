#include "KHMath.h"
#include "ParserDLL.h"

#include <vector>
#include <string>
using namespace std;

#include "CParsingDataClass.h"

namespace ParserData
{
	Vertex::Vertex()
	{
		m_Pos				= DXVector3::Zero();
		m_Normal			= DXVector3::Zero();
		m_IsNormalSet		= false;
		m_IsTextureSet		= false;
		m_U					= 0.0f;
		m_V					= 0.0f;
	}

	Face::Face()
	{
		m_VertexIndex[0] = 0;
		m_VertexIndex[1] = 0;
		m_VertexIndex[2] = 0;

		m_Normal = DXVector3::Zero();
	}

	TVertex::TVertex()
	{
		m_U = 0;
		m_V = 0;
		m_W = 0;
	}

	Scenedata::Scenedata()
	{
		m_FirstFrame				= 0;
		m_LastFrame					= 0;
		m_FrameSpeed				= 0;
		m_TicksPerFrame				= 0;
	}

	Scenedata::~Scenedata()
	{

	}

	CMaterial::CMaterial()
	{
		m_MaterialNumber			= 0;	// Material�� ��ȣ. �̰��� �������� ������Ʈ���� �����Ѵ�.
		m_Material_Ambient			= DXVector3::Zero();
		m_Material_Diffuse			= DXVector3::Zero();
		m_Material_Specular			= DXVector3::Zero();
		m_Material_Emissive			= DXVector3::Zero();
		m_Material_Shininess		= 0.0f;
		m_Material_Transparency		= 0.0f;
		m_Material_Reflectivity		= 0.0f;

		m_IsDiffuseMap				= false;
		m_IsBumpMap					= false;
		m_IsSpecularMap				= false;
		m_IsShineMap				= false;
	}

	CMaterial::~CMaterial()
	{

	}

	Mesh::Mesh()
	{
		///----------------------------------
		/// *NODE_TM (Transform Matrix)
		///----------------------------------
		m_tm_row0			= DXVector3::Zero();
		m_tm_row1			= DXVector3::Zero();
		m_tm_row2			= DXVector3::Zero();
		m_tm_row3			= DXVector3::Zero();

		///----------------------------------
		/// �߰� ������
		///----------------------------------
		m_MaterialData		= nullptr;
		m_TopNode			= false;

		///----------------------------------
		/// Animation
		///----------------------------------
		m_Animation			= nullptr; 

		///----------------------------------
		/// Skinning Object
		///----------------------------------
		m_IsSkinningObject		= false;
		m_IsBone				= false;
	}

	Mesh::~Mesh()
	{
		// �Ҹ��ڿ��� �����Ǿ��� �޽õ����� (���ؽ�����Ʈ, ���̽� ����Ʈ) �����ؾ���.
	}

}