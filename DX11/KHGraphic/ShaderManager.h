#pragma once
#include "ShaderManagerBase.h"

class ShaderManager : public IShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

public:
	friend class OriginalShader;

public:
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) override;
	void AddSampler(Hash_Code hash_code, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler) override;
	void Release() override;

public:
	IShader* LoadShader(eShaderType shaderType, std::string shaderName);
	OriginalShader GetShader(std::string shaderName) override;

private:
	VertexShader* GetVertexShader(std::string shaderName);
	PixelShader* GetPixelShader(std::string shaderName);
	ComputeShader* GetComputeShader(std::string shaderName);

	void CreateShader();
	void SetSampler();

private:
	// SamplerState List
	std::unordered_map<Hash_Code, Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_SamplerList;

	// Shader List
	std::unordered_map<std::string, IShader*> m_ShaderList;
};
