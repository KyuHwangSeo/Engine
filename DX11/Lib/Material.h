
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

	std::string m_Name;

public:
	void Update(DXMatrix4X4 world, DXMatrix4X4 view, DXMatrix4X4 proj);

	void SetMaterialData(MaterialData materialData);
	ENGINE_DLL void SetDiffuseMap(ID3D11ShaderResourceView* diffuseMap);
	ENGINE_DLL void SetNormalMap(ID3D11ShaderResourceView* normalMap);
	ENGINE_DLL void SetCubeMap(ID3D11ShaderResourceView* cubeMap);
	ENGINE_DLL void SetTexTransform(DXMatrix4X4 texTransform);
	void SetShader(Shader* shader);
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

	ENGINE_DLL ID3D11ShaderResourceView* GetDiffuseMap();
	ENGINE_DLL ID3D11ShaderResourceView* GetNormalMap();
	ENGINE_DLL ID3D11ShaderResourceView* GetCubeMap();
};

template <typename T>
inline void Material::SetVertexConstantBuffer(T data)
{
	m_Shader->SetVertexConstantBuffer(data);
}

template <typename T>
inline void Material::SetPixelConstantBuffer(T data)
{
	m_Shader->SetPixelConstantBuffer(data);
}

