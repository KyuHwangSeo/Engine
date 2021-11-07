#pragma once
///
/// 2021/11/07 23:00
/// SeoKyuHwang
///
/// ShaderManager Interface Class
///
/// - Shader Resource�� �����ϴ� Class

typedef size_t Hash_Code;

class IShader;
interface IShaderManager
{
public:
	virtual void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) abstract;
	virtual void AddSampler(Hash_Code hash_code, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler) abstract;

public:
	virtual IShader* GetShader(std::string shaderName) abstract;
};