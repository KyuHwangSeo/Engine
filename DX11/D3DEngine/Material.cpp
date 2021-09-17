#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Gizmos.h"
#include "Camera.h"
#include "Material.h"

DXMatrix4X4 Material::g_ShadowTranspose;

Material::Material()
	:m_DiffuseMapSRV(nullptr), m_NormalMapSRV(nullptr), m_CubeMapSRV(nullptr),
	Is_DiffuseMap(false), Is_NormalMap(false), Is_CubeMap(false), Is_ShadowMap(true)
{
	/// Before Material Data
	//m_MaterialData.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	//m_MaterialData.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	//m_MaterialData.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
	//m_MaterialData.Reflect = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);

	// ASE 로드 방식으로 변경해야함..
	// 기본 Material Data 설정..
	m_MaterialData.Ambient = DXVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_MaterialData.Diffuse = DXVector4(0.8f, 0.8f, 0.8f, 1.0f);
	m_MaterialData.Specular = DXVector4(0.4f, 0.4f, 0.4f, 16.0f);
	m_MaterialData.Reflect = DXVector4(0.4f, 0.4f, 0.4f, 1.0f);

	m_TextureSpace =  DXMatrix4X4 (
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
}

Material::~Material()
{

}

void Material::Update(DXMatrix4X4 world, DXMatrix4X4 view, DXMatrix4X4 proj)
{
	if (Is_DiffuseMap)
	{
		m_Shader->SetDiffuseMap(m_DiffuseMapSRV);
	}
	if (Is_NormalMap)
	{
		m_Shader->SetNormalMap(m_NormalMapSRV);
	}
	if (Is_CubeMap)
	{
		m_Shader->SetCubeMap(m_CubeMapSRV);
	}
	if (Is_ShadowMap)
	{
		m_ShadowData.gShadowTransform = world * g_ShadowTranspose * m_TextureSpace;
		
		m_Shader->SetVertexConstantBuffer(m_ShadowData);
	}

	// Material ID Update
	m_Shader->SetPixelConstantBuffer(m_ResourceID);

	// Vertex Shader Update
	m_Shader->VSUpdate();

	// Pixel Shader Update
	m_Shader->PSUpdate();
}

void Material::SetMaterialData(MaterialData materialData)
{
	//m_MaterialData = materialData;

	m_MaterialData.Ambient = DXVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_MaterialData.Diffuse = DXVector4(0.8f, 0.8f, 0.8f, 1.0f);
	m_MaterialData.Specular = DXVector4(0.4f, 0.4f, 0.4f, 16.0f);
	m_MaterialData.Reflect = DXVector4(0.4f, 0.4f, 0.4f, 1.0f);
}

KH_API void Material::SetDiffuseMap(ID3D11ShaderResourceView* diffuseMap)
{
	Is_DiffuseMap = true;
	m_DiffuseMapSRV = diffuseMap;
}

KH_API void Material::SetNormalMap(ID3D11ShaderResourceView* normalMap)
{
	Is_NormalMap = true;
	m_NormalMapSRV = normalMap;
}

KH_API void Material::SetCubeMap(ID3D11ShaderResourceView* cubeMap)
{
	Is_CubeMap = true;
	m_CubeMapSRV = cubeMap;
}

KH_API void Material::SetTexTransform(DXMatrix4X4 texTransform)
{
	m_TexTranspose = texTransform;
}

void Material::SetShadowTransform(DXMatrix4X4 shadowTransform)
{
	g_ShadowTranspose = shadowTransform;
}

void Material::SetShader(string shaderName)
{
	m_Shader = ResourceManager::GetInstance()->GetShader(shaderName);
}

void Material::SetMaterialID(UINT matID)
{
	m_ResourceID.gMatID = matID;
}

MaterialData Material::GetMaterialData()
{
	return m_MaterialData;
}

int Material::GetMaterialID()
{
	return m_ResourceID.gMatID;
}

Shader* Material::GetShader()
{
	return m_Shader;
}

DXMatrix4X4 Material::GetTexTransform()
{
	return m_TexTranspose;
}

KH_API ID3D11ShaderResourceView* Material::GetDiffuseMap()
{
	if (Is_DiffuseMap)
		return m_DiffuseMapSRV;
	else
		return nullptr;
}

KH_API ID3D11ShaderResourceView* Material::GetNormalMap()
{
	if (Is_NormalMap)
		return m_NormalMapSRV;
	else
		return nullptr;
}

KH_API ID3D11ShaderResourceView* Material::GetCubeMap()
{
	if (Is_CubeMap)
		return m_CubeMapSRV;
	else
		return nullptr;
}