#pragma once
#include "ShaderManagerBase.h"
#include "ShaderBase.h"

class ShaderManager : public IShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

public:
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) override;
	void AddSampler(Hash_Code hash_code, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler) override;

public:
	VertexShader* GetVertexShader(std::string shaderName) override;
	PixelShader* GetPixelShader(std::string shaderName) override;
	ComputeShader* GetComputeShader(std::string shaderName) override;

private:
	void CreateShader();
	void LoadShader(eShaderType shaderType, std::string shaderName);
	
	void SetSampler();

private:
	// SamplerState List
	std::unordered_map<Hash_Code, Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_SamplerList;

	// Shader List
	std::unordered_map<std::string, IShader*> m_ShaderList;
};