#pragma once

class ShaderBase;
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

public:
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

private:
	void LoadShader(ShaderType shaderType, std::string shaderName);
	void CreateSampler(ComPtr<ID3D11Device> device);
	void CreateShader();

private:
	// SamplerState List
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_SamplerList;

	// Shader List
	std::unordered_map<std::string, ShaderBase*> m_ShaderList;
};

