#include "KHMath.h"
#include "ParserDLL.h"

#include <vector>
#include <string>
using namespace std;

#include "CParsingDataClass.h"
#include "ASEFile.h"
#include "CASEParser.h"

using namespace ParserData;

CASEParser::CASEParser()
	: m_Token(0), m_parsingmode(eNone), m_data_asciiexport(0), m_materialcount(0),
	m_MaterialData(nullptr), m_materialmap(nullptr), m_OneMesh(nullptr), m_IsAnimation(false), m_Animation(nullptr), m_lexer(nullptr)
{
	m_lexer = new ASE::CASELexer;
}

CASEParser::~CASEParser()
{
	delete m_lexer;
}

PARSER_DLL bool CASEParser::Load(LPSTR p_File)
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

/// <summary>
/// �޽��� ����ȭ�� �� �ش�.
/// �븻��, �ؽ��� ��ǥ�� ���� ���ؽ��� �ø���, ��ø�Ǵ°��� �����ϰ�..
/// </summary>
void CASEParser::OptimizeVertex(ASEMesh* pMesh)
{
	bool new_VertexSet = true;
	unsigned int resize_VertexIndex = pMesh->m_MeshVertex.size();

	// �⺻ Vertex �־�ΰ� ����..
	for (unsigned int i = 0; i < pMesh->m_MeshVertex.size(); i++)
	{
		Vertex* newVertex = new Vertex;
		newVertex->m_Pos = pMesh->m_MeshVertex[i]->m_Pos;
		newVertex->m_Indices = pMesh->m_MeshVertex[i]->m_Indices;
		newVertex->m_BoneIndices = pMesh->m_MeshVertex[i]->m_BoneIndices;
		newVertex->m_BoneWeights = pMesh->m_MeshVertex[i]->m_BoneWeights;

		pMesh->m_Final_Vertex.push_back(newVertex);
	}

	// ���� Face���� �����ϴ� 3���� Vertex ��..
	for (unsigned int i = 0; i < pMesh->m_MeshFace.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			unsigned int vertexIndex = pMesh->m_MeshFace[i]->m_VertexIndex[j];

			Vertex* nowVertex = pMesh->m_Final_Vertex[vertexIndex];

			// �ؽ�ó�� �ְ�, �������� �ʾ����� �ؽ�ó u,v ����..
			if (pMesh->m_Mesh_NumTVertex > 0 && nowVertex->m_IsTextureSet == false)
			{
				nowVertex->m_U = pMesh->m_Mesh_TVertex[pMesh->m_MeshFace[i]->m_TFace[j]]->m_U;
				nowVertex->m_V = pMesh->m_Mesh_TVertex[pMesh->m_MeshFace[i]->m_TFace[j]]->m_V;
				nowVertex->m_IsTextureSet = true;
			}

			// ���� �ε��� �븻�� �˻�ÿ� �־��ְ� ����..
			if (nowVertex->m_IsNormalSet == false)
			{
				nowVertex->m_Normal = pMesh->m_MeshFace[i]->m_NormalVertex[j];
				nowVertex->m_IsNormalSet = true;
			}

			// Normal, U, V ���� �Ѱ��� �ٸ��� Vertex ���� ����..
			if ((pMesh->m_Final_Vertex[vertexIndex]->m_Normal != pMesh->m_MeshFace[i]->m_NormalVertex[j]))
			{
				new_VertexSet = true;
			}

			if (pMesh->m_Mesh_NumTVertex > 0)
			{
				if ((pMesh->m_Final_Vertex[vertexIndex]->m_U != pMesh->m_Mesh_TVertex[pMesh->m_MeshFace[i]->m_TFace[j]]->m_U) ||
					(pMesh->m_Final_Vertex[vertexIndex]->m_V != pMesh->m_Mesh_TVertex[pMesh->m_MeshFace[i]->m_TFace[j]]->m_V))
				{
					new_VertexSet = true;
				}
			}

			if (new_VertexSet)
			{
				// �߰��� Vertex�� �ִٸ� üũ..
				if (resize_VertexIndex > pMesh->m_MeshVertex.size())
				{
					for (unsigned int k = pMesh->m_MeshVertex.size(); k < resize_VertexIndex; k++)
					{
						// ���� �߰��� Vertex�� ������ �����͸� �����ִ� Face ���� Vertex Index ����..
						if ((pMesh->m_Final_Vertex[k]->m_Indices == pMesh->m_MeshFace[i]->m_VertexIndex[j]) &&
							(pMesh->m_Final_Vertex[k]->m_Normal == pMesh->m_MeshFace[i]->m_NormalVertex[j]))
						{
							if (pMesh->m_Mesh_NumTVertex > 0)
							{
								if ((pMesh->m_Final_Vertex[k]->m_U == pMesh->m_Mesh_TVertex[pMesh->m_MeshFace[i]->m_TFace[j]]->m_U) &&
									(pMesh->m_Final_Vertex[k]->m_V == pMesh->m_Mesh_TVertex[pMesh->m_MeshFace[i]->m_TFace[j]]->m_V))
								{
									pMesh->m_MeshFace[i]->m_VertexIndex[j] = (int)k;
									new_VertexSet = false;
									break;
								}
							}
							else
							{
								pMesh->m_MeshFace[i]->m_VertexIndex[j] = (int)k;
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
					newVertex->m_Indices = nowVertex->m_Indices;
					newVertex->m_Normal = pMesh->m_MeshFace[i]->m_NormalVertex[j];
					newVertex->m_BoneIndices = pMesh->m_MeshVertex[nowVertex->m_Indices]->m_BoneIndices;
					newVertex->m_BoneWeights = pMesh->m_MeshVertex[nowVertex->m_Indices]->m_BoneWeights;
					newVertex->m_IsNormalSet = true;

					if (pMesh->m_Mesh_NumTVertex > 0)
					{
						newVertex->m_U = pMesh->m_Mesh_TVertex[pMesh->m_MeshFace[i]->m_TFace[j]]->m_U;
						newVertex->m_V = pMesh->m_Mesh_TVertex[pMesh->m_MeshFace[i]->m_TFace[j]]->m_V;
						newVertex->m_IsTextureSet = true;
					}

					pMesh->m_Final_Vertex.push_back(newVertex);
					pMesh->m_MeshFace[i]->m_VertexIndex[j] = resize_VertexIndex;
					resize_VertexIndex++;
				}
			}
		}
	}

	// Tanget �� ����..
	for (int i = 0; i < pMesh->m_Mesh_NumFaces; i++)
	{
		int index0 = pMesh->m_MeshFace[i]->m_VertexIndex[0];
		int index1 = pMesh->m_MeshFace[i]->m_VertexIndex[1];
		int index2 = pMesh->m_MeshFace[i]->m_VertexIndex[2];

		DXVector3 ep1 = pMesh->m_Final_Vertex[index1]->m_Pos - pMesh->m_Final_Vertex[index0]->m_Pos;
		DXVector3 ep2 = pMesh->m_Final_Vertex[index2]->m_Pos - pMesh->m_Final_Vertex[index0]->m_Pos;

		DXVector3 uv1 = { pMesh->m_Final_Vertex[index1]->m_U - pMesh->m_Final_Vertex[index0]->m_U,
						  pMesh->m_Final_Vertex[index1]->m_V - pMesh->m_Final_Vertex[index0]->m_V };
		DXVector3 uv2 = { pMesh->m_Final_Vertex[index2]->m_U - pMesh->m_Final_Vertex[index0]->m_U,
						  pMesh->m_Final_Vertex[index2]->m_V - pMesh->m_Final_Vertex[index0]->m_V };

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
		pMesh->m_Final_Vertex[index0]->m_Tanget += tangent;
		pMesh->m_Final_Vertex[index1]->m_Tanget += tangent;
		pMesh->m_Final_Vertex[index2]->m_Tanget += tangent;
	}

	// �ε����� �׳� ����
	pMesh->m_Final_Index = new IndexList[pMesh->m_MeshFace.size()];

	for (unsigned int i = 0; i < pMesh->m_MeshFace.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pMesh->m_Final_Index[i].m_Index[j] = pMesh->m_MeshFace[i]->m_VertexIndex[j];
		}
	}
}

void CASEParser::RecombinationTM(ParserData::ASEMesh* pMesh)
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
		for (auto& k : pMesh->m_Final_Vertex)
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
	pMesh->m_WorldTM = oMatrix;

	// �θ� ��尡 ������� WorldTM �� �� LocalTM..
	// �θ� ��尡 ������� LocalTM �� ��������..
	if (pMesh->m_Parent == nullptr)
	{
		pMesh->m_LocalTM = pMesh->m_WorldTM;
	}
	else
	{
		DXMatrix4X4 piMatrix = pMesh->m_Parent->m_WorldTM;
		pMesh->m_LocalTM = pMesh->m_WorldTM * piMatrix.Inverse();
	}

	for (unsigned int i = 0; i < pMesh->m_Final_Vertex.size(); i++)
	{
		pMesh->m_Final_Vertex[i]->m_Pos = XMVector3Transform(pMesh->m_Final_Vertex[i]->m_Pos, iMatrix);
	}

	// �ִϸ��̼��� ������� ����� ���ص���..
	if (pMesh->m_Animation)
	{
		pMesh->m_Animation->m_TicksPerFrame = 1.0f / m_scenedata.m_FrameSpeed;
		pMesh->m_Animation->m_TotalFrame = m_scenedata.m_LastFrame - m_scenedata.m_FirstFrame;
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
			if (m_MeshList[i]->m_NodeName == m_MeshList[j]->m_ParentName)
			{
				m_MeshList[j]->m_Parent = m_MeshList[i];
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

PARSER_DLL void CASEParser::SetBoneTM(ParserData::ASEMesh* pMesh)
{
	for (size_t i = 0; i < pMesh->m_BoneList.size(); i++)
	{
		for (size_t j = 0; j < m_MeshList.size(); j++)
		{
			if (m_MeshList[j]->m_NodeName == pMesh->m_BoneList[i]->m_BoneName)
			{
				// Mesh Node Name�� Bone Name�� ��ġ�Ұ�� Bone�� Mesh �����͸� �־�����..
				pMesh->m_BoneMeshList.emplace_back(m_MeshList[j]);

				// Bone Setting..
				m_MeshList[j]->m_IsBone = true;
			}

			// Bone List size�� �������� �� �� �ʿ䰡 ����..
			if (pMesh->m_BoneMeshList.size() == pMesh->m_BoneList.size())
			{
				break;
			}
		}
	}

	// Mesh�� ���Ե��ִ� Bone���� OffsetTM�� ���ص���..
	for (size_t i = 0; i < pMesh->m_BoneMeshList.size(); i++)
	{
		// Bone�� WorldTM(NodeTM)
		DXMatrix4X4 boneNodeTM = pMesh->m_BoneMeshList[i]->m_WorldTM;

		// Mesh�� WorldTM(NodeTM)
		DXMatrix4X4 skinNodeTM = pMesh->m_WorldTM;

		// Bone NodeTM * Mesh NodeTM ����� -> Mesh ���� Bone�� OffsetTM
		DXMatrix4X4 boneoffsetTM = boneNodeTM * skinNodeTM.Inverse();

		// Bone OffsetTM ����� -> Bone ���� Mesh�� OffsetTM
		pMesh->m_BoneTMList.emplace_back(boneoffsetTM.Inverse());
	}
}

ParserData::Mesh* CASEParser::GetMesh(int m_Index)
{
	return m_MeshList[m_Index];
}

void CASEParser::DataParsing()
{
	// ��� �����ٰ� ���� �̻� ������ ���� ���(����)�� üũ�ϱ� ���� ����
	int i = 0;

	// �ε��� üũ�� ����
	int m_Index = 0;

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
			break;

			///----------------------------------
			/// SCENE
			///----------------------------------

		case TOKENR_SCENE:
			//
			break;
		case TOKENR_SCENE_FILENAME:
			m_scenedata.m_FileName = Parsing_String();
			break;
		case TOKENR_SCENE_FIRSTFRAME:
			m_scenedata.m_FirstFrame = Parsing_NumberInt();
			break;
		case TOKENR_SCENE_LASTFRAME:
			m_scenedata.m_LastFrame = Parsing_NumberInt();
			break;
		case TOKENR_SCENE_FRAMESPEED:
			m_scenedata.m_FrameSpeed = Parsing_NumberInt();
			break;
		case TOKENR_SCENE_TICKSPERFRAME:
			m_scenedata.m_TicksPerFrame = Parsing_NumberFloat();
			break;
		case TOKENR_SCENE_MESHFRAMESTEP:
		case TOKENR_SCENE_KEYFRAMESTEP:
		case TOKENR_SCENE_BACKGROUND_STATIC:
		case TOKENR_SCENE_AMBIENT_STATIC:
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
			m_MaterialData->m_MaterialNumber = Parsing_NumberInt();
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
			break;

			///----------------------------------
			/// Material
			///----------------------------------

		case TOKENR_MATERIAL_COUNT:
			m_materialcount = Parsing_NumberInt();
			break;
		case TOKENR_MATERIAL_NAME:
		{
			std::string fileName = Parsing_String();
			m_MaterialData->m_MaterialName = fileName.c_str();
		}
		break;
		case TOKENR_MATERIAL_CLASS:
			break;
		case TOKENR_MATERIAL_AMBIENT:
			m_MaterialData->m_Material_Ambient = Parsing_ChangeNumberVector3();
			break;
		case TOKENR_MATERIAL_DIFFUSE:
			m_MaterialData->m_Material_Diffuse = Parsing_ChangeNumberVector3();
			break;
		case TOKENR_MATERIAL_SPECULAR:
			m_MaterialData->m_Material_Specular = Parsing_ChangeNumberVector3();
			break;
		case TOKENR_MATERIAL_SHINE:
			break;
		case TOKENR_MATERIAL_SHINESTRENGTH:
			m_MaterialData->m_Material_Shininess = Parsing_NumberFloat();
			break;
		case TOKENR_MATERIAL_TRANSPARENCY:
			m_MaterialData->m_Material_Transparency = Parsing_NumberFloat();
			break;
		case TOKENR_MATERIAL_WIRESIZE:
		case TOKENR_MATERIAL_SHADING:
		case TOKENR_MATERIAL_XP_FALLOFF:
		case TOKENR_MATERIAL_SELFILLUM:
		case TOKENR_MATERIAL_FALLOFF:
		case TOKENR_MATERIAL_XP_TYPE:
			break;

			///----------------------------------
			/// Material Map
			///----------------------------------

		case TOKENR_MAP_NAME:
			m_materialmap->m_MapName = Parsing_String();
			break;
		case TOKENR_MAP_CLASS:
		case TOKENR_MAP_SUBNO:
		case TOKENR_MAP_AMOUNT:
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
			m_materialmap->m_BitMap = fileRoute.c_str();
		}
		break;
		case TOKENR_MAP_TYPE:
		case TOKENR_UVW_U_OFFSET:
		case TOKENR_UVW_V_OFFSET:
		case TOKENR_UVW_U_TILING:
		case TOKENR_UVW_V_TILING:
		case TOKENR_UVW_ANGLE:
		case TOKENR_UVW_BLUR:
		case TOKENR_UVW_BLUR_OFFSET:
		case TOKENR_UVW_NOUSE_AMT:
		case TOKENR_UVW_NOISE_SIZE:
		case TOKENR_UVW_NOISE_LEVEL:
		case TOKENR_UVW_NOISE_PHASE:
		case TOKENR_BITMAP_FILTER:
			break;

			//--------------------
			// GEOMOBJECT
			//--------------------

		case TOKENR_GROUP:
			//	�� ���� �׷� ����
			break;

		case TOKENR_HELPEROBJECT:
			Create_OneMesh_to_list();
			m_OneMesh->m_Type = eHelperObject;
			break;

		case TOKENR_SHAPEOBJECT:
			Create_OneMesh_to_list();
			m_OneMesh->m_Type = eShape;
			break;

		case TOKENR_GEOMOBJECT:
			Create_OneMesh_to_list();
			m_OneMesh->m_Type = eGeomobject;
			break;
		case TOKENR_LIGHTOBJECT:
			Create_OneMesh_to_list();
			m_OneMesh->m_Type = eLight;
			break;
		case TOKENR_CAMERAOBJECT:
			Create_OneMesh_to_list();
			m_OneMesh->m_Type = eCamera;
			break;

		case TOKENR_NODE_NAME:
			// ��� ������ ������Ʈ���� ���� �� �� �ִ� ������ ���̴�.
			// ��忡 ���� �־�� �� ���� �ٸ���.
		{
			m_OneMesh->m_NodeName = Parsing_String();
		}
		break;

		case TOKENR_NODE_PARENT:
			// �� ����� �θ� ����� ����.
			// �ϴ� �Է��� �ϰ�, ���߿� ��������.
			m_OneMesh->m_ParentName = Parsing_String();
			break;

			///----------------------------------
			/// NODE_TM
			///----------------------------------

		case TOKENR_NODE_TM:
			break;

			// Camera, Light�� NodeTM�� �ι� �����Ƿ�
			// �ι�°�� �ϴ� ��������..

		case TOKENR_INHERIT_POS:
		case TOKENR_INHERIT_ROT:
		case TOKENR_INHERIT_SCL:
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
		case TOKENR_TM_ROTAXIS:
		case TOKENR_TM_ROTANGLE:
		case TOKENR_TM_SCALE:
		case TOKENR_TM_SCALEAXIS:
		case TOKENR_TM_SCALEAXISANG:
			break;

			///----------------------------------
			/// Light
			///----------------------------------

		case TOKENR_LIGHT_TYPE:
		case TOKENR_LIGHT_SHADOWS:
		case TOKENR_LIGHT_USELIGHT:
		case TOKENR_LIGHT_SPOTSHAPE:
		case TOKENR_LIGHT_USEGLOBAL:
		case TOKENR_LIGHT_ABSMAPBIAS:
		case TOKENR_LIGHT_OVERSHOOT:
		case TOKENR_LIGHT_SETTINGS:
		case TOKENR_LIGHT_COLOR:
		case TOKENR_LIGHT_INTENS:
		case TOKENR_LIGHT_ASPECT:
		case TOKENR_LIGHT_HOTSPOT:
		case TOKENR_LIGHT_FALLOFF:
		case TOKENR_LIGHT_TDIST:
		case TOKENR_LIGHT_MAPBIAS:
		case TOKENR_LIGHT_MAPRANGE:
		case TOKENR_LIGHT_MAPSIZE:
		case TOKENR_LIGHT_RAYBIAS:
			break;

			///----------------------------------
			/// Camera
			///----------------------------------

		case TOKENR_CAMERA_TYPE:
		case TOKENR_CAMERA_SETTINGS:
		case TOKENR_CAMERA_NEAR:
		case TOKENR_CAMERA_FAR:
		case TOKENR_CAMERA_FOV:
		case TOKENR_CAMERA_TDIST:
			break;

			///----------------------------------
			/// Bone
			///----------------------------------
		case TOKENR_MESH_NUMBONE:
		{
			// �̰� �ִٸ� �̰��� Skinned Mesh��� ������ ���´�.
			if (Parsing_NumberInt() > 0)
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
			break;
		case TOKENR_BONE_LIST:
			break;
		case TOKENR_BONE:
		{
			Create_BoneData_to_list();
			m_Bone->m_BoneNumber = Parsing_NumberInt();
		}
		break;
		case TOKENR_BONE_NAME:
			m_Bone->m_BoneName = Parsing_String();

		case TOKENR_MESH_WVERTEXS:
			break;

		case TOKENR_MESH_WEIGHT:
			m_Index = Parsing_NumberInt();
			break;
		case TOKENR_BONE_BLENGING_WEIGHT:
		{
			int boneNum = Parsing_NumberInt();
			float boneWeight = Parsing_NumberFloat();

			m_OneMesh->m_MeshVertex[m_Index]->m_BoneIndices.push_back(boneNum);
			m_OneMesh->m_MeshVertex[m_Index]->m_BoneWeights.push_back(boneWeight);
		}
		break;

		///----------------------------------
		/// MESH
		///----------------------------------

		case TOKENR_MESH:
			break;
		case TOKENR_TIMEVALUE:
			break;
		case TOKENR_MESH_NUMVERTEX:
			// Vertex �� ���� ����..
			m_OneMesh->m_Mesh_NumVertex = Parsing_NumberInt();
			break;

			/// MESH_VERTEX_LIST

		case TOKENR_MESH_VERTEX_LIST:
			// ������ �־�� Vertex ������ŭ �����ص���..
		{
			for (int i = 0; i < m_OneMesh->m_Mesh_NumVertex; i++)
			{
				m_OneMesh->m_MeshVertex.push_back(new Vertex);
			}
		}
		break;
		case TOKENR_MESH_VERTEX:
		{
			m_Index = Parsing_NumberInt();
			m_OneMesh->m_MeshVertex[m_Index]->m_Indices = m_Index;
			m_OneMesh->m_MeshVertex[m_Index]->m_Pos = Parsing_ChangeNumberVector3();
		}
		break;

		case TOKENR_MESH_NUMFACES:
			// Face �� ���� ����..
			m_OneMesh->m_Mesh_NumFaces = Parsing_NumberInt();
			break;

			/// MESH_FACE_LIST
		case TOKENR_MESH_FACE_LIST:
			// ������ �־�� Face ������ŭ �����ص���..
			for (int i = 0; i < m_OneMesh->m_Mesh_NumFaces; i++)
			{
				m_OneMesh->m_MeshFace.push_back(new Face);
			}
			break;
		case TOKENR_MESH_FACE:
		{
			m_Index = Parsing_NumberInt();

			Parsing_String();
			m_OneMesh->m_MeshFace[m_Index]->m_VertexIndex[0] = Parsing_NumberInt();
			Parsing_String();
			m_OneMesh->m_MeshFace[m_Index]->m_VertexIndex[2] = Parsing_NumberInt();
			Parsing_String();
			m_OneMesh->m_MeshFace[m_Index]->m_VertexIndex[1] = Parsing_NumberInt();
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
			m_Index = Parsing_NumberInt();

			m_OneMesh->m_MeshFace[m_Index]->m_Normal = Parsing_ChangeNumberVector3();
			// ���� ��ū
			m_lexer->GetToken(m_TokenString);
			Parsing_NumberInt();
			m_OneMesh->m_MeshFace[m_Index]->m_NormalVertex[0] = Parsing_ChangeNumberVector3();
			// ���� ��ū
			m_lexer->GetToken(m_TokenString);
			Parsing_NumberInt();
			m_OneMesh->m_MeshFace[m_Index]->m_NormalVertex[2] = Parsing_ChangeNumberVector3();
			// ���� ��ū
			m_lexer->GetToken(m_TokenString);
			Parsing_NumberInt();
			m_OneMesh->m_MeshFace[m_Index]->m_NormalVertex[1] = Parsing_ChangeNumberVector3();
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
			m_OneMesh->m_Mesh_SumTVertex = m_OneMesh->m_Mesh_NumTVertex;
			m_OneMesh->m_Mesh_NumTVertex += Parsing_NumberInt();
		}
		break;

		case TOKENR_MESH_TVERTLIST:
			// �߰��� tVertex ������ŭ ����..
			for (int i = m_OneMesh->m_Mesh_SumTVertex; i < m_OneMesh->m_Mesh_NumTVertex; i++)
			{
				m_OneMesh->m_Mesh_TVertex.push_back(new ParserData::TVertex);
			}
			break;
		case TOKENR_MESH_TVERT:
		{
			/// ���� ���� �ε��� üũ.. 
			m_Index = Parsing_NumberInt() + m_OneMesh->m_Mesh_SumTVertex;

			m_OneMesh->m_Mesh_TVertex[m_Index]->m_U = Parsing_NumberFloat();
			m_OneMesh->m_Mesh_TVertex[m_Index]->m_V = 1.0f - Parsing_NumberFloat();
			m_OneMesh->m_Mesh_TVertex[m_Index]->m_W = Parsing_NumberFloat();
		}
		break;
		case TOKENR_MESH_NUMTVFACES:
			break;
		case TOKENR_MESH_TFACELIST:
			break;
		case TOKENR_MESH_TFACE:
		{
			m_Index = Parsing_NumberInt();

			m_OneMesh->m_MeshFace[m_Index]->m_TFace[0] = Parsing_NumberInt();
			m_OneMesh->m_MeshFace[m_Index]->m_TFace[2] = Parsing_NumberInt();
			m_OneMesh->m_MeshFace[m_Index]->m_TFace[1] = Parsing_NumberInt();
		}
		break;

		///----------------------------------
		/// Animation
		///----------------------------------

		case TOKENR_TM_ANIMATION:
			Create_AnimationData_to_mesh(m_OneMesh);
			m_OneMesh->m_Type = eAnimation;
			m_OneMesh->m_Animation->m_AniData.push_back(new OneFrame);
			break;
		case TOKENR_CONTROL_POS_TRACK:
			m_OneMesh->m_Animation->m_isPosAnimation = true;
			break;
		case TOKENR_CONTROL_POS_SAMPLE:
		{
			m_Animation->m_AniData.back()->m_Time = (Parsing_NumberFloat() / m_scenedata.m_TicksPerFrame) - m_scenedata.m_FirstFrame;
			m_Animation->m_AniData.back()->m_Pos = Parsing_ChangeNumberVector3();
			break;
		}
		case TOKENR_CONTROL_ROT_TRACK:
			m_OneMesh->m_Animation->m_isRotAnimation = true;
			break;
		case TOKENR_CONTROL_ROT_SAMPLE:
		{
			float _time = 0;
			float _angle = 0;
			Vector3 _rot = Vector3();
			_time = (Parsing_NumberFloat() / m_scenedata.m_TicksPerFrame) - m_scenedata.m_FirstFrame;
			_rot = Parsing_ChangeNumberVector3();
			_angle = Parsing_NumberFloat();

			m_Animation->m_AniData.back()->m_Time = _time;

			Quaternion nowQT = XMQuaternionRotationAxis(_rot, _angle);

			// quaternion ���� ������������..
			if (m_Animation->m_AniData.size() > 1)
			{
				m_Animation->m_AniData.back()->m_RotQt = XMQuaternionMultiply(m_Animation->m_AniData[m_Animation->m_AniData.size() - 2]->m_RotQt, nowQT);
			}
			else
			{
				m_Animation->m_AniData.back()->m_RotQt = nowQT;
			}
			break;
		}
		case TOKENR_CONTROL_SCALE_TRACK:
			m_OneMesh->m_Animation->m_isScaleAnimation = true;
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
			int materialIndex = Parsing_NumberInt();

			// Mesh�� �ش��ϴ� Material Data ����
			m_OneMesh->m_MaterialData = m_list_materialdata[materialIndex];
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
	ASEMesh* temp = new ASEMesh;
	m_OneMesh = temp;
	m_MeshList.push_back(m_OneMesh);
}

// ��Ʈ���� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_MaterialData_to_list()
{
	CMaterial* temp = new CMaterial;
	m_MaterialData = temp;
	m_list_materialdata.push_back(m_MaterialData);
}

void CASEParser::Create_DiffuseMap_to_list()
{
	MaterialMap* temp = new MaterialMap;
	m_materialmap = temp;
	m_MaterialData->m_MapList.push_back(temp);
	m_MaterialData->m_DiffuseMap = temp;
	m_MaterialData->m_IsDiffuseMap = true;
}

void CASEParser::Create_BumpMap_to_list()
{
	MaterialMap* temp = new MaterialMap;
	m_materialmap = temp;
	m_MaterialData->m_MapList.push_back(temp);
	m_MaterialData->m_BumpMap = temp;
	m_MaterialData->m_IsBumpMap = true;
}

void CASEParser::Create_SpecularMap_to_list()
{
	MaterialMap* temp = new MaterialMap;
	m_materialmap = temp;
	m_MaterialData->m_MapList.push_back(temp);
	m_MaterialData->m_SpecularMap = temp;
	m_MaterialData->m_IsSpecularMap = true;
}

void CASEParser::Create_ShineMap_to_list()
{
	MaterialMap* temp = new MaterialMap;
	m_materialmap = temp;
	m_MaterialData->m_MapList.push_back(temp);
	m_MaterialData->m_ShineMap = temp;
	m_MaterialData->m_IsShineMap = true;
}

void CASEParser::Create_AnimationData_to_mesh(Mesh* nowMesh)
{
	OneAnimation* temp = new OneAnimation;
	m_Animation = temp;
	m_Animation->m_TicksPerFrame = m_scenedata.m_TicksPerFrame;
	m_Animation->m_StartFrame = m_scenedata.m_FirstFrame;
	m_Animation->m_EndFrame = m_scenedata.m_LastFrame;
	nowMesh->m_Animation = m_Animation;
	m_IsAnimation = true;
}

// ���� �ϳ���..
void CASEParser::Create_OneVertex_to_list()
{
	Vertex* temp = new Vertex;
	m_OneMesh->m_MeshVertex.push_back(temp);
}

void CASEParser::Create_BoneData_to_list()
{
	Bone* temp = new Bone;
	m_Bone = temp;
	m_OneMesh->m_BoneList.push_back(temp);
}







