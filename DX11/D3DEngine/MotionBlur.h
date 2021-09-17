#pragma once
///
/// 2021/07/29 22:38
/// SeoKyuHwang
///
/// MotionBlur Class
///
/// - MotionBlur 전용 Class
/// - Camera, Object MotionBlur 지원
/// - 구현중

class MotionBlur : public IRenderer
{
public:
	MotionBlur(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context);
	~MotionBlur();

public:
	RenderTarget* m_MotionBlurRT;
	RenderTarget* m_VelocityRT;

private:
	Shader* m_VelocityShader;
	Shader* m_MotionBlurShader;

	cbFullScreen m_ScreenData;
	cbPerObject m_ObjectData;

	VertexBuffer* m_QuadBuffer;

public:
	void VelocityRender(ID3D11ShaderResourceView* depthSRV);
	void MotionBlurRender(ID3D11ShaderResourceView* screenSRV);
	void OnResize(int width, int height);
};

