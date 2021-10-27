#pragma once
/// <summary>
/// Shadow Class
/// </summary>
/// 
/// Shadow Render 전용 클래스
/// Shadow는 색상을 찍을 필요가 없으므로 RenderTarget을 바인딩하지 않는다
/// 즉, SRV에는 Depth 값만 찍히게 된다 (Light 기준)

class Shadow : public IRenderer
{
public:
	Shadow(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context);
	~Shadow();

public:
	RenderTarget* m_ShadowRT;

private:
	ComPtr<ID3D11DepthStencilView> m_ShadowDepthStencilView = nullptr;

	D3D11_VIEWPORT m_ShadowViewport = { 0, };

public:
	void OnResize(int width, int height);
	void Render();

	D3D11_VIEWPORT& GetViewPort() { return m_ShadowViewport; }
};

