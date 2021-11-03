#pragma once
#include "ShaderManagerBase.h"

class ShaderManager : public IShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

public:
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) override;

public:
	IShader* GetShader(std::string shaderName) override;

private:
	void CreateShader();
	void LoadShader(ShaderType shaderType, std::string shaderName);
	
	void CreateSampler(Microsoft::WRL::ComPtr<ID3D11Device> device);
	void SetSampler();

private:
	// SamplerState List
	std::unordered_map<Hash_Code, Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_SamplerList;

	// Shader List
	std::unordered_map<std::string, IShader*> m_ShaderList;
};