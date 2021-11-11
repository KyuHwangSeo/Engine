#pragma once
#include <string>

///
/// 2021/11/07 23:00
/// SeoKyuHwang
///
/// ShaderManager Interface Class
///
/// - Shader Resource를 관리하는 Class

typedef size_t Hash_Code;

class VertexShader;
class PixelShader;
class ComputeShader;
class ShaderType;

interface IShaderManager
{
public:
	virtual void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) abstract;
	virtual void AddSampler(Hash_Code hash_code, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler) abstract;
	virtual void Release() abstract;

private:
	virtual VertexShader* GetVertexShader(std::string shaderName) abstract;
	virtual PixelShader* GetPixelShader(std::string shaderName) abstract;
	virtual ComputeShader* GetComputeShader(std::string shaderName) abstract;

public:
	friend class ShaderType;

public:
	ShaderType GetShader(std::string shaderName);
};

class ShaderType
{
public:
	IShaderManager* pThis;
	std::string shaderName;

	operator VertexShader* ()
	{
		return pThis->GetVertexShader(shaderName);
	}
	operator PixelShader* ()
	{
		return pThis->GetPixelShader(shaderName);
	}
	operator ComputeShader* ()
	{
		return pThis->GetComputeShader(shaderName);
	}
};
