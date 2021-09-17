#pragma once
/// <summary>
/// Shadow Class
/// </summary>
/// 
/// Shadow Render ���� Ŭ����
/// Shadow�� ������ ���� �ʿ䰡 �����Ƿ� RenderTarget�� ���ε����� �ʴ´�
/// ��, SRV���� Depth ���� ������ �ȴ� (Light ����)

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

