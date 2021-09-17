#pragma once
class Material : public Object
{
public:
	Material();
	~Material();

private:
	MaterialData m_MaterialData;

	Shader* m_Shader;

	ID3D11ShaderResourceView* m_DiffuseMapSRV;
	ID3D11ShaderResourceView* m_NormalMapSRV;
	ID3D11ShaderResourceView* m_CubeMapSRV;

	DXMatrix4X4 m_TexTranspose;

	// Material ShadowTransform 공유 데이터..
	static DXMatrix4X4 g_ShadowTranspose;

	// Shader Data
	cbMaterial m_MatData;
	cbShadow m_ShadowData;

	cbID m_ResourceID;

	DXMatrix4X4 m_TextureSpace;

public:
	bool Is_DiffuseMap;
	bool Is_NormalMap;
	bool Is_CubeMap;
	bool Is_ShadowMap;

	string m_Name;

public:
	void Update(DXMatrix4X4 world, DXMatrix4X4 view, DXMatrix4X4 proj);

	void SetMaterialData(MaterialData materialData);
	KH_API void SetDiffuseMap(ID3D11ShaderResourceView* diffuseMap);
	KH_API void SetNormalMap(ID3D11ShaderResourceView* normalMap);
	KH_API void SetCubeMap(ID3D11ShaderResourceView* cubeMap);
	KH_API void SetTexTransform(DXMatrix4X4 texTransform);
	void SetShader(string shaderName);
	void SetMaterialID(UINT matID);	

	template <typename T>
	void SetVertexConstantBuffer(T data);

	template <typename T>
	void SetPixelConstantBuffer(T data);

	static void SetShadowTransform(DXMatrix4X4 shadowTransform);
	static DXMatrix4X4 GetShadowTransform() { return g_ShadowTranspose; }

	int GetMaterialID();
	MaterialData GetMaterialData();
	Shader* GetShader();
	DXMatrix4X4 GetTexTransform();

	KH_API ID3D11ShaderResourceView* GetDiffuseMap();
	KH_API ID3D11ShaderResourceView* GetNormalMap();
	KH_API ID3D11ShaderResourceView* GetCubeMap();
};

template <typename T>
void Material::SetVertexConstantBuffer(T data)
{
	m_Shader->SetVertexConstantBuffer(data);
}

template <typename T>
void Material::SetPixelConstantBuffer(T data)
{
	m_Shader->SetPixelConstantBuffer(data);
}

