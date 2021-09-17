#pragma once
class DebugViewPort : public IRenderer
{
public:
	DebugViewPort(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context);
	~DebugViewPort();

private:
	D3D11_VIEWPORT m_DebugViewport[7];

public:
	void OnResize(int width, int height);
	void SetViewPort(int count, int viewportNum);
};

