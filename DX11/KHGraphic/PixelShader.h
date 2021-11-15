#pragma once

/// <summary>
/// PixelShader Class
/// </summary>
/// 
/// - 한개의 PixelShader를 관리하는 클래스

class PixelShader : public ShaderBase
{
public:
	PixelShader(const char* fileName);
	~PixelShader();

public:
	void LoadShader(std::string fileName) override;
	void Update() override;
	void Release() override;

private:
	// Pixel Shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;
};