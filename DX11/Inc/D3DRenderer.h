#pragma once
/// <summary>
/// D3DRenderer
/// </summary>
/// 
/// 최종 Renderer
/// 실질적으로 Rendering 작업 순서, 종류를 컨트롤 할 것이다..
/// Device 클래스에 통합되있던 Renderer만 별도로 분리한 클래스
/// 추후에 라이브러리로 뺄 예정

class D3DRenderer
{
public:
	D3DRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context, ComPtr<IDXGISwapChain> swapchain);
	~D3DRenderer();

public:
	ComPtr<ID3D11Device> m_Device = nullptr;
	ComPtr<ID3D11DeviceContext> m_DeviceContext = nullptr;
	ComPtr<IDXGISwapChain> m_SwapChain = nullptr;

	ID3D11RenderTargetView* m_RenderTarget = nullptr;
	ID3D11DepthStencilView* m_DepthStencilView = nullptr;
	ID3D11DepthStencilState* m_DepthStencilState = nullptr;

	ComPtr<ID3D11BlendState> m_AlphaBlend = nullptr;

	DepthStencilManager* m_DSManger;

	Forward* m_Forward;
	PostProcessing* m_PostProcessing;
	Shadow* m_Shadow;
	Deferred* m_Deferred;
	SSAO* m_SSAO;
	MotionBlur* m_MotionBlur;
	DebugViewPort* m_DebugViewPort;

	vector<IRenderer*> m_RendererList;

	VertexBuffer* m_QuadBuffer;
public:
	bool Is_Blur = false;
	bool Is_Shadow = true;
	bool Is_Debug = false;
	bool Is_SSAO = true;
	bool Is_Deferred = true;

public:
	void Initialize();

	void MainRender();

	// SSAO Render
	void SSAORender();

	// Post Render
	void PostRender(RenderTarget* finalRT, RenderTarget* horzRT, RenderTarget* vertRT);
	void FullScreenBlur();

	// Shadow Render
	void ShadowRender();

	// Deferred Render
	void DeferredRender();

	// Motion Blur Render
	void MotionBlurRender();

	// Debug Render
	void DebugRender();

	// Reset RenderTarget
	void ResetSSAO();

	// UI Render
	void AlphaRender();

	void EndRender();

	void OnResize(int width, int height);
};

