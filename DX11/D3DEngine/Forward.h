#pragma once
/// <summary>
/// Forward Class
/// </summary>
/// 
/// Forward Render 방식 클래스
/// 기존의 Render 방식을 따로 빼서 관리한다
/// Deferred와 구분하기 위함

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

	// 최종 출력을 위한 Rendering 방식
	void FinalRender(ID3D11ShaderResourceView* srv);

	D3D11_VIEWPORT& GetViewPort() { return m_ForwardViewport; }
};

