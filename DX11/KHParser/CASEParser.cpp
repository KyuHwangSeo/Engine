#include "DLLDefine.h"
#include "KHMath.h"

#include <vector>
#include <string>
using namespace std;

#include "AnimationData.h"
#include "CParsingDataClass.h"
#include "ASEFile.h"
#include "CASEParser.h"

using namespace ParserData;

CASEParser::CASEParser()
	: m_Token(0), m_parsingmode(eNone), m_data_asciiexport(0), m_materialcount(0),
	m_materialdata(nullptr), m_materialmap(nullptr), m_OneMesh(nullptr), m_nowshapeline(nullptr), m_nowshapevertex(nullptr), m_isAnimation(false), m_animation(nullptr), m_lexer(nullptr)
{
	m_lexer = new ASE::CASELexer;
}

CASEParser::~CASEParser()
{
	delete m_lexer;
}

bool CASEParser::Init()
{

	return TRUE;
}
//---------------------------------------------------------------------------------------------------
// �ε��Ѵ�.
// �̰��� �����ٸ� ������ ���������� ��� �����͸� �о �� �־�� �Ѵ�.
//
// Parsing�� ����:
// �׻� ������ �������� parsing�̶�� ���� �����Ͱ� ���� �԰ݿ� �°� �� �ִٴ� ���� ������ �Ѵ�.
// ������, ���� ���ο� ������ �ٲ�� �� �ִ°��� �����ϴٴ��� �ϴ� ��Ģ�� �����
// �˻��ϴ� ��ƾ�� ���������� ����. �ϴ��� ������� ���� �Ծ����� �� �ִٴ°��� ��������.
// -> �׷���, ��� ȣ���� �ϴ� �ϳ��� ū �Լ��� �ذ�ô�.
// -> depth�� �������� �����ۿ� ���� ���� �ڵ带 �־�߰ڴ�
//---------------------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	/// ������ �ε��Ѵ�.
	if (!m_lexer->Open(p_File))
	{
		TRACE("������ ���� �߿� ������ �߻��߽��ϴ�!");
		return FALSE;
	}

	/// ����� ����..
	DataParsing();

	// Optimize & Parent Set..
	OptimizeData();

	return TRUE;
}

/// 1�� ��ȯ

// CScenedata ����
bool CASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
	// �� ����
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// �޽��� ����ȭ�� �� �ش�.
/// �븻��, �ؽ��� ��ǥ�� ���� ���ؽ��� �ø���, ��ø�Ǵ°��� �����ϰ�..
/// </summary>
void CASEParser::OptimizeVertex(Mesh* pMesh)
{
	bool new_VertexSet = true;
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
			if (pMesh->m_mesh_numtvertex > 0 && nowVertex->m_istextureset == false)
			{
				nowVertex->u = pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_u;
				nowVertex->v = pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_v;
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

			if (pMesh->m_mesh_numtvertex > 0)
			{
				if ((pMesh->m_opt_vertex[vertexIndex]->u != pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_u) ||
					(pMesh->m_opt_vertex[vertexIndex]->v != pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_v))
				{
					new_VertexSet = true;
				}
			}

			if (new_VertexSet)
			{
				// �߰��� Vertex�� �ִٸ� üũ..
				if (resize_VertexIndex > pMesh->m_meshvertex.size())
				{
					for (unsigned int k = pMesh->m_meshvertex.size(); k < resize_VertexIndex; k++)
					{
						// ���� �߰��� Vertex�� ������ �����͸� �����ִ� Face ���� Vertex Index ����..
						if ((pMesh->m_opt_vertex[k]->m_indices == pMesh->m_meshface[i]->m_vertexindex[j]) &&
							(pMesh->m_opt_vertex[k]->m_Normal == pMesh->m_meshface[i]->m_normalvertex[j]))
						{
							if (pMesh->m_mesh_numtvertex > 0)
							{
								if ((pMesh->m_opt_vertex[k]->u == pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_u) &&
									(pMesh->m_opt_vertex[k]->v == pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_v))
								{
									pMesh->m_meshface[i]->m_vertexindex[j] = (int)k;
									new_VertexSet = false;
									break;
								}
							}
							else
							{
								pMesh->m_meshface[i]->m_vertexindex[j] = (int)k;
								new_VertexSet = false;
								break;
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
					newVertex->m_boneIndices = pMesh->m_meshvertex[nowVertex->m_indices]->m_boneIndices;
					newVertex->m_boneWeights = pMesh->m_meshvertex[nowVertex->m_indices]->m_boneWeights;
					newVertex->m_isnormalset = true;

					if (pMesh->m_mesh_numtvertex > 0)
					{
						newVertex->u = pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_u;
						newVertex->v = pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_v;
						newVertex->m_istextureset = true;
					}

					pMesh->m_opt_vertex.push_back(newVertex);
					pMesh->m_meshface[i]->m_vertexindex[j] = resize_VertexIndex;
					resize_VertexIndex++;
				}
			}
		}
	}

	// Tanget �� ����..
	for (int i = 0; i < pMesh->m_mesh_numfaces; i++)
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

		//DXVector3 binormal = (ep2 * uv1.x - ep1 * uv2.x) * den;
		//binormal.Normalize();

		// ���� ������ ���� �����Ͽ� ���̴����� ��հ��� ����ϵ��� ����..
		pMesh->m_opt_vertex[index0]->m_Tanget += tangent;
		pMesh->m_opt_vertex[index1]->m_Tanget += tangent;
		pMesh->m_opt_vertex[index2]->m_Tanget += tangent;
	}

	// �ε����� �׳� ����
	pMesh->m_opt_index = new IndexList[pMesh->m_meshface.size()];

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pMesh->m_opt_index[i].index[j] = pMesh->m_meshface[i]->m_vertexindex[j];
		}
	}
}

void CASEParser::RecombinationTM(ParserData::Mesh* pMesh)
{
	// WorldTM -> LocalTM ��ȯ..
	DXVector4 row0 = pMesh->m_tm_row0;
	DXVector4 row1 = pMesh->m_tm_row1;
	DXVector4 row2 = pMesh->m_tm_row2;
	DXVector4 row3 = pMesh->m_tm_row3;
	row3.w = 1;

	DXMatrix4X4 oMatrix(row0, row1, row2, row3);

	/// Negative Scale Check
	// 3D Max�� �۾��� Mirroring���� ���� Scale���� ������ ���� ��츦 Negative Scale �̶�� �Ѵ�
	// ó���� ������ ��� Normal ó���� Animation ó���ÿ� ������ �ȴ�
	// Rotaion�� ��Ÿ���� �� Row1 ~ Row3 �� ������ ���� �� ���� �ٸ� �� ���� �����Ͽ� �����̸� ���� �ݴ��̰� ����̸� ���� ��ġ�Ѵ�

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
		XMMatrixDecompose(&nodeScale, &nodetRot, &nodePos, oMatrix);

		// ������ ������..
		nodeScale *= -1;

		// �븻�� ������..
		for (auto& k : pMesh->m_opt_vertex)
		{
			k->m_Normal *= -1;
		}

		/// �ؽ��ĳ� ���� �����͵� ��ȯ�� ���־�� �ұ�..?

		// ��Ʈ���� ������..
		oMatrix = XMMatrixScalingFromVector(nodeScale) * XMMatrixRotationQuaternion(nodetRot) * XMMatrixTranslationFromVector(nodePos);
	}

	// WorldTM ������� ���Ѵ�..
	DXMatrix4X4 iMatrix = oMatrix.Inverse();

	// �������� �̵��ϱ����� WorldTM �� LocalTM ���� ����..
	pMesh->m_worldTM = oMatrix;

	// �θ� ��尡 ������� WorldTM �� �� LocalTM..
	// �θ� ��尡 ������� LocalTM �� ��������..
	if (pMesh->m_parent == nullptr)
	{
		pMesh->m_localTM = pMesh->m_worldTM;
	}
	else
	{
		DXMatrix4X4 piMatrix = pMesh->m_parent->m_worldTM;
		pMesh->m_localTM = pMesh->m_worldTM * piMatrix.Inverse();
	}

	for (unsigned int i = 0; i < pMesh->m_opt_vertex.size(); i++)
	{
		pMesh->m_opt_vertex[i]->m_Pos = XMVector3Transform(pMesh->m_opt_vertex[i]->m_Pos, iMatrix);
	}

	// �ִϸ��̼��� ������� ����� ���ص���..
	if (pMesh->m_isAnimation)
	{
		if (pMesh->m_animation->m_isPosAnimation)
		{
			pMesh->m_animation->m_lastPosFrame = (int)pMesh->m_animation->m_position.size() - 1;
		}
		if (pMesh->m_animation->m_isRotAnimation)
		{
			pMesh->m_animation->m_lastRotFrame = (int)pMesh->m_animation->m_rotation.size() - 1;
		}
		if (pMesh->m_animation->m_isScaleAnimation)
		{
			pMesh->m_animation->m_lastScaleFrame = (int)pMesh->m_animation->m_scale.size() - 1;
		}

		pMesh->m_animation->m_ticksperFrame = 1.0f / pMesh->m_scenedata.m_framespeed;
		pMesh->m_animation->m_totalFrame = pMesh->m_scenedata.m_lastframe - pMesh->m_scenedata.m_firstframe;
	}
}

void CASEParser::OptimizeData()
{
	// �θ� �ڽ� ����..
	for (unsigned int i = 0; i < m_MeshList.size(); i++)
	{
		for (unsigned int j = 0; j < m_MeshList.size(); j++)
		{
			if (i == j)
			{
				continue;
			}

			// ��� �̸��� �ٸ� ��� �θ� �̸��� ���ٸ� ��Ӱ���..
			if (m_MeshList[i]->m_nodename == m_MeshList[j]->m_nodeparent)
			{
				m_MeshList[j]->m_parent = m_MeshList[i];
				m_MeshList[i]->m_childlist.emplace_back(m_MeshList[j]);
			}
		}
	}

	// Optimize Data
	for (unsigned int i = 0; i < m_MeshList.size(); i++)
	{
		OptimizeVertex(m_MeshList[i]);
		RecombinationTM(m_MeshList[i]);
	}
}

void CASEParser::SetBoneTM(ParserData::Mesh* pMesh)
{
	for (size_t i = 0; i < pMesh->m_vector_bone_list.size(); i++)
	{
		for (size_t j = 0; j < m_MeshList.size(); j++)
		{
			if (m_MeshList[j]->m_nodename == pMesh->m_vector_bone_list[i]->m_bone_name)
			{
				// Mesh Node Name�� Bone Name�� ��ġ�Ұ�� Bone�� Mesh �����͸� �־�����..
				pMesh->m_boneList.emplace_back(m_MeshList[j]);

				// Bone Setting..
				m_MeshList[j]->m_IsBone = true;
			}

			// Bone List size�� �������� �� �� �ʿ䰡 ����..
			if (pMesh->m_boneList.size() == pMesh->m_vector_bone_list.size())
			{
				break;
			}
		}
	}

	// Mesh�� ���Ե��ִ� Bone���� OffsetTM�� ���ص���..
	for (size_t i = 0; i < pMesh->m_boneList.size(); i++)
	{
		// Bone�� WorldTM(NodeTM)
		DXMatrix4X4 boneNodeTM = pMesh->m_boneList[i]->m_worldTM;

		// Mesh�� WorldTM(NodeTM)
		DXMatrix4X4 skinNodeTM = pMesh->m_worldTM;

		// Bone NodeTM * Mesh NodeTM ����� -> Mesh ���� Bone�� OffsetTM
		DXMatrix4X4 boneoffsetTM = boneNodeTM * skinNodeTM.Inverse();

		// Bone OffsetTM ����� -> Bone ���� Mesh�� OffsetTM
		pMesh->m_boneTMList.emplace_back(boneoffsetTM.Inverse());
	}
}

ParserData::Mesh* CASEParser::GetMesh(int index)
{
	return m_MeshList[index];
}

void CASEParser::DataParsing()
{
	// ��� �����ٰ� ���� �̻� ������ ���� ���(����)�� üũ�ϱ� ���� ����
	int i = 0;

	// �ε��� üũ�� ����
	int index = 0;

	LONG nowtoken = NULL;

	// ������ ���������� ����ؼ� �б�..
	while (true)
	{
		// ���� ��ū Ȯ��..
		nowtoken = m_lexer->GetToken(m_TokenString);

		switch (nowtoken)
		{
		case TOKEND_BLOCK_START:
			break;
		case TOKENR_HELPER_CLASS:
			break;

			//--------------------
			// 3DSMAX_ASCIIEXPORT
			//--------------------

		case TOKENR_3DSMAX_ASCIIEXPORT:
			m_data_asciiexport = Parsing_NumberInt();
			break;

			//--------------------
			// COMMENT
			//--------------------

		case TOKENR_COMMENT:
			Parsing_String();
			//AfxMessageBox( m_TokenString, NULL, NULL);		/// �ӽ÷� �ڸ�Ʈ�� ����غ���
			break;

			///----------------------------------
			/// SCENE
			///----------------------------------

		case TOKENR_SCENE:
			//
			break;
		case TOKENR_SCENE_FILENAME:
			m_scenedata.m_filename = Parsing_String();
			break;
		case TOKENR_SCENE_FIRSTFRAME:
			m_scenedata.m_firstframe = Parsing_NumberInt();
			break;
		case TOKENR_SCENE_LASTFRAME:
			m_scenedata.m_lastframe = Parsing_NumberInt();
			break;
		case TOKENR_SCENE_FRAMESPEED:
			m_scenedata.m_framespeed = Parsing_NumberInt();
			break;
		case TOKENR_SCENE_TICKSPERFRAME:
			m_scenedata.m_ticksperframe = Parsing_NumberFloat();
			break;
		case TOKENR_SCENE_MESHFRAMESTEP:
			m_scenedata.m_meshframestep = Parsing_NumberInt();
			break;
		case TOKENR_SCENE_KEYFRAMESTEP:
			m_scenedata.m_keyframestep = Parsing_NumberInt();
			break;
		case TOKENR_SCENE_BACKGROUND_STATIC:
			m_scenedata.m_scene_background_static = Parsing_ChangeNumberVector3();
			break;
		case TOKENR_SCENE_AMBIENT_STATIC:
			m_scenedata.m_scene_ambient_static = Parsing_ChangeNumberVector3();
			break;

		case TOKENR_SCENE_ENVMAP:
		{
			// �׳� ���� ������ �о������ }�� ���ö�����
			while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END) {
			}
		}
		break;

		///----------------------------------
		/// MATERIAL_LIST
		///----------------------------------

		case TOKENR_MATERIAL:
			Create_MaterialData_to_list();
			m_materialdata->m_materialnumber = Parsing_NumberInt();
			break;
		case TOKENR_MAP_DIFFUSE:
			Create_DiffuseMap_to_list();
			break;
		case TOKENR_MAP_BUMP:
			Create_BumpMap_to_list();
			break;
		case TOKENR_MAP_SPECULAR:
			Create_SpecularMap_to_list();
			break;
		case TOKENR_MAP_SHINE:
			Create_ShineMap_to_list();
			break;
		case TOKENR_MAP_GENERIC:
			//Create_materialmap_to_list();
			break;

		///----------------------------------
		/// Material
		///----------------------------------
		
		case TOKENR_MATERIAL_COUNT:
			m_materialcount = Parsing_NumberInt();
			break;
		case TOKENR_MATERIAL_NAME:
		{
			string fileName = Parsing_String();
			m_materialdata->m_material_name = fileName;
			m_materialdata->m_material_key = wstring(fileName.c_str(), fileName.c_str() + strlen(fileName.c_str()));
		}
			break;
		case TOKENR_MATERIAL_CLASS:
			m_materialdata->m_material_class = Parsing_String();
			break;
		case TOKENR_MATERIAL_AMBIENT:
			m_materialdata->m_material_ambient = Parsing_ChangeNumberVector3();
			break;
		case TOKENR_MATERIAL_DIFFUSE:
			m_materialdata->m_material_diffuse = Parsing_ChangeNumberVector3();
			break;
		case TOKENR_MATERIAL_SPECULAR:
			m_materialdata->m_material_specular = Parsing_ChangeNumberVector3();
			break;
		case TOKENR_MATERIAL_SHINE:
			m_materialdata->m_material_shine = Parsing_NumberFloat();
			break;
		case TOKENR_MATERIAL_SHINESTRENGTH:
			m_materialdata->m_material_shininess = Parsing_NumberFloat();
			break;
		case TOKENR_MATERIAL_TRANSPARENCY:
			m_materialdata->m_material_transparency = Parsing_NumberFloat();
			break;
		case TOKENR_MATERIAL_WIRESIZE:
			m_materialdata->m_material_wiresize = Parsing_NumberFloat();
			break;
		case TOKENR_MATERIAL_SHADING:
			m_materialdata->m_material_shading = Parsing_String();
			break;
		case TOKENR_MATERIAL_XP_FALLOFF:
			m_materialdata->m_material_xp_falloff = Parsing_NumberFloat();
			break;
		case TOKENR_MATERIAL_SELFILLUM:
			m_materialdata->m_material_selfillum = Parsing_NumberFloat();
			break;
		case TOKENR_MATERIAL_FALLOFF:
			m_materialdata->m_material_falloff = Parsing_String();
		break;	
		case TOKENR_MATERIAL_XP_TYPE:
			m_materialdata->m_material_xp_type = Parsing_String();
			break;

		///----------------------------------
		/// Material Map
		///----------------------------------
		
		case TOKENR_MAP_NAME:
			m_materialmap->m_map_name = Parsing_String();
			break;
		case TOKENR_MAP_CLASS:
			m_materialmap->m_map_class = Parsing_String();
			break;
		case TOKENR_MAP_SUBNO:
			m_materialmap->m_subno = Parsing_NumberInt();
			break;
		case TOKENR_MAP_AMOUNT:
			m_materialmap->m_map_amount = Parsing_NumberFloat();
			break;
		case TOKENR_BITMAP:
		{
			string filePath = Parsing_String();

			size_t indexSlash = filePath.rfind("\\") + 1;
			size_t indexDot = filePath.rfind(".");

			// ���� �ӽð��..
			string fileRoute = "../Resource/Textures/";

			string fileName = filePath.substr(indexSlash, indexDot - indexSlash);
			fileRoute += filePath.substr(indexSlash, filePath.size() - indexSlash);
			m_materialmap->m_bitmap = fileRoute;
		}
			break;
		case TOKENR_MAP_TYPE:
			m_materialmap->m_map_type = Parsing_String();
			break;
		case TOKENR_UVW_U_OFFSET:
			m_materialmap->m_uvw_u_offset = Parsing_NumberFloat();
			break;
		case TOKENR_UVW_V_OFFSET:
			m_materialmap->m_uvw_v_offset = Parsing_NumberFloat();
			break;
		case TOKENR_UVW_U_TILING:
			m_materialmap->m_uvw_u_tiling = Parsing_NumberFloat();
			break;
		case TOKENR_UVW_V_TILING:
			m_materialmap->m_uvw_v_tiling = Parsing_NumberFloat();
			break;
		case TOKENR_UVW_ANGLE:
			m_materialmap->m_uvw_angle = Parsing_NumberFloat();
			break;
		case TOKENR_UVW_BLUR:
			m_materialmap->m_uvw_blur = Parsing_NumberFloat();
			break;
		case TOKENR_UVW_BLUR_OFFSET:
			m_materialmap->m_uvw_blur_offset = Parsing_NumberFloat();
			break;
		case TOKENR_UVW_NOUSE_AMT:
			m_materialmap->m_uvw_noise_amt = Parsing_NumberFloat();
			break;
		case TOKENR_UVW_NOISE_SIZE:
			m_materialmap->m_uvw_noise_size = Parsing_NumberFloat();
			break;
		case TOKENR_UVW_NOISE_LEVEL:
			m_materialmap->m_uvw_noise_level = Parsing_NumberFloat();
			break;
		case TOKENR_UVW_NOISE_PHASE:
			m_materialmap->m_uvw_noise_phase = Parsing_NumberFloat();
			break;
		case TOKENR_BITMAP_FILTER:
			m_materialmap->m_bitmap_filter = Parsing_String();
			break;

			//--------------------
			// GEOMOBJECT
			//--------------------

		case TOKENR_GROUP:
			//	�� ���� �׷� ����
			break;

		case TOKENR_HELPEROBJECT:
			Create_OneMesh_to_list();
			m_OneMesh->m_type = eHelperObject;
			m_OneMesh->m_IsHelper = true;
			break;

		case TOKENR_SHAPEOBJECT:
			Create_OneMesh_to_list();
			m_OneMesh->m_type = eShape;
			break;

		case TOKENR_GEOMOBJECT:
			Create_OneMesh_to_list();
			m_OneMesh->m_type = eGeomobject;
			break;
		case TOKENR_LIGHTOBJECT:
			Create_OneMesh_to_list();
			m_OneMesh->m_type = eLight;
			break;
		case TOKENR_CAMERAOBJECT:
			Create_OneMesh_to_list();
			m_OneMesh->m_type = eCamera;
			break;

		case TOKENR_NODE_NAME:
			// ��� ������ ������Ʈ���� ���� �� �� �ִ� ������ ���̴�.
			// ��忡 ���� �־�� �� ���� �ٸ���.
		{
			if (m_OneMesh->m_type == eAnimation)
			{
				m_OneMesh->m_animation->m_nodename = Parsing_String();
			}
			else
			{
				m_OneMesh->m_nodename = Parsing_String();
			}
		}
		break;

		case TOKENR_NODE_PARENT:
			// �� ����� �θ� ����� ����.
			// �ϴ� �Է��� �ϰ�, ���߿� ��������.
			m_OneMesh->m_nodeparent = Parsing_String();
			break;

			///----------------------------------
			/// NODE_TM
			///----------------------------------

		case TOKENR_NODE_TM:
			break;

		// Camera, Light�� NodeTM�� �ι� �����Ƿ�
		// �ι�°�� �ϴ� ��������..

		case TOKENR_INHERIT_POS:
			if (m_OneMesh->m_inherit_pos == DXVector3::Zero())
				m_OneMesh->m_inherit_pos = Parsing_ChangeNumberVector3();

			break;
		case TOKENR_INHERIT_ROT:
			if (m_OneMesh->m_inherit_rot == DXVector3::Zero())
				m_OneMesh->m_inherit_rot = Parsing_ChangeNumberVector3();
			break;
		case TOKENR_INHERIT_SCL:
			if (m_OneMesh->m_inherit_scl == DXVector3::Zero())
				m_OneMesh->m_inherit_scl = Parsing_ChangeNumberVector3();
			break;
		case TOKENR_TM_ROW0:
			if (m_OneMesh->m_tm_row0 == DXVector3::Zero())
				m_OneMesh->m_tm_row0 = Parsing_ChangeNumberVector3();
			break;
		case TOKENR_TM_ROW1:
			if (m_OneMesh->m_tm_row2 == DXVector3::Zero())
				m_OneMesh->m_tm_row2 = Parsing_ChangeNumberVector3();
			break;
		case TOKENR_TM_ROW2:
			if (m_OneMesh->m_tm_row1 == DXVector3::Zero())
				m_OneMesh->m_tm_row1 = Parsing_ChangeNumberVector3();
			break;
		case TOKENR_TM_ROW3:
			if (m_OneMesh->m_tm_row3 == DXVector3::Zero())
				m_OneMesh->m_tm_row3 = Parsing_ChangeNumberVector3();
			break;
		case TOKENR_TM_POS:
			if (m_OneMesh->m_tm_pos == DXVector3::Zero())
				m_OneMesh->m_tm_pos = Parsing_ChangeNumberVector3();
			break;
		case TOKENR_TM_ROTAXIS:
			if (m_OneMesh->m_tm_rotaxis == DXVector3::Zero())
				m_OneMesh->m_tm_rotaxis = Parsing_ChangeNumberVector3();
			break;
		case TOKENR_TM_ROTANGLE:
			if (m_OneMesh->m_tm_rotangle == 0)
				m_OneMesh->m_tm_rotangle = Parsing_NumberFloat();
			break;
		case TOKENR_TM_SCALE:
			if (m_OneMesh->m_tm_scale == DXVector3::Zero())
				m_OneMesh->m_tm_scale = Parsing_ChangeNumberVector3();
			break;
		case TOKENR_TM_SCALEAXIS:
			if (m_OneMesh->m_tm_scaleaxis == DXVector3::Zero())
				m_OneMesh->m_tm_scaleaxis = Parsing_ChangeNumberVector3();
			break;
		case TOKENR_TM_SCALEAXISANG:
			// ���� ī�޶� ���¿��ٸ� �̹� ��带 ���� ������ ǥ�����ش�.
			if (m_OneMesh->m_tm_scaleaxisang == 0)
				m_OneMesh->m_tm_scaleaxisang = Parsing_NumberFloat();
			break;

			///----------------------------------
			/// Light
			///----------------------------------

		case TOKENR_LIGHT_TYPE:
			m_OneMesh->m_light_type = Parsing_String();
			break;
		case TOKENR_LIGHT_SHADOWS:
			m_OneMesh->m_light_shadows = Parsing_String();
			break;
		case TOKENR_LIGHT_USELIGHT:
			m_OneMesh->m_light_uselight = Parsing_NumberInt();
			break;
		case TOKENR_LIGHT_SPOTSHAPE:
			m_OneMesh->m_light_spotshape = Parsing_String();
			break;
		case TOKENR_LIGHT_USEGLOBAL:
			m_OneMesh->m_light_useglobal = Parsing_NumberInt();
			break;
		case TOKENR_LIGHT_ABSMAPBIAS:
			m_OneMesh->m_light_absmapbias = Parsing_NumberInt();
			break;
		case TOKENR_LIGHT_OVERSHOOT:
			m_OneMesh->m_light_overshoot = Parsing_NumberInt();
			break;
		case TOKENR_LIGHT_SETTINGS:
			Create_LightData_to_list();
			break;
		case TOKENR_LIGHT_COLOR:
			m_OneMesh->m_lightanimation->m_light_color = Parsing_NormalNumberVector3();
			break;
		case TOKENR_LIGHT_INTENS:
			m_OneMesh->m_lightanimation->m_light_intens = Parsing_NumberFloat();
			break;
		case TOKENR_LIGHT_ASPECT:
			m_OneMesh->m_lightanimation->m_light_aspect = Parsing_NumberFloat();
			break;
		case TOKENR_LIGHT_HOTSPOT:
			m_OneMesh->m_lightanimation->m_light_hotspot = Parsing_NumberFloat();
			break;
		case TOKENR_LIGHT_FALLOFF:
			m_OneMesh->m_lightanimation->m_light_falloff = Parsing_NumberFloat();
			break;
		case TOKENR_LIGHT_TDIST:
			m_OneMesh->m_lightanimation->m_light_tdist = Parsing_NumberFloat();
			break;
		case TOKENR_LIGHT_MAPBIAS:
			m_OneMesh->m_lightanimation->m_light_mapbias = Parsing_NumberFloat();
			break;
		case TOKENR_LIGHT_MAPRANGE:
			m_OneMesh->m_lightanimation->m_light_maprange = Parsing_NumberFloat();
			break;
		case TOKENR_LIGHT_MAPSIZE:
			m_OneMesh->m_lightanimation->m_light_mapsize = Parsing_NumberInt();
			break;
		case TOKENR_LIGHT_RAYBIAS:
			m_OneMesh->m_lightanimation->m_light_raybias = Parsing_NumberFloat();
			break;

			///----------------------------------
			/// Camera
			///----------------------------------

		case TOKENR_CAMERA_TYPE:
			m_OneMesh->m_camera_type = Parsing_String();
			break;
		case TOKENR_CAMERA_SETTINGS:
			break;
		case TOKENR_CAMERA_NEAR:
			m_OneMesh->m_camera_near = Parsing_NumberFloat();
			break;
		case TOKENR_CAMERA_FAR:
			m_OneMesh->m_camera_far = Parsing_NumberFloat();
			break;
		case TOKENR_CAMERA_FOV:
			m_OneMesh->m_camera_fov = Parsing_NumberFloat();
			break;
		case TOKENR_CAMERA_TDIST:
			m_OneMesh->m_camera_tdist = Parsing_NumberFloat();
			break;

			///----------------------------------
			/// Bone
			///----------------------------------
		case TOKENR_MESH_NUMBONE:
		{
			// �̰� �ִٸ� �̰��� Skinned Mesh��� ������ ���´�.
			m_OneMesh->m_numbone = Parsing_NumberInt();

			if (m_OneMesh->m_numbone > 0)
			{
				m_OneMesh->m_IsSkinningObject = true;
			}
		}
		break;
		case TOKENR_MESH_NUMSKINWEIGHT:
			break;

		case TOKENR_SKIN_INITTM:
			break;
		case TOKENR_BONE_PROPERTY:
			// �� ���� ABSOLUTE�� ������� �ϴµ�, �� ���� ����.
			break;
		case TOKENR_BONE_LIST:
			break;
		case TOKENR_BONE:
		{
			Create_BoneData_to_list();
			m_OneMesh->m_bone->m_bone_number = Parsing_NumberInt();
		}
		break;
		case TOKENR_BONE_NAME:
			m_OneMesh->m_bone->m_bone_name = Parsing_String();

		case TOKENR_MESH_WVERTEXS:
			break;

		case TOKENR_MESH_WEIGHT:
			index = Parsing_NumberInt();
			break;
		case TOKENR_BONE_BLENGING_WEIGHT:
		{
			int boneNum = Parsing_NumberInt();
			float boneWeight = Parsing_NumberFloat();

			m_OneMesh->m_meshvertex[index]->m_boneIndices.push_back(boneNum);
			m_OneMesh->m_meshvertex[index]->m_boneWeights.push_back(boneWeight);
		}
		break;

		///----------------------------------
		/// MESH
		///----------------------------------

		case TOKENR_MESH:
			//
			break;
		case TOKENR_TIMEVALUE:
		{
			/// TIMEVALUE�� ���������� ���Ǵµ� ��� ó���ؾ��ұ�..
			if (m_OneMesh->m_type == eLight)
			{
				m_OneMesh->m_lightanimation->m_timevalue = Parsing_NumberInt();
			}
			else
			{
				m_OneMesh->m_timevalue = Parsing_NumberFloat();
			}
		}
		break;

		case TOKENR_MESH_NUMVERTEX:
			// Vertex �� ���� ����..
			m_OneMesh->m_mesh_numvertex = Parsing_NumberInt();
			break;

			/// MESH_VERTEX_LIST

		case TOKENR_MESH_VERTEX_LIST:
			// ������ �־�� Vertex ������ŭ �����ص���..
		{
			for (int i = 0; i < m_OneMesh->m_mesh_numvertex; i++)
			{
				m_OneMesh->m_meshvertex.push_back(new Vertex);
			}
		}
		break;
		case TOKENR_MESH_VERTEX:
		{
			index = Parsing_NumberInt();
			m_OneMesh->m_meshvertex[index]->m_indices = index;
			m_OneMesh->m_meshvertex[index]->m_Pos = Parsing_ChangeNumberVector3();
		}
		break;

		case TOKENR_MESH_NUMFACES:
			// Face �� ���� ����..
			m_OneMesh->m_mesh_numfaces = Parsing_NumberInt();
			break;

			/// MESH_FACE_LIST
		case TOKENR_MESH_FACE_LIST:
			// ������ �־�� Face ������ŭ �����ص���..
			for (int i = 0; i < m_OneMesh->m_mesh_numfaces; i++)
			{
				m_OneMesh->m_meshface.push_back(new Face);
			}
			break;
		case TOKENR_MESH_FACE:
		{
			index = Parsing_NumberInt();

			Parsing_String();
			m_OneMesh->m_meshface[index]->m_vertexindex[0] = Parsing_NumberInt();
			Parsing_String();
			m_OneMesh->m_meshface[index]->m_vertexindex[2] = Parsing_NumberInt();
			Parsing_String();
			m_OneMesh->m_meshface[index]->m_vertexindex[1] = Parsing_NumberInt();
		}
		break;

		case TOKENR_MESH_NUMCVFACES:
			break;
		case TOKENR_MESH_CFACELIST:
			break;
		case TOKENR_MESH_CFACE:
			break;
		case TOKENR_MESH_NORMALS:
			break;

		case TOKENR_MESH_FACENORMAL:
		{
			index = Parsing_NumberInt();

			m_OneMesh->m_meshface[index]->m_normal = Parsing_ChangeNumberVector3();
			// ���� ��ū
			m_lexer->GetToken(m_TokenString);
			Parsing_NumberInt();
			m_OneMesh->m_meshface[index]->m_normalvertex[0] = Parsing_ChangeNumberVector3();
			// ���� ��ū
			m_lexer->GetToken(m_TokenString);
			Parsing_NumberInt();
			m_OneMesh->m_meshface[index]->m_normalvertex[2] = Parsing_ChangeNumberVector3();
			// ���� ��ū
			m_lexer->GetToken(m_TokenString);
			Parsing_NumberInt();
			m_OneMesh->m_meshface[index]->m_normalvertex[1] = Parsing_ChangeNumberVector3();
		}
		break;

		case TOKENR_MESH_VERTEXNORMAL:
			break;

			///----------------------------------
			/// MESH_TVERTLIST
			///----------------------------------

		case TOKENR_MESH_NUMTVERTEX:
			/// ������ ��� TVERTEX�� ������ �����Ƿ� ������������..
		{
			m_OneMesh->m_mesh_sumtvertex = m_OneMesh->m_mesh_numtvertex;
			m_OneMesh->m_mesh_numtvertex += Parsing_NumberInt();
		}
		break;

		case TOKENR_MESH_TVERTLIST:
			// �߰��� tVertex ������ŭ ����..
			for (int i = m_OneMesh->m_mesh_sumtvertex; i < m_OneMesh->m_mesh_numtvertex; i++)
			{
				m_OneMesh->m_mesh_tvertex.push_back(new ParserData::COneTVertex);
			}
			break;
		case TOKENR_MESH_TVERT:
		{
			/// ���� ���� �ε��� üũ.. 
			index = Parsing_NumberInt() + m_OneMesh->m_mesh_sumtvertex;

			m_OneMesh->m_mesh_tvertex[index]->m_u = Parsing_NumberFloat();
			m_OneMesh->m_mesh_tvertex[index]->m_v = 1.0f - Parsing_NumberFloat();
			m_OneMesh->m_mesh_tvertex[index]->m_w = Parsing_NumberFloat();
		}
		break;
		case TOKENR_MESH_NUMTVFACES:
			m_OneMesh->m_mesh_tvfaces += Parsing_NumberInt();
			break;
		case TOKENR_MESH_TFACELIST:
			break;
		case TOKENR_MESH_TFACE:
		{
			index = Parsing_NumberInt();

			m_OneMesh->m_meshface[index]->m_TFace[0] = Parsing_NumberInt();
			m_OneMesh->m_meshface[index]->m_TFace[2] = Parsing_NumberInt();
			m_OneMesh->m_meshface[index]->m_TFace[1] = Parsing_NumberInt();
		}
		break;

			///----------------------------------
			/// Animation
			///----------------------------------

		case TOKENR_TM_ANIMATION:
			Create_AnimationData_to_mesh(m_OneMesh);
			m_OneMesh->m_type = eAnimation;
			m_OneMesh->m_isAnimation = true;
			break;
		case TOKENR_CONTROL_POS_TRACK:
			m_OneMesh->m_animation->m_isPosAnimation = true;
			break;
		case TOKENR_CONTROL_POS_SAMPLE:
		{
			m_animation->m_position.push_back(new CAnimation_pos);
			m_animation->m_position.back()->m_time = (Parsing_NumberFloat() / m_OneMesh->m_scenedata.m_ticksperframe) - m_OneMesh->m_scenedata.m_firstframe;
			m_animation->m_position.back()->m_pos = Parsing_ChangeNumberVector3();
			break;
		}
		case TOKENR_CONTROL_ROT_TRACK:
			m_OneMesh->m_animation->m_isRotAnimation = true;
			break;
		case TOKENR_CONTROL_ROT_SAMPLE:
		{
			float _time = 0;
			float _angle = 0;
			Vector3 _rot = Vector3();
			m_animation->m_rotation.push_back(new CAnimation_rot);
			_time = (Parsing_NumberFloat() / m_OneMesh->m_scenedata.m_ticksperframe) - m_OneMesh->m_scenedata.m_firstframe;
			_rot = Parsing_ChangeNumberVector3();
			_angle = Parsing_NumberFloat();

			m_animation->m_rotation.back()->m_time = _time;
			m_animation->m_rotation.back()->m_rot = _rot;
			m_animation->m_rotation.back()->m_angle = _angle;

			Quaternion nowQT = XMQuaternionRotationAxis(_rot, _angle);

			// quaternion ���� ������������..
			if (m_animation->m_rotation.size() > 1)
			{
				m_animation->m_rotation.back()->m_rotQT_accumulation = XMQuaternionMultiply(m_animation->m_rotation[m_animation->m_rotation.size() - 2]->m_rotQT_accumulation, nowQT);
			}
			else
			{
				m_animation->m_rotation.back()->m_rotQT_accumulation = nowQT;
			}
			break;
		}
		case TOKENR_CONTROL_SCALE_TRACK:
			m_OneMesh->m_animation->m_isScaleAnimation = true;
			break;
		case TOKENR_CONTROL_SCALE_SAMPLE:
		{
			//m_animation->m_scale.push_back(new CAnimation_scl);
			//m_animation->m_scale.back()->m_time = (Parsing_NumberFloat() / m_OneMesh->m_scenedata.m_ticksperframe) - m_OneMesh->m_scenedata.m_firstframe;
			//m_animation->m_scale.back()->m_scale = Parsing_ChangeNumberVector3();
			//m_animation->m_scale.back()->m_scaleaxis = Parsing_ChangeNumberVector3();
			//m_animation->m_scale.back()->m_scaleaxisang = Parsing_NumberFloat();
			break;
		}
		case TOKENR_MATERIAL_REF:
		{
			m_OneMesh->m_material_ref = Parsing_NumberInt();

			// Mesh�� �ش��ϴ� Material Data ����
			m_OneMesh->m_materialdata = m_list_materialdata[m_OneMesh->m_material_ref];
		}
			break;

		case TOKENR_WIREFRAME_COLOR:
			break;
		case TOKEND_END:
			// �Ƹ��� �̰� ������ ���� ��Ÿ�������ΰ� ������. while�� Ż���ؾ� �ϴµ�?

			//AfxMessageBox("������ ���� �� �� �����ϴ�!", MB_OK, NULL);
			TRACE("TRACE: �Ľ���: ������ ���� �ý��ϴ�!\n");
			return;

			break;

			/// ���� �ƹ��͵� �ش����� ������
		default:
			// �ƹ��͵� ���� �ʴ´�.
			break;

		}	// switch()


		///-----------------------------------------------
		/// ���� �ڵ�.
		i++;
		if (i > 1000000)
		{
			// ������ 1000000���̻��̳� �� ������ ����. (�����Ͱ� 100000���� �ƴ��̻�)
			// ���� 1000000�̻� ���Ҵٸ� Ȯ���� ���� ������ �ִ� ���̹Ƿ�
			TRACE("������ �鸸�� ���ҽ��ϴ�!");
			return;
		}
		/// ���� �ڵ�.
		///-----------------------------------------------

	}		// while()

	return;
}


///----------------------------------------------------------------------
/// parsing�� ���� ������ �Լ���
///----------------------------------------------------------------------

// long�� �о �������ش�.
int CASEParser::Parsing_NumberLong()
{
	LONG			token;
	LONG			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = strtoul(m_TokenString, NULL, 10);

	return			tempNumber;
}

// float
float CASEParser::Parsing_NumberFloat()
{
	LONG			token;
	float			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (float)atof(m_TokenString);

	return			tempNumber;
}

// String
LPSTR CASEParser::Parsing_String()
{
	/// ��m_TokenString ( char[255] ) �̱� ������ CString�� ������ ���� ���Ŷ� �����ߴµ�, �������� CString�� �� ������� �� ����. �˾Ƽ� �޾Ƶ��̴µ�?
	m_lexer->GetToken(m_TokenString);

	return m_TokenString;
}


// int
int CASEParser::Parsing_NumberInt() {

	LONG			token;
	int				tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (int)atoi(m_TokenString);

	return			tempNumber;
}


// 3���� Float�� ���� �ϳ���
Vector3 CASEParser::Parsing_ChangeNumberVector3()
{
	LONG				token;
	Vector3			tempVector3;

	token = m_lexer->GetToken(m_TokenString);
	tempVector3.x = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.z = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.y = (float)atof(m_TokenString);

	return			tempVector3;		// ����ƽ ������ ���۷������ٴ� �� ������ ����.
}

Vector3 CASEParser::Parsing_NormalNumberVector3()
{
	LONG				token;
	Vector3			tempVector3;

	token = m_lexer->GetToken(m_TokenString);
	tempVector3.x = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.y = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.z = (float)atof(m_TokenString);

	return			tempVector3;
}

///--------------------------------------------------
/// ���ο��� ������ ����, ����Ʈ�� �ִ´�
///--------------------------------------------------
// �޽ø� �ϳ� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_OneMesh_to_list()
{
	Mesh* temp = new Mesh;
	m_OneMesh = temp;
	m_OneMesh->m_scenedata = m_scenedata;		// Ŭ������ �� ����
	m_MeshList.push_back(m_OneMesh);
}

// ��Ʈ���� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_MaterialData_to_list()
{
	CMaterial* temp = new CMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

void CASEParser::Create_DiffuseMap_to_list()
{
	MaterialMap* temp = new MaterialMap;
	m_materialmap = temp;
	m_materialdata->m_MapList.push_back(temp);
	m_materialdata->m_DiffuseMap = temp;
	m_materialdata->m_isDiffuseMap = true;
}

void CASEParser::Create_BumpMap_to_list()
{
	MaterialMap* temp = new MaterialMap;
	m_materialmap = temp;
	m_materialdata->m_MapList.push_back(temp); 
	m_materialdata->m_BumpMap = temp;
	m_materialdata->m_isBumpMap = true;
}

void CASEParser::Create_SpecularMap_to_list()
{
	MaterialMap* temp = new MaterialMap;
	m_materialmap = temp;
	m_materialdata->m_MapList.push_back(temp); 
	m_materialdata->m_SpecularMap = temp;
	m_materialdata->m_isSpecularMap = true;
}

void CASEParser::Create_ShineMap_to_list()
{
	MaterialMap* temp = new MaterialMap;
	m_materialmap = temp;
	m_materialdata->m_MapList.push_back(temp); 
	m_materialdata->m_ShineMap = temp;
	m_materialdata->m_isShineMap = true;
}

void CASEParser::Create_AnimationData_to_mesh(Mesh* nowMesh)
{
	AnimationData* temp = new AnimationData;
	m_animation = temp;
	nowMesh->m_animation = m_animation;
	m_isAnimation = true;
}

// ���� �ϳ���..
void CASEParser::Create_OneVertex_to_list()
{
	Vertex* temp = new Vertex;
	m_OneMesh->m_meshvertex.push_back(temp);
}

void CASEParser::Create_BoneData_to_list()
{
	Bone* temp = new Bone;
	m_OneMesh->m_bone = temp;
	m_OneMesh->m_vector_bone_list.push_back(temp);
}

void CASEParser::Create_LightData_to_list()
{
	Light_AnimationData* temp = new Light_AnimationData;
	m_OneMesh->m_lightanimation = temp;
	m_OneMesh->m_list_lightanimation.push_back(temp);
}









