#pragma once

class IShader;
class IResourceManager
{
public:
	IResourceManager() = default;
	virtual ~IResourceManager() = default;

public:
	virtual void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) abstract;

public:
	virtual IShader* GetShader(std::string shaderName) abstract;
};