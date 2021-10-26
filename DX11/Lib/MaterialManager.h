#pragma once
/// <summary>
/// MaterialManager Class
/// </summary>
/// 
/// Render 방식을 Deferred로 가면서 Material List를 관리할 클래스가 필요해서 생성.
/// 현재 각각의 Material은 고유 ID 값을 가지고 있으며 
/// 부여받은 ID를 통해 Shader에서 해당 Material을 적용하는 방식.

class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();

public:
	void Update(float dTime);
	void AddMaterial(Material* mat);

	void Release();

private:
	unordered_map<int, Material*> m_MaterialList;

	cbMaterialList m_MatData;
	Shader* m_Shader;

	int m_CountID;
};