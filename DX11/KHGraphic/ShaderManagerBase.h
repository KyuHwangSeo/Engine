#pragma once

class IShader;
class IShaderManager
{
public:
	IShaderManager() = default;
	virtual ~IShaderManager() = default;

public:
	virtual void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) abstract;

public:
	virtual IShader* GetShader(std::string shaderName) abstract;
};