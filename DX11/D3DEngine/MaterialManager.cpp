#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Object.h"
#include "Shader.h"
#include "Material.h"
#include "MaterialManager.h"
#include "ResourceManager.h"

MaterialManager::MaterialManager()
	:m_CountID(0)
{
	m_Shader = D3DEngine::GetInstance()->GetResourceManager()->GetShader("LightShader");

	MaterialData mat;
	mat.Ambient = DXVector4(1.0f, 1.0f, 1.0f, 1.0f);
	mat.Diffuse = DXVector4(0.8f, 0.8f, 0.8f, 1.0f);
	mat.Specular = DXVector4(0.4f, 0.4f, 0.4f, 16.0f);
	mat.Reflect = DXVector4(0.4f, 0.4f, 0.4f, 1.0f);

	m_MatData.gMaterials[m_CountID++] = mat;
}

MaterialManager::~MaterialManager()
{
	m_MaterialList.clear();
}

void MaterialManager::Update(float dTime)
{
	m_Shader->SetPixelConstantBuffer(m_MatData);
}

void MaterialManager::AddMaterial(Material* mat)
{
	// 동일한 Material 체크
	for (auto& matData : m_MaterialList)
	{
		if (matData.second->GetMaterialData() == mat->GetMaterialData())
		{
			mat->SetMaterialID(matData.second->GetMaterialID());
			return;
		}
	}

	// 해당 인덱스(ID)에 MaterialData 삽입..
	m_MatData.gMaterials[m_CountID] = mat->GetMaterialData();
	mat->SetMaterialID(m_CountID);

	// Material List 추가..
	m_MaterialList.insert(make_pair(m_CountID++, mat));

	// Pixel Shader Share Constant Buffer Update..
	m_Shader->SetPixelConstantBuffer(m_MatData);
}

void MaterialManager::Release()
{
	m_MaterialList.clear();
}
