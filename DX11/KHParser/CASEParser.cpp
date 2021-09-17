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
// 로딩한다.
// 이것이 끝났다면 정해진 데이터형에 모든 데이터를 읽어서 들어가 있어야 한다.
//
// Parsing에 대해:
// 항상 느끼는 것이지만 parsing이라는 것은 데이터가 일정 규격에 맞게 들어가 있다는 것을 전제로 한다.
// 하지만, 파일 내부에 순서가 바뀌어 들어가 있는것이 가능하다던지 하는 규칙이 생기면
// 검색하는 루틴이 복잡해지기 마련. 일단은 순서대로 일정 규약으로 들어가 있다는것을 가정하자.
// -> 그래서, 재귀 호출을 하는 하나의 큰 함수로 해결봤다.
// -> depth를 기준으로 오동작에 대한 안전 코드를 넣어야겠다
//---------------------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	/// 파일을 로드한다.
	if (!m_lexer->Open(p_File))
	{
		TRACE("파일을 여는 중에 문제가 발생했습니다!");
		return FALSE;
	}

	/// 비재귀 버전..
	DataParsing();

	// Optimize & Parent Set..
	OptimizeData();

	return TRUE;
}

/// 1차 변환

// CScenedata 복사
bool CASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
	// 값 복사
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// 메시의 최적화를 해 준다.
/// 노말값, 텍스쳐 좌표에 따라 버텍스를 늘리고, 중첩되는것은 제거하고..
/// </summary>
void CASEParser::OptimizeVertex(Mesh* pMesh)
{
	bool new_VertexSet = true;
	unsigned int resize_VertexIndex = pMesh->m_meshvertex.size();

	// 기본 Vertex 넣어두고 시작..
	for (unsigned int i = 0; i < pMesh->m_meshvertex.size(); i++)
	{
		Vertex* newVertex = new Vertex;
		newVertex->m_Pos = pMesh->m_meshvertex[i]->m_Pos;
		newVertex->m_indices = pMesh->m_meshvertex[i]->m_indices;
		newVertex->m_boneIndices = pMesh->m_meshvertex[i]->m_boneIndices;
		newVertex->m_boneWeights = pMesh->m_meshvertex[i]->m_boneWeights;

		pMesh->m_opt_vertex.push_back(newVertex);
	}

	// 각각 Face마다 존재하는 3개의 Vertex 비교..
	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			unsigned int vertexIndex = pMesh->m_meshface[i]->m_vertexindex[j];

			Vertex* nowVertex = pMesh->m_opt_vertex[vertexIndex];

			// 텍스처가 있고, 설정하지 않았으면 텍스처 u,v 설정..
			if (pMesh->m_mesh_numtvertex > 0 && nowVertex->m_istextureset == false)
			{
				nowVertex->u = pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_u;
				nowVertex->v = pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_v;
				nowVertex->m_istextureset = true;
			}

			// 최초 인덱스 노말값 검사시엔 넣어주고 시작..
			if (nowVertex->m_isnormalset == false)
			{
				nowVertex->m_Normal = pMesh->m_meshface[i]->m_normalvertex[j];
				nowVertex->m_isnormalset = true;
			}

			// Normal, U, V 값중 한개라도 다르면 Vertex 새로 생성..
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
				// 추가된 Vertex가 있다면 체크..
				if (resize_VertexIndex > pMesh->m_meshvertex.size())
				{
					for (unsigned int k = pMesh->m_meshvertex.size(); k < resize_VertexIndex; k++)
					{
						// 새로 추가한 Vertex와 동일한 데이터를 갖고있는 Face 내의 Vertex Index 수정..
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

				// 새로 추가해야할 Vertex..
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

	// Tanget 값 설정..
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

		// 현재 픽셀 쉐이더에서 연산을 통해 T, B, N을 얻는데
		// 픽셀 쉐이더 내의 연산은 버텍스 쉐이더의 연산에 비해 호출 횟수가 차원이 다르게 크므로 부하가 올 수 있다..
		// 법선맵의 픽셀의 색은 픽셀 쉐이더 안이 아니면 얻을수 없기 때문에 픽셀 쉐이더에서 연산을 한다고 한다..
		/// 현재 연산과정을 버텍스 쉐이더로 옮겨둠
		DXVector3 tangent = (ep1 * uv2.y - ep2 * uv1.y) * den;
		tangent.Normalize();

		//DXVector3 binormal = (ep2 * uv1.x - ep1 * uv2.x) * den;
		//binormal.Normalize();

		// 유사 정점은 값을 누적하여 쉐이더에서 평균값을 사용하도록 하자..
		pMesh->m_opt_vertex[index0]->m_Tanget += tangent;
		pMesh->m_opt_vertex[index1]->m_Tanget += tangent;
		pMesh->m_opt_vertex[index2]->m_Tanget += tangent;
	}

	// 인덱스는 그냥 복사
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
	// WorldTM -> LocalTM 변환..
	DXVector4 row0 = pMesh->m_tm_row0;
	DXVector4 row1 = pMesh->m_tm_row1;
	DXVector4 row2 = pMesh->m_tm_row2;
	DXVector4 row3 = pMesh->m_tm_row3;
	row3.w = 1;

	DXMatrix4X4 oMatrix(row0, row1, row2, row3);

	/// Negative Scale Check
	// 3D Max의 작업상 Mirroring으로 인해 Scale값이 음수가 나올 경우를 Negative Scale 이라고 한다
	// 처리를 안해줄 경우 Normal 처리나 Animation 처리시에 문제가 된다
	// Rotaion을 나타내는 축 Row1 ~ Row3 중 두축을 외적 한 값과 다른 한 축을 내적하여 음수이면 축이 반대이고 양수이면 축이 일치한다

	XMVECTOR crossVec = XMVector3Cross(pMesh->m_tm_row0, pMesh->m_tm_row1);
	XMVECTOR dotVec = XMVector3Dot(crossVec, pMesh->m_tm_row2);

	XMFLOAT3 resultVec;

	XMStoreFloat3(&resultVec, dotVec);

	// 축 방향이 다른 경우만 처리해주자..
	if (resultVec.x < 0 || resultVec.y < 0 || resultVec.z < 0)
	{
		XMVECTOR nodeScale;
		XMVECTOR nodetRot;
		XMVECTOR nodePos;

		// 기존 매트릭스에서 분리..
		XMMatrixDecompose(&nodeScale, &nodetRot, &nodePos, oMatrix);

		// 스케일 재조정..
		nodeScale *= -1;

		// 노말값 재조정..
		for (auto& k : pMesh->m_opt_vertex)
		{
			k->m_Normal *= -1;
		}

		/// 텍스쳐나 맵핑 데이터도 변환을 해주어야 할까..?

		// 매트릭스 재조합..
		oMatrix = XMMatrixScalingFromVector(nodeScale) * XMMatrixRotationQuaternion(nodetRot) * XMMatrixTranslationFromVector(nodePos);
	}

	// WorldTM 역행렬을 구한다..
	DXMatrix4X4 iMatrix = oMatrix.Inverse();

	// 원점으로 이동하기위해 WorldTM 을 LocalTM 으로 설정..
	pMesh->m_worldTM = oMatrix;

	// 부모 노드가 없을경우 WorldTM 이 곧 LocalTM..
	// 부모 노드가 있을경우 LocalTM 을 구해주자..
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

	// 애니메이션이 있을경우 사이즈를 구해두자..
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
	// 부모 자식 연결..
	for (unsigned int i = 0; i < m_MeshList.size(); i++)
	{
		for (unsigned int j = 0; j < m_MeshList.size(); j++)
		{
			if (i == j)
			{
				continue;
			}

			// 노드 이름과 다른 노드 부모 이름이 같다면 상속관계..
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
				// Mesh Node Name과 Bone Name이 일치할경우 Bone의 Mesh 포인터를 넣어주자..
				pMesh->m_boneList.emplace_back(m_MeshList[j]);

				// Bone Setting..
				m_MeshList[j]->m_IsBone = true;
			}

			// Bone List size가 같아지면 더 돌 필요가 없다..
			if (pMesh->m_boneList.size() == pMesh->m_vector_bone_list.size())
			{
				break;
			}
		}
	}

	// Mesh에 포함되있는 Bone들의 OffsetTM을 구해두자..
	for (size_t i = 0; i < pMesh->m_boneList.size(); i++)
	{
		// Bone의 WorldTM(NodeTM)
		DXMatrix4X4 boneNodeTM = pMesh->m_boneList[i]->m_worldTM;

		// Mesh의 WorldTM(NodeTM)
		DXMatrix4X4 skinNodeTM = pMesh->m_worldTM;

		// Bone NodeTM * Mesh NodeTM 역행렬 -> Mesh 기준 Bone의 OffsetTM
		DXMatrix4X4 boneoffsetTM = boneNodeTM * skinNodeTM.Inverse();

		// Bone OffsetTM 역행렬 -> Bone 기준 Mesh의 OffsetTM
		pMesh->m_boneTMList.emplace_back(boneoffsetTM.Inverse());
	}
}

ParserData::Mesh* CASEParser::GetMesh(int index)
{
	return m_MeshList[index];
}

void CASEParser::DataParsing()
{
	// 재귀 돌리다가 일정 이상 루프를 도는 경우(오류)를 체크하기 위한 변수
	int i = 0;

	// 인덱스 체크용 변수
	int index = 0;

	LONG nowtoken = NULL;

	// 파일이 끝날때까지 계속해서 읽기..
	while (true)
	{
		// 현재 토큰 확인..
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
			//AfxMessageBox( m_TokenString, NULL, NULL);		/// 임시로 코멘트를 출력해본다
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
			// 그냥 안의 내용을 읽어버린다 }가 나올때까지
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

			// 파일 임시경로..
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
			//	한 개의 그룹 시작
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
			// 어쩄든 지금은 오브젝트들을 구별 할 수 있는 유일한 값이다.
			// 모드에 따라 넣어야 할 곳이 다르다.
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
			// 현 노드의 부모 노드의 정보.
			// 일단 입력을 하고, 나중에 정리하자.
			m_OneMesh->m_nodeparent = Parsing_String();
			break;

			///----------------------------------
			/// NODE_TM
			///----------------------------------

		case TOKENR_NODE_TM:
			break;

		// Camera, Light는 NodeTM이 두번 나오므로
		// 두번째는 일단 무시하자..

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
			// 현재 카메라 상태였다면 이미 노드를 읽은 것으로 표시해준다.
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
			// 이게 있다면 이것은 Skinned Mesh라고 단정을 짓는다.
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
			// 이 다음 ABSOLUTE가 나오기는 하는데, 쓸 일이 없다.
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
			/// TIMEVALUE가 여러곳에서 사용되는데 어떻게 처리해야할까..
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
			// Vertex 총 개수 저장..
			m_OneMesh->m_mesh_numvertex = Parsing_NumberInt();
			break;

			/// MESH_VERTEX_LIST

		case TOKENR_MESH_VERTEX_LIST:
			// 이전에 넣어둔 Vertex 개수만큼 생성해두자..
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
			// Face 총 개수 저장..
			m_OneMesh->m_mesh_numfaces = Parsing_NumberInt();
			break;

			/// MESH_FACE_LIST
		case TOKENR_MESH_FACE_LIST:
			// 이전에 넣어둔 Face 개수만큼 생성해두자..
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
			// 다음 토큰
			m_lexer->GetToken(m_TokenString);
			Parsing_NumberInt();
			m_OneMesh->m_meshface[index]->m_normalvertex[0] = Parsing_ChangeNumberVector3();
			// 다음 토큰
			m_lexer->GetToken(m_TokenString);
			Parsing_NumberInt();
			m_OneMesh->m_meshface[index]->m_normalvertex[2] = Parsing_ChangeNumberVector3();
			// 다음 토큰
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
			/// 블랜더의 경우 TVERTEX는 여러번 들어오므로 누적시켜주자..
		{
			m_OneMesh->m_mesh_sumtvertex = m_OneMesh->m_mesh_numtvertex;
			m_OneMesh->m_mesh_numtvertex += Parsing_NumberInt();
		}
		break;

		case TOKENR_MESH_TVERTLIST:
			// 추가된 tVertex 개수만큼 생성..
			for (int i = m_OneMesh->m_mesh_sumtvertex; i < m_OneMesh->m_mesh_numtvertex; i++)
			{
				m_OneMesh->m_mesh_tvertex.push_back(new ParserData::COneTVertex);
			}
			break;
		case TOKENR_MESH_TVERT:
		{
			/// 누적 현재 인덱스 체크.. 
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

			// quaternion 값은 누적시켜주자..
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

			// Mesh에 해당하는 Material Data 삽입
			m_OneMesh->m_materialdata = m_list_materialdata[m_OneMesh->m_material_ref];
		}
			break;

		case TOKENR_WIREFRAME_COLOR:
			break;
		case TOKEND_END:
			// 아마도 이건 파일의 끝이 나타났을때인것 같은데. while을 탈출해야 하는데?

			//AfxMessageBox("파일의 끝을 본 것 같습니다!", MB_OK, NULL);
			TRACE("TRACE: 파싱중: 파일의 끝을 봤습니다!\n");
			return;

			break;

			/// 위의 아무것도 해당하지 않을때
		default:
			// 아무것도 하지 않는다.
			break;

		}	// switch()


		///-----------------------------------------------
		/// 안전 코드.
		i++;
		if (i > 1000000)
		{
			// 루프를 1000000번이상이나 돌 이유가 없다. (데이터가 100000개가 아닌이상)
			// 만약 1000000이상 돌았다면 확실히 뭔가 문제가 있는 것이므로
			TRACE("루프를 백만번 돌았습니다!");
			return;
		}
		/// 안전 코드.
		///-----------------------------------------------

	}		// while()

	return;
}


///----------------------------------------------------------------------
/// parsing을 위한 단위별 함수들
///----------------------------------------------------------------------

// long을 읽어서 리턴해준다.
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
	/// ※m_TokenString ( char[255] ) 이기 때문에 CString에 넣으면 에러 날거라 생각했는데, 생각보다 CString은 잘 만들어진 것 같다. 알아서 받아들이는데?
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


// 3개의 Float를 벡터 하나로
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

	return			tempVector3;		// 스태틱 변수의 레퍼런스보다는 값 전달을 하자.
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
/// 내부에서 뭔가를 생성, 리스트에 넣는다
///--------------------------------------------------
// 메시를 하나 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_OneMesh_to_list()
{
	Mesh* temp = new Mesh;
	m_OneMesh = temp;
	m_OneMesh->m_scenedata = m_scenedata;		// 클래스간 값 복사
	m_MeshList.push_back(m_OneMesh);
}

// 메트리얼 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
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

// 정점 하나를..
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









