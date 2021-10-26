#pragma once
/// <summary>
/// PostProcessing Class
/// </summary>
/// 
/// PostProcessing Render ��� Ŭ����
/// PostProcessing�� �ʿ��� �۾����� ���ش�

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

