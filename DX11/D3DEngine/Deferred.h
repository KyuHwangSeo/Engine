#pragma once
/// <summary>
/// Deferred Class
/// </summary>
/// 
/// Deferred Render ���� Ŭ����
/// ���� ��½� Light������ �����Ѵ�
/// Light�� ������ �� ���̵� O(1)�� Pixel �۾��� �̷������

class Deferred : public IRenderer
{
public:
	Deferred(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context);
	~Deferred();

public:
	RenderTarget* m_AlbedoRT;
	RenderTarget* m_NormalRT;
	RenderTarget* m_PositionRT;
	RenderTarget* m_LightRT;
	RenderTarget* m_ShadowRT;
	RenderTarget* m_NormalDepthRT;
	RenderTarget* m_DepthRT;

private:
	vector<ID3D11RenderTargetView*> m_DeferredRTV;
	vector<ID3D11ShaderResourceView*> m_DeferredSRV;

	Shader* m_LightShader;

	VertexBuffer* m_QuadBuffer;
public:
	void BeginRender(ID3D11DepthStencilView* dsv);
	void LightRender(ID3D11ShaderResourceView* ssaoSRV);
	void OnResize(int width, int height);

	ID3D11RenderTargetView* GetDeferredRTV(int index)		{ return m_DeferredRTV[index]; }
	ID3D11ShaderResourceView* GetDeferredSRV(int index)		{ return m_DeferredSRV[index]; }
};

