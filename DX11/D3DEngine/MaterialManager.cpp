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
	m_Shader = ResourceManager::GetInstance()->GetShader("LightShader");
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
	// ������ Material üũ
	for (auto& matData : m_MaterialList)
	{
		if (matData.second->GetMaterialData() == mat->GetMaterialData())
		{
			mat->SetMaterialID(matData.second->GetMaterialID());
			return;
		}
	}

	// �ش� �ε���(ID)�� MaterialData ����..
	m_MatData.gMaterials[m_CountID] = mat->GetMaterialData();
	mat->SetMaterialID(m_CountID);

	// Material List �߰�..
	m_MaterialList.insert(make_pair(m_CountID++, mat));

	// Pixel Shader Share Constant Buffer Update..
	m_Shader->SetPixelConstantBuffer(m_MatData);
}

void MaterialManager::Release()
{
	m_MaterialList.clear();
}