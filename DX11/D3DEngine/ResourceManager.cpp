#define _CRT_SECURE_NO_WARNINGS

#include "D3DDefine.h"
#include "D3DEngine.h"
#include "GeometryGenerator.h"
#include "Object.h"
#include "Shader.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
	m_Device = D3DEngine::GetInstance()->GetDevice();
	m_DeviceContext = D3DEngine::GetInstance()->GetDeviceContext();
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Initialize()
{
	m_TexRoute = "../Resource/Textures/";
	m_ModelRoute = "../Resource/Models/";

	m_FBXParser = new FBXParser();
	m_FBXParser->Initalize();

	// Sampler 생성
	CreateSamplerState();

	LoadShader("ColorShader", "ColorVS.cso", "ColorPS.cso", "");
	LoadShader("SkyCubeShader", "SkyCubeVS.cso", "SkyCubePS.cso", "");
	LoadShader("FinalShader", "FinalVS.cso", "FinalPS.cso", "");

	// Blur Shader 로드..
	LoadShader("HorizonBlurShader", "", "", "HorizonBlurCS.cso");
	LoadShader("VerticalBlurShader", "", "", "VerticalBlurCS.cso");

	// Shadow Shader 로드..
	LoadShader("NormalShadowShader", "NormalShadowVS.cso", "", "");
	LoadShader("SkinShadowShader", "SkinShadowVS.cso", "", "");

	// Deferred Shader 로드..
	LoadShader("LightShader", "FullScreenVS.cso", "LightPS.cso", "");
	LoadShader("BasicDeferredShader", "BasicVS.cso", "BasicDeferredPS.cso", "");
	LoadShader("NormalDeferredShader", "NormalTextureVS.cso", "NormalTextureDeferredPS.cso", "");
	LoadShader("TextureDeferredShader", "TextureVS.cso", "TextureDeferredPS.cso", "");
	LoadShader("NormalSkinDeferredShader", "NormalSkinVS.cso", "NormalTextureDeferredPS.cso", "");
	LoadShader("SkinDeferredShader", "SkinVS.cso", "TextureDeferredPS.cso", "");

	// SSAO Shader 로드..
	LoadShader("SSAOShader", "SSAOVS.cso", "SSAOPS.cso", "");
	LoadShader("SSAOHorizonBlurShader", "SSAOBlurVS.cso", "SSAOHorizonBlurPS.cso", "");
	LoadShader("SSAOVerticalBlurShader", "SSAOBlurVS.cso", "SSAOVerticalBlurPS.cso", "");

	// Motion Blur Shader 로드..
	LoadShader("VelocityShader", "FinalVS.cso", "VelocityPS.cso", "");
	LoadShader("MotionBlurShader", "FinalVS.cso", "MotionBlurPS.cso", "");

	// UI Shader 로드..
	LoadShader("UIBasicShader", "UIBasicVS.cso", "UIBasicPS.cso", "");
	LoadShader("UIColliderShader", "UIColliderVS.cso", "UIColliderPS.cso", "");

	// Terrain Shader 로드..
	LoadShader("TerrainShader", "TerrainVS.cso", "TerrainPS.cso", "");

	// Shader에 적용된 Sampler 설정..
	SetShaderSamplerState();

	// 기본 로드..
	LoadData_ASE_Gizmos("CameraGizmo", "../Resource/Models/CameraGizmo.ASE");
	LoadData_ASE_Gizmos("LightGizmo", "../Resource/Models/LightGizmo.ASE");
	LoadData_Axis("AxisGizmo", 0.5f);

	LoadData_Axis("Axis", 10.0f);
	LoadData_Grid();
	LoadData_GeoSphere();
	LoadData_GeoBox();
	LoadData_GeoCylinder();
	LoadData_ColorBox();
	LoadData_ScreenQuad();
	LoadData_SsaoQuad();
	LoadData_UI();
	LoadData_Sky(500.0f);
}

void ResourceManager::CreateSamplerState()
{
	ComPtr<ID3D11SamplerState> samplerState;

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	m_Device->CreateSamplerState(&samplerDesc, &samplerState);
	m_SamplerList.insert(make_pair("samClampMinLinear", samplerState));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	m_Device->CreateSamplerState(&samplerDesc, &samplerState);
	m_SamplerList.insert(make_pair("samWrapAnisotropic", samplerState));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	m_Device->CreateSamplerState(&samplerDesc, &samplerState);
	m_SamplerList.insert(make_pair("samWrapMinLinear", samplerState));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	m_Device->CreateSamplerState(&samplerDesc, &samplerState);
	m_SamplerList.insert(make_pair("samMirrorMinLinear", samplerState));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	m_Device->CreateSamplerState(&samplerDesc, &samplerState);
	m_SamplerList.insert(make_pair("samClampMinLinearPoint", samplerState));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1e5f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	m_Device->CreateSamplerState(&samplerDesc, &samplerState);
	m_SamplerList.insert(make_pair("samBorderLinerPoint", samplerState));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	m_Device->CreateSamplerState(&samplerDesc, &samplerState);
	m_SamplerList.insert(make_pair("samWrapLinerPoint", samplerState));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	m_Device->CreateSamplerState(&samplerDesc, &samplerState);
	m_SamplerList.insert(make_pair("gShadowSam", samplerState));
}

void ResourceManager::SetShaderSamplerState()
{
	for (pair<std::string, ComPtr<ID3D11SamplerState>> sampler : m_SamplerList)
	{
		for (pair<std::string, Shader*> shader : m_ShaderList)
		{
			shader.second->SetSamplerState(sampler.first, sampler.second);
		}
	}
}

void ResourceManager::LoadShader(std::string shaderName, std::string vSName, std::string pSName, std::string cSName)
{
	wstring vName = wstring(vSName.begin(), vSName.end());
	wstring pName = wstring(pSName.begin(), pSName.end());
	wstring cName = wstring(cSName.begin(), cSName.end());

	Shader* newShader = new Shader(shaderName.c_str());
	newShader->CreateShader(vName.c_str(), pName.c_str(), cName.c_str());

	m_ShaderList.insert(make_pair(shaderName, newShader));
}

void ResourceManager::LoadData_ASE(std::string objectName, std::string fileName)
{
	// Parser 생성 및 초기화
	CASEParser* newASE = new CASEParser();

	newASE->Load((LPSTR)fileName.c_str());

	// Mesh Object Buffer 생성
	for (size_t i = 0; i < newASE->m_MeshList.size(); i++)
	{
		ParserData::ASEMesh* mesh = newASE->m_MeshList[i];
		std::string key = objectName + " " + mesh->m_NodeName;

		/// 스키닝 오브젝트 유무 체크..
		if (newASE->m_MeshList[i]->m_IsSkinningObject)
		{
			/// 스키닝 Mesh Object
			LoadData_SkinMesh(objectName, key, mesh);

			/// Bone의 Offset을 구하는 함수..
			// 모든 Local, WorldTM 초기값이 설정된 후에 해주어야 한다..
			newASE->SetBoneTM(mesh);
		}
		else
		{
			/// 일반 Mesh Object
			LoadData_Mesh(objectName, key, mesh);
		}

		if (mesh->m_Animation)
		{
			LoadData_Animation(objectName, key, mesh);
		}
	}

	m_ASEParserList.insert(make_pair(objectName, newASE));

	/// ASE Texture, ASE Material 생성
	LoadData_MaterialList(objectName);
}

void ResourceManager::LoadData_ASE_Animation(std::string objectName, std::string fileName)
{
	CASEParser* newASE = new CASEParser();

	newASE->Load((LPSTR)fileName.c_str());

	for (size_t i = 0; i < newASE->m_MeshList.size(); i++)
	{
		ParserData::Mesh* mesh = newASE->m_MeshList[i];
		std::string key = objectName + " " + mesh->m_NodeName;

		// Animation Data 저장..
		if (mesh->m_Animation)
		{
			LoadData_Animation(objectName, key, mesh);
		}
	}

	m_ASEParserList.insert(make_pair(objectName, newASE));
}

void ResourceManager::LoadData_FBX(std::string objectName, std::string fileName, bool fbxScaling)
{
	m_FBXParser->LoadScene(fileName.c_str(), fbxScaling);
	FBXModel* newFBX = m_FBXParser->GetModel();

	// Mesh Object Buffer 생성
	for (size_t i = 0; i < newFBX->m_MeshList.size(); i++)
	{
		ParserData::Mesh* mesh = newFBX->m_MeshList[i];
		std::string key = objectName + " " + mesh->m_NodeName;

		/// 스키닝 오브젝트 유무 체크..
		if (newFBX->m_MeshList[i]->m_IsSkinningObject)
		{
			/// 스키닝 Mesh Object
			LoadData_SkinMesh(objectName, key, mesh);
		}
		else
		{
			/// 일반 Mesh Object
			LoadData_Mesh(objectName, key, mesh);
		}

		if (mesh->m_Animation)
		{
			LoadData_Animation(objectName, key, mesh);
		}
	}

	m_FBXParserList.insert(make_pair(objectName, newFBX));

	/// FBX Texture, FBX Material 생성
	LoadData_MaterialList(objectName);
}

void ResourceManager::LoadData_FBX_Animation(std::string objectName, std::string fileName, bool fbxScaling)
{
	m_FBXParser->LoadScene(fileName.c_str(), fbxScaling);
	FBXModel* newFBX = m_FBXParser->GetModel();

	for (size_t i = 0; i < newFBX->m_MeshList.size(); i++)
	{
		ParserData::Mesh* mesh = newFBX->m_MeshList[i];
		std::string key = objectName + " " + mesh->m_NodeName;

		// Animation Data 저장..
		if (mesh->m_Animation)
		{
			LoadData_Animation(objectName, key, mesh);
		}
	}

	m_FBXParserList.insert(make_pair(objectName, newFBX));
}

ENGINE_DLL void ResourceManager::LoadData(eLoadType loadType, std::string objectName, std::string fileName, bool fbxScaling)
{
	switch (loadType)
	{
	case eLoadType::Texture:
		LoadData_Texture(objectName, m_TexRoute + fileName);
		break;
	case eLoadType::Sky:
		LoadData_Sky(5000.0f);
		break;
	case eLoadType::Text:
		LoadData_Text(objectName, m_ModelRoute + fileName);
		break;
	case eLoadType::ASE:
		LoadData_ASE(objectName, m_ModelRoute + fileName);
		break;
	case eLoadType::ASE_Ani:
		LoadData_ASE_Animation(objectName, m_ModelRoute + fileName);
		break;
	case eLoadType::FBX:
		LoadData_FBX(objectName, m_ModelRoute + fileName, fbxScaling);
		break;
	case eLoadType::FBX_Ani:
		LoadData_FBX_Animation(objectName, m_ModelRoute + fileName);
		break;
	default:
		break;
	}
}

void ResourceManager::LoadData_MaterialList(std::string objectName)
{
	/// Material Data가 없을경우..

	vector<ParserData::CMaterial*> mat_list;

	if (m_ASEParserList.find(objectName) != m_ASEParserList.end())
	{
		if (m_ASEParserList[objectName]->m_materialcount == 0)
			return;
		else
			mat_list = m_ASEParserList[objectName]->m_list_materialdata;
	}
	else if (m_FBXParserList.find(objectName) != m_FBXParserList.end())
	{
		if (m_FBXParserList[objectName]->m_materialcount == 0)
			return;
		else
			mat_list = m_FBXParserList[objectName]->m_list_materialdata;
	}

	for (ParserData::CMaterial* mat : mat_list)
	{
		// 오브젝트별 Material을 리스트에 담기위해 키 재설정..
		mat->m_MaterialName = objectName + " " + mat->m_MaterialName;

		// Material 생성..
		LoadData_Material(mat);

		// Material에 적용된 Texture 생성..
		for (ParserData::MaterialMap* tex : mat->m_MapList)
		{
			LoadData_Texture(objectName, tex->m_BitMap, tex);
		}
	}
}

void ResourceManager::LoadData_Material(ParserData::CMaterial* materialData)
{
	/// ASE 파일에는 float3 형식으로 로드가되서 w 값이 없다..
	/// 임시로 Reflect 속성과 w 값을 넣어두었으니 나중에 변경하도록 하자..

	MaterialData newMaterial;
	newMaterial.Ambient = XMFLOAT4(materialData->m_Material_Ambient.x, materialData->m_Material_Ambient.y, materialData->m_Material_Ambient.z, 1.0f);
	newMaterial.Diffuse = XMFLOAT4(materialData->m_Material_Diffuse.x, materialData->m_Material_Diffuse.y, materialData->m_Material_Diffuse.z, 1.0f);
	newMaterial.Specular = XMFLOAT4(materialData->m_Material_Specular.x, materialData->m_Material_Specular.y, materialData->m_Material_Specular.z, 16.0f);
	newMaterial.Reflect = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);

	m_MaterialList.insert(make_pair(materialData->m_MaterialName, newMaterial));
}

void ResourceManager::LoadData_Axis(std::string objectName, float length)
{
	VertexBuffer* newBuf = new VertexBuffer();

	// 정점 버퍼를 생성한다.
	// 각 축에 맞도록 6개의 정점을 만들었다.
	Vertex vertices[] =
	{
		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },	// x축 (빨강)
		{ XMFLOAT3(length, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },

		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},	// y축 (초록)
		{ XMFLOAT3(0.0f, length, 0.0f), XMFLOAT4((const float*)&Colors::Green)},

		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Blue)	},	// z축 (파랑)
		{ XMFLOAT3(0.0f, 0.0f, -length), XMFLOAT4((const float*)&Colors::Blue) }
	};

	for (int i = 0; i < 6; i++)
	{
		newBuf->m_VertexPos.push_back(vertices[i].Pos);
	}

	newBuf->Stride = sizeof(Vertex);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = newBuf->Stride * 6;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(m_Device->CreateBuffer(&vbd, &vinitData, &newBuf->VB));


	// 인덱스 버퍼를 생성한다.
	// 역시 3개의 축을 나타내도록 했다.
	newBuf->IndexCount = 6;
	std::vector<UINT> indices(newBuf->IndexCount);

	for (UINT i = 0; i < newBuf->IndexCount; ++i)
	{
		indices[i] = i;
	};

	newBuf->m_MeshIndices = indices;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &newBuf->IB));

	m_VertexList.insert(make_pair(objectName, newBuf));
}

void ResourceManager::LoadData_Grid()
{
	VertexBuffer* newBuf = new VertexBuffer();

	GeometryGenerator::MeshData grid;

	GeometryGenerator geoGen;

	geoGen.CreateGrid(30, 30, 30, 30, grid);

	UINT totalVertexCount = (UINT)grid.Vertices.size();

	std::vector<Vertex> vertices(totalVertexCount);

	for (size_t i = 0; i < totalVertexCount; ++i)
	{
		vertices[i].Pos = grid.Vertices[i].Position;
		vertices[i].Color = XMFLOAT4((const float*)&Colors::DarkGray);
	}

	newBuf->Stride = sizeof(Vertex);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = newBuf->Stride * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_Device->CreateBuffer(&vbd, &vinitData, &newBuf->VB));

	std::vector<UINT> indices;
	indices.insert(indices.end(), grid.Indices.begin(), grid.Indices.end());



	UINT totalIndexCount = (UINT)indices.size();
	newBuf->IndexCount = totalIndexCount;
	newBuf->m_MeshIndices = indices;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &newBuf->IB));

	m_VertexList.insert(make_pair("Grid", newBuf));
}

void ResourceManager::LoadData_Text(std::string objectName, std::string fileName)
{
	std::ifstream fin(fileName);

	if (!fin)
	{
		MessageBox(0, L"Text not found.", 0, 0);
		return;
	}

	VertexBuffer* newBuf = new VertexBuffer();

	DXVector3 vMin(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	DXVector3 vMax(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;

	std::vector<TexVertex> vertices(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
		fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;

		XMVECTOR P = vertices[i].Pos;

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);

		newBuf->m_VertexPos.push_back(vertices[i].Pos);
	}

	newBuf->m_MeshBox.Center = (vMin + vMax) * 0.5f;
	newBuf->m_MeshBox.Extents = (vMax - vMin) * 0.5f;

	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	newBuf->IndexCount = 3 * tcount;
	std::vector<UINT> indices(newBuf->IndexCount);
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	newBuf->m_MeshIndices = indices;

	fin.close();

	newBuf->Stride = sizeof(TexVertex);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = newBuf->Stride * vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_Device->CreateBuffer(&vbd, &vinitData, &newBuf->VB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * newBuf->IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &newBuf->IB));

	//size_t start_index = fileName.rfind("/") + 1;
	//size_t end_index = fileName.rfind(".");
	//fileName = fileName.substr(start_index, end_index - start_index);

	m_VertexList.insert(make_pair(objectName, newBuf));
}

void ResourceManager::LoadData_Wave(UINT NumRows, UINT NumCols)
{
	VertexBuffer* newBuf = new VertexBuffer();

	// Create the vertex buffer.  Note that we allocate space only, as
	// we will be updating the data every time step of the simulation.

	UINT mVertexCount = NumRows * NumCols;
	UINT mTriangleCount = (NumRows - 1) * (NumCols - 1) * 2;

	newBuf->Stride = sizeof(TexVertex);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = newBuf->Stride * mVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	HR(m_Device->CreateBuffer(&vbd, 0, &newBuf->VB));

	// Create the index buffer.  The index buffer is fixed, so we only 
	// need to create and set once.

	std::vector<UINT> indices(3 * mTriangleCount); // 3 indices per face

	int k = 0;
	for (UINT i = 0; i < NumRows - 1; ++i)
	{
		for (DWORD j = 0; j < NumCols - 1; ++j)
		{
			indices[k] = i * NumCols + j;
			indices[k + 1] = i * NumCols + j + 1;
			indices[k + 2] = (i + 1) * NumCols + j;

			indices[k + 3] = (i + 1) * NumCols + j;
			indices[k + 4] = i * NumCols + j + 1;
			indices[k + 5] = (i + 1) * NumCols + j + 1;

			k += 6; // next quad
		}
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * (UINT)indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &newBuf->IB));

	m_VertexList.insert(make_pair("Wave", newBuf));
}

void ResourceManager::LoadData_GeoBox()
{
	GeometryGenerator::MeshData box;

	GeometryGenerator geoGen;

	geoGen.CreateBox(1.0f, 1.0f, 1.0f, box);

	VertexBuffer* newBuf = new VertexBuffer();

	DXVector3 vMin(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	DXVector3 vMax(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	newBuf->IndexCount = (UINT)box.Indices.size();
	UINT totalVertexCount = (UINT)box.Vertices.size();

	std::vector<NormalMapVertex> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
		vertices[k].Tex = box.Vertices[i].TexC;
		vertices[k].Tangent = box.Vertices[i].TangentU;

		XMVECTOR P = XMLoadFloat3(&box.Vertices[i].Position);

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);

		newBuf->m_VertexPos.push_back(vertices[i].Pos);
	}

	newBuf->m_MeshBox.Center = (vMin + vMax) * 0.5f;
	newBuf->m_MeshBox.Extents = (vMax - vMin) * 0.5f;

	newBuf->Stride = sizeof(NormalMapVertex);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = newBuf->Stride * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_Device->CreateBuffer(&vbd, &vinitData, &newBuf->VB));

	std::vector<UINT> indices;
	indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());

	newBuf->m_MeshIndices = indices;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * newBuf->IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &newBuf->IB));

	m_VertexList.insert(make_pair("GeoBox", newBuf));
}

void ResourceManager::LoadData_ColorBox()
{
	GeometryGenerator::MeshData box;

	GeometryGenerator geoGen;

	geoGen.CreateBox(1.0f, 1.0f, 1.0f, box);

	VertexBuffer* newBuf = new VertexBuffer();

	DXVector3 vMin(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	DXVector3 vMax(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	newBuf->IndexCount = (UINT)box.Indices.size();
	UINT totalVertexCount = (UINT)box.Vertices.size();

	std::vector<Vertex> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Color.x = 0.0f;
		vertices[k].Color.y = 1.0f;
		vertices[k].Color.z = 0.0f;
		vertices[k].Color.w = 1.0f;

		XMVECTOR P = XMLoadFloat3(&box.Vertices[i].Position);

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);

		newBuf->m_VertexPos.push_back(vertices[i].Pos);
	}

	newBuf->m_MeshBox.Center = (vMin + vMax) * 0.5f;
	newBuf->m_MeshBox.Extents = (vMax - vMin) * 0.5f;

	newBuf->Stride = sizeof(Vertex);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = newBuf->Stride * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_Device->CreateBuffer(&vbd, &vinitData, &newBuf->VB));

	std::vector<UINT> indices;
	indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());

	newBuf->m_MeshIndices = indices;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * newBuf->IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &newBuf->IB));

	m_VertexList.insert(make_pair("ColorBox", newBuf));
}

void ResourceManager::LoadData_Sky(float skySphereRadius)
{
	GeometryGenerator::MeshData box;

	GeometryGenerator geoGen;

	geoGen.CreateSphere(skySphereRadius, 30, 30, box);

	VertexBuffer* newBuf = new VertexBuffer();

	newBuf->IndexCount = (UINT)box.Indices.size();
	UINT totalVertexCount = (UINT)box.Vertices.size();

	std::vector<VertexPos> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
	}

	newBuf->Stride = sizeof(VertexPos);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = newBuf->Stride * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_Device->CreateBuffer(&vbd, &vinitData, &newBuf->VB));

	std::vector<UINT> indices;
	indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());

	newBuf->m_MeshIndices = indices;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * newBuf->IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &newBuf->IB));

	m_VertexList.insert(make_pair("Sky", newBuf));
}

void ResourceManager::LoadData_GeoSphere()
{
	GeometryGenerator::MeshData sphere;

	GeometryGenerator geoGen;

	geoGen.CreateGeosphere(1.0f, 2, sphere);

	VertexBuffer* newBuf = new VertexBuffer();

	DXVector3 vMin(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	DXVector3 vMax(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	newBuf->IndexCount = (UINT)sphere.Indices.size();
	UINT totalVertexCount = (UINT)sphere.Vertices.size();

	std::vector<TexVertex> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < sphere.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = sphere.Vertices[i].Position;
		vertices[k].Normal = sphere.Vertices[i].Normal;
		vertices[k].Tex = sphere.Vertices[i].TexC;

		XMVECTOR P = XMLoadFloat3(&sphere.Vertices[i].Position);

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);

		newBuf->m_VertexPos.push_back(vertices[i].Pos);
	}

	newBuf->m_MeshBox.Center = (vMin + vMax) * 0.5f;
	newBuf->m_MeshBox.Extents = (vMax - vMin) * 0.5f;

	newBuf->Stride = sizeof(TexVertex);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = newBuf->Stride * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_Device->CreateBuffer(&vbd, &vinitData, &newBuf->VB));

	std::vector<UINT> indices;
	indices.insert(indices.end(), sphere.Indices.begin(), sphere.Indices.end());

	newBuf->m_MeshIndices = indices;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * newBuf->IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &newBuf->IB));

	m_VertexList.insert(make_pair("GeoSphere", newBuf));
}

void ResourceManager::LoadData_GeoCylinder()
{
	GeometryGenerator::MeshData cylinder;

	GeometryGenerator geoGen;

	geoGen.CreateCylinder(0.5f, 0.5f, 3.0f, 15, 15, cylinder);

	VertexBuffer* newBuf = new VertexBuffer();

	DXVector3 vMin(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	DXVector3 vMax(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	newBuf->IndexCount = (UINT)cylinder.Indices.size();
	UINT totalVertexCount = (UINT)cylinder.Vertices.size();

	std::vector<NormalMapVertex> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < cylinder.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = cylinder.Vertices[i].Position;
		vertices[k].Normal = cylinder.Vertices[i].Normal;
		vertices[k].Tex = cylinder.Vertices[i].TexC;
		vertices[k].Tangent = cylinder.Vertices[i].TangentU;

		XMVECTOR P = XMLoadFloat3(&cylinder.Vertices[i].Position);

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);

		newBuf->m_VertexPos.push_back(vertices[i].Pos);
	}

	newBuf->m_MeshBox.Center = (vMin + vMax) * 0.5f;
	newBuf->m_MeshBox.Extents = (vMax - vMin) * 0.5f;

	newBuf->Stride = sizeof(NormalMapVertex);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = newBuf->Stride * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_Device->CreateBuffer(&vbd, &vinitData, &newBuf->VB));

	std::vector<UINT> indices;
	indices.insert(indices.end(), cylinder.Indices.begin(), cylinder.Indices.end());

	newBuf->m_MeshIndices = indices;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * newBuf->IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &newBuf->IB));

	m_VertexList.insert(make_pair("GeoCylinder", newBuf));
}

void ResourceManager::LoadData_ScreenQuad()
{
	GeometryGenerator::MeshData quad;

	GeometryGenerator geoGen;
	geoGen.CreateFullscreenQuad(quad);

	VertexBuffer* newBuf = new VertexBuffer();

	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//
	newBuf->IndexCount = (UINT)quad.Indices.size();

	std::vector<TexVertex> vertices(quad.Vertices.size());

	for (UINT i = 0; i < quad.Vertices.size(); ++i)
	{
		vertices[i].Pos = quad.Vertices[i].Position;
		vertices[i].Normal = quad.Vertices[i].Normal;
		vertices[i].Tex = quad.Vertices[i].TexC;
	}

	newBuf->Stride = sizeof(TexVertex);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(TexVertex) * (UINT)quad.Vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_Device->CreateBuffer(&vbd, &vinitData, &newBuf->VB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * (UINT)quad.Indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &quad.Indices[0];
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &newBuf->IB));

	m_VertexList.insert(make_pair("Quad", newBuf));
}

void ResourceManager::LoadData_SsaoQuad()
{
	VertexBuffer* newBuf = new VertexBuffer();

	TexVertex v[4];

	v[0].Pos = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	v[1].Pos = XMFLOAT3(-1.0f, +1.0f, 0.0f);
	v[2].Pos = XMFLOAT3(+1.0f, +1.0f, 0.0f);
	v[3].Pos = XMFLOAT3(+1.0f, -1.0f, 0.0f);

	// Store far plane frustum corner indices in Normal.x slot.
	v[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	v[1].Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
	v[2].Normal = XMFLOAT3(2.0f, 0.0f, 0.0f);
	v[3].Normal = XMFLOAT3(3.0f, 0.0f, 0.0f);

	v[0].Tex = XMFLOAT2(0.0f, 1.0f);
	v[1].Tex = XMFLOAT2(0.0f, 0.0f);
	v[2].Tex = XMFLOAT2(1.0f, 0.0f);
	v[3].Tex = XMFLOAT2(1.0f, 1.0f);

	UINT indices[6] =
	{
		0, 1, 2,
		0, 2, 3
	};

	newBuf->Stride = sizeof(TexVertex);
	newBuf->IndexCount = 6;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(TexVertex) * 4;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = v;

	HR(m_Device->CreateBuffer(&vbd, &vinitData, &newBuf->VB));

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	HR(m_Device->CreateBuffer(&ibd, &iinitData, &newBuf->IB));

	m_VertexList.insert(make_pair("SsaoQuad", newBuf));
}

void ResourceManager::LoadData_UI()
{
	VertexBuffer* newBuf = new VertexBuffer();

	PosTex v[4];

	v[0].Pos = DXVector3(1.0f, 0.0f, 0.0f);
	v[1].Pos = DXVector3(0.0f, 1.0f, 0.0f);
	v[2].Pos = DXVector3(0.0f, 0.0f, 0.0f);
	v[3].Pos = DXVector3(1.0f, 1.0f, 0.0f);

	v[0].Tex = DXVector2(1.0f, 0.0f);
	v[1].Tex = DXVector2(0.0f, 1.0f);
	v[2].Tex = DXVector2(0.0f, 0.0f);
	v[3].Tex = DXVector2(1.0f, 1.0f);

	UINT indices[6] =
	{
		0, 1, 2,
		1, 0, 3
	};

	newBuf->Stride = sizeof(PosTex);
	newBuf->IndexCount = 6;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(PosTex) * 4;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = v;

	HR(m_Device->CreateBuffer(&vbd, &vinitData, &newBuf->VB));

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	HR(m_Device->CreateBuffer(&ibd, &iinitData, &newBuf->IB));

	m_VertexList.insert(make_pair("UIBasic", newBuf));
}


void ResourceManager::LoadData_ASE_Gizmos(std::string objectName, std::string fileName)
{
	// Parser 생성 및 초기화
	CASEParser* newASE = new CASEParser();

	newASE->Load((LPSTR)fileName.c_str());

	// Mesh Object Buffer 생성
	for (size_t m_Index = 0; m_Index < newASE->m_MeshList.size(); m_Index++)
	{
		VertexBuffer* newBuf = new VertexBuffer();

		UINT vcount = 0;
		UINT tcount = 0;

		DXVector3 vMin(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
		DXVector3 vMax(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

		vcount = (UINT)newASE->m_MeshList[m_Index]->m_Final_Vertex.size();

		std::vector<Vertex> vertices(vcount);

		for (UINT i = 0; i < vcount; i++)
		{
			vertices[i].Pos = newASE->m_MeshList[m_Index]->m_Final_Vertex[i]->m_Pos;

			vertices[i].Color = DXVector4(1.0f, 1.0f, 0.0f, 1.0f);

			XMVECTOR P = vertices[i].Pos;

			vMin = XMVectorMin(vMin, P);
			vMax = XMVectorMax(vMax, P);

			newBuf->m_VertexPos.push_back(vertices[i].Pos);
		}

		newBuf->m_MeshBox.Center = (vMin + vMax) * 0.5f;
		newBuf->m_MeshBox.Extents = (vMax - vMin) * 0.5f;

		tcount = newASE->m_MeshList[m_Index]->m_Mesh_NumFaces;

		newBuf->IndexCount = 3 * tcount;
		std::vector<UINT> indices(newBuf->IndexCount);
		for (UINT i = 0; i < tcount; ++i)
		{
			indices[i * 3 + 0] = newASE->m_MeshList[m_Index]->m_Final_Index[i].m_Index[0];
			indices[i * 3 + 1] = newASE->m_MeshList[m_Index]->m_Final_Index[i].m_Index[1];
			indices[i * 3 + 2] = newASE->m_MeshList[m_Index]->m_Final_Index[i].m_Index[2];
		}

		newBuf->m_MeshIndices = indices;

		newBuf->Stride = sizeof(Vertex);

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = newBuf->Stride * vcount;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(m_Device->CreateBuffer(&vbd, &vinitData, &newBuf->VB));

		//
		// Pack the indices of all the meshes into one index buffer.
		//

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * newBuf->IndexCount;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		HR(m_Device->CreateBuffer(&ibd, &iinitData, &newBuf->IB));

		m_MeshList.insert(make_pair(newASE->m_MeshList[m_Index]->m_NodeName, newASE->m_MeshList[m_Index]));
		m_VertexList.insert(make_pair(newASE->m_MeshList[m_Index]->m_NodeName, newBuf));
	}

	m_ASEParserList.insert(make_pair(objectName, newASE));
}

void ResourceManager::LoadData_SkinMesh(std::string objectName, std::string key, ParserData::Mesh* meshData)
{
	// 동일한 Mesh Data가 있을경우 저장하지 않는다..
	if (m_MeshList.find(key) != m_MeshList.end())
		return;

	// Vertex Data가 없는 Mesh도 저장..
	if (meshData->m_Final_Vertex.empty())
	{
		m_MeshList.insert(make_pair(key, meshData));
		return;
	}

	VertexBuffer* newBuf = new VertexBuffer();

	DXVector3 vMin(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	DXVector3 vMax(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	UINT vcount = 0;
	UINT tcount = 0;

	vcount = (UINT)meshData->m_Final_Vertex.size();

	std::vector<SkinVertex> vertices(vcount);

	for (UINT i = 0; i < vcount; i++)
	{
		vertices[i].Pos = meshData->m_Final_Vertex[i]->m_Pos;

		vertices[i].Normal = meshData->m_Final_Vertex[i]->m_Normal;

		vertices[i].Tex.x = meshData->m_Final_Vertex[i]->m_U;
		vertices[i].Tex.y = meshData->m_Final_Vertex[i]->m_V;

		/// 가중치, 본 인덱스 삽입!
		for (size_t j = 0; j < meshData->m_Final_Vertex[i]->m_BoneIndices.size(); j++)
		{
			if (j < 4)
			{
				vertices[i].BoneIndex1[j] = meshData->m_Final_Vertex[i]->m_BoneIndices[j];
				vertices[i].BoneWeight1[j] = meshData->m_Final_Vertex[i]->m_BoneWeights[j];
			}
			else if (j < 8)
			{
				vertices[i].BoneIndex2[j - 4] = meshData->m_Final_Vertex[i]->m_BoneIndices[j];
				vertices[i].BoneWeight2[j - 4] = meshData->m_Final_Vertex[i]->m_BoneWeights[j];
			}
			else if (j < 12)
			{
				vertices[i].BoneIndex3[j - 8] = meshData->m_Final_Vertex[i]->m_BoneIndices[j];
				vertices[i].BoneWeight3[j - 8] = meshData->m_Final_Vertex[i]->m_BoneWeights[j];
			}
		}

		vertices[i].Tangent = meshData->m_Final_Vertex[i]->m_Tanget;

		XMVECTOR P = meshData->m_Final_Vertex[i]->m_Pos;

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);

		newBuf->m_VertexPos.push_back(vertices[i].Pos);
	}

	newBuf->m_MeshBox.Center = (vMin + vMax) * 0.5f;
	newBuf->m_MeshBox.Extents = (vMax - vMin) * 0.5f;
	newBuf->m_ColType = eColliderType::Box;

	tcount = meshData->m_MeshFace.size();

	newBuf->IndexCount = 3 * tcount;
	std::vector<UINT> indices(newBuf->IndexCount);
	for (UINT i = 0; i < tcount; ++i)
	{
		indices[i * 3 + 0] = meshData->m_Final_Index[i].m_Index[0];
		indices[i * 3 + 1] = meshData->m_Final_Index[i].m_Index[1];
		indices[i * 3 + 2] = meshData->m_Final_Index[i].m_Index[2];
	}

	newBuf->m_MeshIndices = indices;

	newBuf->Stride = sizeof(SkinVertex);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = newBuf->Stride * vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_Device->CreateBuffer(&vbd, &vinitData, &newBuf->VB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * newBuf->IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &newBuf->IB));

	m_MeshList.insert(make_pair(key, meshData));
	m_VertexList.insert(make_pair(key, newBuf));
}

void ResourceManager::LoadData_Mesh(std::string objectName, std::string key, ParserData::Mesh* meshData)
{
	// 동일한 Mesh Data가 있을경우 저장하지 않는다..
	if (m_MeshList.find(key) != m_MeshList.end())
		return;

	// Vertex Data가 없는 Mesh도 저장..
	if (meshData->m_Final_Vertex.size() <= 1 || meshData->m_MeshFace.empty())
	{
		m_MeshList.insert(make_pair(key, meshData));
		return;
	}

	VertexBuffer* newBuf = new VertexBuffer();

	DXVector3 vMin(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	DXVector3 vMax(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	UINT vcount = 0;
	UINT tcount = 0;

	vcount = (UINT)meshData->m_Final_Vertex.size();

	std::vector<NormalMapVertex> vertices(vcount);

	for (UINT i = 0; i < vcount; i++)
	{
		vertices[i].Pos = meshData->m_Final_Vertex[i]->m_Pos;

		vertices[i].Normal = meshData->m_Final_Vertex[i]->m_Normal;

		vertices[i].Tex.x = meshData->m_Final_Vertex[i]->m_U;
		vertices[i].Tex.y = meshData->m_Final_Vertex[i]->m_V;

		vertices[i].Tangent = meshData->m_Final_Vertex[i]->m_Tanget;

		XMVECTOR P = meshData->m_Final_Vertex[i]->m_Pos;

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);

		newBuf->m_VertexPos.push_back(vertices[i].Pos);
	}

	newBuf->m_MeshBox.Center = (vMin + vMax) * 0.5f;
	newBuf->m_MeshBox.Extents = (vMax - vMin) * 0.5f;
	newBuf->m_ColType = eColliderType::Box;

	tcount = meshData->m_MeshFace.size();

	newBuf->IndexCount = 3 * tcount;
	std::vector<UINT> indices(newBuf->IndexCount);
	for (UINT i = 0; i < tcount; ++i)
	{
		indices[i * 3 + 0] = meshData->m_Final_Index[i].m_Index[0];
		indices[i * 3 + 1] = meshData->m_Final_Index[i].m_Index[1];
		indices[i * 3 + 2] = meshData->m_Final_Index[i].m_Index[2];
	}

	newBuf->m_MeshIndices = indices;

	newBuf->Stride = sizeof(NormalMapVertex);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = newBuf->Stride * vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_Device->CreateBuffer(&vbd, &vinitData, &newBuf->VB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * newBuf->IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &newBuf->IB));

	m_MeshList.insert(make_pair(key, meshData));
	m_VertexList.insert(make_pair(key, newBuf));
}

void ResourceManager::LoadData_Animation(std::string objectName, std::string key, ParserData::Mesh* meshData)
{
	m_AnimationList.insert(make_pair(key, meshData->m_Animation));
}

ENGINE_DLL void ResourceManager::CreateBoneCollider(std::string objectName, eColliderType colType, float range)
{
	FBXModel* fbxParser = m_FBXParserList.find(objectName)->second;

	for (size_t i = 0; i < fbxParser->m_MeshList.size(); i++)
	{
		if (fbxParser->m_MeshList[i]->m_IsSkinningObject) continue;

		std::string key = objectName + " " + fbxParser->m_MeshList[i]->m_NodeName;

		switch (colType)
		{
		case eColliderType::Box:
			SetBoxCollider(objectName, key, fbxParser->m_MeshList[i], DXVector3(range, range, range));
			break;
		case eColliderType::Sphere:
			SetSphereCollider(objectName, key, fbxParser->m_MeshList[i], range);
			break;
		case eColliderType::Mesh:
			break;
		default:
			break;
		}
	}
}

void ResourceManager::SetBoxCollider(std::string objectName, std::string key, ParserData::Mesh* meshData, DXVector3 range)
{
	if (meshData->m_IsBone)
	{
		auto buf = m_VertexList.find(key);

		VertexBuffer* newBuf;

		if (buf == m_VertexList.end())
		{
			newBuf = new VertexBuffer();
			m_VertexList.insert(make_pair(key, newBuf));
		}
		else
		{
			newBuf = buf->second;
		}

		//newBuf->m_MeshBox.Center = DXVector3(XMVector3Transform(meshData->m_tm_row3, meshData->m_worldTM.Inverse()));
		newBuf->m_MeshBox.Center = DXVector3(0.0f, 0.0f, 0.0f);
		newBuf->m_MeshBox.Extents = range;
		newBuf->m_ColType = eColliderType::Box;
	}
}


void ResourceManager::SetSphereCollider(std::string objectName, std::string key, ParserData::Mesh* meshData, float range)
{
	if (meshData->m_IsBone)
	{
		auto buf = m_VertexList.find(key);

		VertexBuffer* newBuf;

		if (buf == m_VertexList.end())
		{
			newBuf = new VertexBuffer();
			m_VertexList.insert(make_pair(key, newBuf));
		}
		else
		{
			newBuf = buf->second;
		}

		//newBuf->m_MeshSphere.Center = DXVector3(XMVector3Transform(meshData->m_tm_row3, meshData->m_worldTM.Inverse()));
		newBuf->m_MeshBox.Center = DXVector3(0.0f, 0.0f, 0.0f);
		newBuf->m_MeshSphere.Radius = range;
		newBuf->m_ColType = eColliderType::Sphere;
	}
}

void ResourceManager::LoadData_Texture(std::string objectName, std::string fileName, ParserData::MaterialMap* materialmap)
{
	ID3D11Resource* texResource = nullptr;
	ComPtr<ID3D11ShaderResourceView> newTex = nullptr;

	std::string file_extension;
	size_t m_Index = fileName.rfind(".");
	file_extension = fileName.substr(m_Index, fileName.size() - m_Index);
	CString file_path = fileName.c_str();

	// 확장자에 따른 텍스처 파일 로드 방식..
	if (file_extension.compare(".dds") == 0)
	{
		HR(CreateDDSTextureFromFile(m_Device.Get(), file_path, &texResource, newTex.GetAddressOf()));
	}
	else
	{
		HR(CreateWICTextureFromFile(m_Device.Get(), m_DeviceContext.Get(), file_path, &texResource, newTex.GetAddressOf()));
	}

	// Material 설정..
	if (materialmap == nullptr)
	{
		fileName = objectName;
	}
	else
	{
		materialmap->m_MapName = objectName + " " + materialmap->m_MapName;
		fileName = materialmap->m_MapName;
	}

	// map key 저장시 포인터 형태로 저장하면 주소값이 저장되므로 주의..
	// 문자열을 키로 입력하고 싶으면 string 을 쓰자..
	m_TexList.insert(make_pair(fileName, std::move(newTex)));

	RELEASE_COM(texResource);
}

void ResourceManager::ResetFBX()
{
	RELEASE_COM(m_FBXParser);
}

ENGINE_DLL ID3D11ShaderResourceView* ResourceManager::GetTexture(std::string meshName, eTextureType textureType)
{
	std::string fileName;

	if (m_MeshList[meshName]->m_MaterialData == nullptr) return nullptr;

	// 텍스쳐 타입에 따른 로딩방식..
	switch (textureType)
	{
	case eTextureType::Diffuse:
	{
		if (m_MeshList[meshName]->m_MaterialData->m_IsDiffuseMap)
			fileName = m_MeshList[meshName]->m_MaterialData->m_DiffuseMap->m_MapName;
		else
			return nullptr;
	}
	break;
	case eTextureType::Bump:
	{
		if (m_MeshList[meshName]->m_MaterialData->m_IsBumpMap)
			fileName = m_MeshList[meshName]->m_MaterialData->m_BumpMap->m_MapName;
		else
			return nullptr;
	}
	break;
	case eTextureType::Specular:
	{
		if (m_MeshList[meshName]->m_MaterialData->m_IsSpecularMap)
			fileName = m_MeshList[meshName]->m_MaterialData->m_SpecularMap->m_MapName;
		else
			return nullptr;
	}
	break;
	case eTextureType::Shine:
	{
		if (m_MeshList[meshName]->m_MaterialData->m_IsShineMap)
			fileName = m_MeshList[meshName]->m_MaterialData->m_ShineMap->m_MapName;
		else
			return nullptr;
	}
	break;
	default:
		break;
	}

	return m_TexList[fileName].Get();
}

ENGINE_DLL ID3D11ShaderResourceView* ResourceManager::GetTexture(std::string texName)
{
	// 일반 텍스쳐 이름을 통한 로딩방식..
	return m_TexList[texName].Get();
}

CASEParser* ResourceManager::GetASEParser(std::string objectName)
{
	return m_ASEParserList[objectName];
}

FBXModel* ResourceManager::GetFBXParser(std::string objectName)
{
	return m_FBXParserList[objectName];
}

VertexBuffer* ResourceManager::GetVertexBuffer(std::string key)
{
	return m_VertexList[key];
}

ParserData::Mesh* ResourceManager::GetMesh(std::string key)
{
	return m_MeshList[key];
}

ParserData::Mesh* ResourceManager::GetMesh(std::string objectName, int count)
{
	if (m_ASEParserList.find(objectName) != m_ASEParserList.end())
	{
		return m_ASEParserList[objectName]->m_MeshList[count];
	}
	else if (m_FBXParserList.find(objectName) != m_FBXParserList.end())
	{
		return m_FBXParserList[objectName]->m_MeshList[count];
	}

	return nullptr;
}

ParserData::OneAnimation* ResourceManager::GetAnimation(std::string key)
{
	return m_AnimationList[key];
}

MaterialData ResourceManager::GetMaterial(std::string key)
{
	if (m_MeshList[key]->m_MaterialData == nullptr)
	{
		return MaterialData();
	}

	return m_MaterialList[m_MeshList[key]->m_MaterialData->m_MaterialName];
}

std::string ResourceManager::GetMeshName(std::string objectName, int count)
{
	if (m_ASEParserList.find(objectName) != m_ASEParserList.end())
	{
		return m_ASEParserList[objectName]->m_MeshList[count]->m_NodeName;
	}
	else if (m_FBXParserList.find(objectName) != m_FBXParserList.end())
	{
		return m_FBXParserList[objectName]->m_MeshList[count]->m_NodeName;
	}

	return "";
}

std::string ResourceManager::GetMeshKey(std::string objectName, int count /*= 0*/)
{
	std::string key(objectName);
	key += " ";
	key += GetMeshName(objectName, count);

	return key;
}

size_t ResourceManager::GetMeshListSize(std::string objectName)
{
	if (m_ASEParserList.find(objectName) != m_ASEParserList.end())
	{
		return m_ASEParserList[objectName]->m_MeshList.size();
	}
	else if (m_FBXParserList.find(objectName) != m_FBXParserList.end())
	{
		return m_FBXParserList[objectName]->m_MeshList.size();
	}

	return 0;
}

Shader* ResourceManager::GetShader(std::string name)
{
	return m_ShaderList[name];
}

std::unordered_map<std::string, Shader*> ResourceManager::GetShaderList()
{
	return m_ShaderList;
}

void ResourceManager::Release()
{
	RESET_COM(m_Device);

	for (auto& k : m_TexList)
	{
		RESET_COM((&k)->second);
	}

	for (auto& k : m_VertexList)
	{
		SAFE_DELETE((&k)->second);
	}

	for (auto& k : m_MeshList)
	{
		SAFE_DELETE((&k)->second);
	}

	for (auto& k : m_ASEParserList)
	{
		SAFE_DELETE((&k)->second);
	}

	m_TexList.clear();
	m_VertexList.clear();
	m_MeshList.clear();
	m_AnimationList.clear();
	m_MaterialList.clear();
	m_ASEParserList.clear();
}