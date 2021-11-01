#pragma once
class D3D11Renderer
{
public:
	D3D11Renderer();
	~D3D11Renderer();

public:
	bool Initialize(HWND hwnd, int screenWidth, int screenHeight);

	void CreateDevice(HWND hwnd);

	void CreateIndexBuffer();
	void CreateVertexBuffer();
	void CreateTextureBuffer();

	void Render();

	void ShadowRender();
	void MainRender();
	void SSAORender();
	void DeferredRender();
	void AlphaRender();

private:
	ComPtr<ID3D11Device> m_Device = nullptr;
	ComPtr<ID3D11DeviceContext> m_DeviceContext = nullptr;
	ComPtr<IDXGISwapChain> m_SwapChain = nullptr;

private:
	POINT* m_ScreenSize;

private:
	ResourceManager* m_RM;
};

