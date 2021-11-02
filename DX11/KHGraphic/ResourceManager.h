#pragma once
#include "ResourceManagerBase.h"

class ResourceManager : public IResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

public:
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

public:
	IShader* GetShader(std::string shaderName);

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