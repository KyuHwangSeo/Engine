#pragma once
typedef ComPtr<ID3D11ShaderResourceView> Texture;

struct TerrainLayer
{
	TerrainLayer(Texture mask, Texture diffuse, Texture normalmap) : m_MaskTex(mask), m_DiffuseTex(diffuse), m_NormalMapTex(normalmap) {}

	Texture m_MaskTex;
	Texture m_DiffuseTex;
	Texture m_NormalMapTex;
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
	void SetShader(Shader* shader);
	void AddLayer(Texture mask, Texture diffuse, Texture normalmap);

private:
	ComPtr<ID3D11DeviceContext> m_DeviceContext;

	ComPtr<ID3D11Buffer> m_VB;		// 버텍스버퍼
	ComPtr<ID3D11Buffer> m_IB;		// 인덱스버퍼

	UINT m_IndexCount;
	UINT m_Stride;
	UINT m_Offset;

	Shader* m_TerrainShader;

	cbPerObject m_ObjectData;

	std::vector<TerrainLayer*> m_LayerList;
};

