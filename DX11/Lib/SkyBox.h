#pragma once
/// <summary>
/// SkyBox
/// </summary>
/// 
/// - SkyCube 전용 클래스
/// - 설정값이나 Render 방식이 달라 다른 클래스로 관리
/// 

class SkyBox : public Object
{
public:
	SkyBox();
	~SkyBox();

public:
	void Render(DXMatrix4X4 view, DXMatrix4X4 proj);
	void SetVertexBuffer(VertexBuffer* vBuffer);
	void SetCubeMap(ID3D11ShaderResourceView* cubeMap);

	void SetEnabled(bool enabled) { m_Enable = enabled; };
	bool GetEnabled() { return m_Enable; };

private:
	bool m_Enable;

	ComPtr<ID3D11DeviceContext> m_DeviceContext;
	ComPtr<ID3D11DepthStencilState> m_SkyDeptStencil;
	ComPtr<ID3D11DepthStencilState> m_BeforDeptStencil;
	ComPtr<ID3D11ShaderResourceView> m_CubeMapSRV;

	ComPtr<ID3D11Buffer> m_VB;		// 버텍스버퍼
	ComPtr<ID3D11Buffer> m_IB;		// 인덱스버퍼

	UINT m_IndexCount;
	UINT m_Stride;
	UINT m_Offset;

	Camera** m_Camera;

	Shader* m_Shader;

	cbPerObject m_ObjectData;
};

