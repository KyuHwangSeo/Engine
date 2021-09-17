#pragma once
/// <summary>
/// MaterialManager Class
/// </summary>
/// 
/// Render ����� Deferred�� ���鼭 Material List�� ������ Ŭ������ �ʿ��ؼ� ����.
/// ���� ������ Material�� ���� ID ���� ������ ������ 
/// �ο����� ID�� ���� Shader���� �ش� Material�� �����ϴ� ���.

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