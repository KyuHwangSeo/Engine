#pragma once
/// <summary>
/// PostProcessing Class
/// </summary>
/// 
/// PostProcessing Render 방식 클래스
/// PostProcessing에 필요한 작업들을 해준다

class PostProcessing : public IRenderer
{
public:
	PostProcessing(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context);
	~PostProcessing();

private:
	Shader* HorzBlurShader;
	Shader* VertBlurShader;

public:
	void OnResize(int width, int height);
	void Render(RenderTarget* vertRT, RenderTarget* horzRT);
};

