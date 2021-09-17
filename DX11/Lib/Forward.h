#pragma once
/// <summary>
/// Forward Class
/// </summary>
/// 
/// Forward Render ��� Ŭ����
/// ������ Render ����� ���� ���� �����Ѵ�
/// Deferred�� �����ϱ� ����

class Forward : public IRenderer
{
public:
	Forward(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context, ComPtr<IDXGISwapChain> swapchain);
	~Forward();

public:
	RenderTarget* m_ForwardRT;
	RenderTarget* m_AllHorzBlurRT;
	RenderTarget* m_AllVertBlurRT;

	ID3D11RenderTargetView* m_ForwardRTV;
private:
	ComPtr<IDXGISwapChain> m_SwapChain;

	D3D11_VIEWPORT m_ForwardViewport = { 0, };

	Shader* m_FinalShader;
	cbPerObject m_ObjectData;

	VertexBuffer* m_QuadBuffer;

public:
	void BeginRender(ID3D11DepthStencilView* dsv);
	void OnResize(int width, int height);

	// ���� ����� ���� Rendering ���
	void FinalRender(ID3D11ShaderResourceView* srv);

	D3D11_VIEWPORT& GetViewPort() { return m_ForwardViewport; }
};

