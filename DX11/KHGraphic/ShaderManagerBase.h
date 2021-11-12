#pragma once
#include <string>
#include "ShaderTypes.h"

///
/// 2021/11/07 23:00
/// SeoKyuHwang
///
/// ShaderManager Interface Class
///
/// - Shader Resource를 관리하는 Class

typedef size_t Hash_Code;

interface IShaderManager
{
public:
	virtual void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) abstract;
	virtual void AddSampler(Hash_Code hash_code, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler) abstract;
	virtual void Release() abstract;

public:
	// GetShader Original Pointer..
	virtual OriginalShader GetShader(std::string shaderName) abstract;
};