#pragma once
typedef ComPtr<ID3D11ShaderResourceView> Texture;

struct MaterialLayer
{
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
	void SetShader(Shader* shader);
	void AddLayer(Texture mask, MaterialLayer& layer1, MaterialLayer& layer2, MaterialLayer& layer3);
	void AddLayer(Texture mask, MaterialLayer& layer1, MaterialLayer& layer2);
	void AddLayer(Texture mask, MaterialLayer& layer1);

private:
	ComPtr<ID3D11DeviceContext> m_DeviceContext;

	ComPtr<ID3D11Buffer> m_VB;		// ���ؽ�����
	ComPtr<ID3D11Buffer> m_IB;		// �ε�������

	UINT m_IndexCount;
	UINT m_Stride;
	UINT m_Offset;

	Shader* m_TerrainShader;

	cbPerObject m_ObjectData;

	std::vector<TerrainLayer*> m_LayerList;
};

