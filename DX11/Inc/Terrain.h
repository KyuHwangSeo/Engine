#pragma once
typedef ComPtr<ID3D11ShaderResourceView> Texture;

struct MaterialLayer
{
	MaterialLayer() = default;
	MaterialLayer(Texture diffuse, Texture normalmap) : m_DiffuseTex(diffuse), m_NormalMapTex(normalmap) {}

	Texture m_DiffuseTex;
	Texture m_NormalMapTex;
};

struct TerrainLayer
{
	TerrainLayer(Texture mask) : m_MaskTex(mask) {}

	Texture m_MaskTex;
	std::vector<MaterialLayer> m_MatList;
};

class Terrain : public Component
{
public:
	Terrain() = default;
	~Terrain() = default;

public:
	void Start();
	void ShadowRender(DXMatrix4X4 view, DXMatrix4X4 proj);
	void Render(DXMatrix4X4 view, DXMatrix4X4 proj);
	void Release();

	void SetVertexBuffer(VertexBuffer* vBuffer);
	void SetShadowShader(Shader* shader);
	void SetShader(Shader* shader);
	void AddLayer(Texture mask, MaterialLayer& channel_R, MaterialLayer& channel_G, MaterialLayer& channel_B, MaterialLayer& channel_A);
	void AddLayer(Texture mask, MaterialLayer& channel_R, MaterialLayer& channel_G, MaterialLayer& channel_B);
	void AddLayer(Texture mask, MaterialLayer& channel_R, MaterialLayer& channel_G);
	void AddLayer(Texture mask, MaterialLayer& channel_R);

private:
	ComPtr<ID3D11DeviceContext> m_DeviceContext;

	ComPtr<ID3D11Buffer> m_VB;		// 버텍스버퍼
	ComPtr<ID3D11Buffer> m_IB;		// 인덱스버퍼

	UINT m_IndexCount;
	UINT m_Stride;
	UINT m_Offset;

	// Shader Data
	cbMaterial m_MatData;
	cbShadow m_ShadowData;
	cbPerObject m_ShadowObjectData;
	cbID m_ResourceID;

	DXMatrix4X4 m_TextureSpace;
	DXMatrix4X4 m_ShadowTransform;

	Shader* m_ShadowShader;
	Shader* m_TerrainShader;

	cbPerObject m_ObjectData;

	std::vector<TerrainLayer*> m_LayerList;
};

