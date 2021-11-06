#pragma once
#include "ShaderManagerBase.h"
#include "ShaderBase.h"

class IGraphicResourceManager;
class GraphicResourceManager;
class ShaderManager : public IShaderManager
{
public:
	ShaderManager(IGraphicResourceManager* manager);
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
	GraphicResourceManager* m_ResourceManager;

	// SamplerState List
	std::vector<Hash_Code> m_SamplerHashList;

	// Shader List
	std::unordered_map<std::string, IShader*> m_ShaderList;
};