#include "ResourceManager.h"
#include "ShaderResourceHash.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"

using namespace Microsoft::WRL;

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::Initialize(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context)
{
	// Shader Global Initialize..
	ShaderBase::Initialize(device, context);
	ShaderBase::SetShaderRoute("../Resource/Shader/");

	// Shader Hash Table Initialize..
	ShaderResourceHashTable::Initialize();

	// SamplerState Create..
	CreateSampler(device);

	// Global Shader Create..
	CreateShader();
}

void ResourceManager::CreateSampler(ComPtr<ID3D11Device> device)
{
	// SamplerState 생성..
	ComPtr<ID3D11SamplerState> samplerState;

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &samplerState);
	m_SamplerList.insert(std::make_pair("samClampMinLinear", samplerState));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &samplerState);
	m_SamplerList.insert(std::make_pair("samWrapAnisotropic", samplerState));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &samplerState);
	m_SamplerList.insert(std::make_pair("samWrapMinLinear", samplerState));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &samplerState);
	m_SamplerList.insert(std::make_pair("samMirrorMinLinear", samplerState));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &samplerState);
	m_SamplerList.insert(std::make_pair("samClampMinLinearPoint", samplerState));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1e5f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &samplerState);
	m_SamplerList.insert(std::make_pair("samBorderLinerPoint", samplerState));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &samplerState);
	m_SamplerList.insert(std::make_pair("samWrapLinerPoint", samplerState));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &samplerState);
	m_SamplerList.insert(std::make_pair("gShadowSam", samplerState));
}

void ResourceManager::CreateShader()
{
	// Global Forward Shader
	LoadShader(ShaderType::VERTEX, "FinalVS.cso");
	LoadShader(ShaderType::PIXEL, "FinalPS.cso");

	// Global Deferred Shader
	LoadShader(ShaderType::VERTEX, "FullScreenVS.cso");
	LoadShader(ShaderType::PIXEL, "LightPS.cso");

	LoadShader(ShaderType::VERTEX, "ColorVS.cso");
	LoadShader(ShaderType::PIXEL, "ColorPS.cso");

	LoadShader(ShaderType::VERTEX, "SkyCubeVS.cso");
	LoadShader(ShaderType::PIXEL, "SkyCubePS.cso");

	LoadShader(ShaderType::VERTEX, "NormalShadowVS.cso");
	LoadShader(ShaderType::VERTEX, "SkinShadowVS.cso");

	LoadShader(ShaderType::VERTEX, "TextureVS.cso");
	LoadShader(ShaderType::VERTEX, "SkinVS.cso");
	LoadShader(ShaderType::PIXEL, "TextureDeferredPS.cso");

	LoadShader(ShaderType::VERTEX, "NormalTextureVS.cso");
	LoadShader(ShaderType::PIXEL, "NormalTextureDeferredPS.cso");

	LoadShader(ShaderType::VERTEX, "NormalSkinVS.cso");
	LoadShader(ShaderType::PIXEL, "NormalTextureDeferredPS.cso");

	// SSAO Shader
	LoadShader(ShaderType::VERTEX, "SSAOVS.cso");
	LoadShader(ShaderType::PIXEL, "SSAOPS.cso");

	LoadShader(ShaderType::VERTEX, "SSAOBlurVS.cso");
	LoadShader(ShaderType::PIXEL, "SSAOHorizonBlurPS.cso");
	LoadShader(ShaderType::PIXEL, "SSAOVerticalBlurPS.cso");

	// Terrain Shader
	LoadShader(ShaderType::VERTEX, "TerrainVS.cso");
	LoadShader(ShaderType::PIXEL, "TerrainPS.cso");

	// Screen Blur Shader
	LoadShader(ShaderType::COMPUTE, "HorizonBlurCS.cso");
	LoadShader(ShaderType::COMPUTE, "VerticalBlurCS.cso");
}

void ResourceManager::LoadShader(ShaderType shaderType, std::string shaderName)
{
	ShaderBase* newShader = ShaderBase::CreateShader(shaderType, shaderName.c_str());

	if (newShader == nullptr)
		return throw std::exception("ERROR: Can not Create Shader.\n");

	std::string shaderKey(shaderName);
	size_t pointPosition = shaderName.rfind(".");

	// Shader File Name 기준 Key 설정..
	if (pointPosition != std::string::npos)
		shaderKey = shaderName.substr(0, pointPosition - 1);

	// 새로 생성한 Shader 삽입..
	m_ShaderList.insert(std::make_pair(shaderKey, newShader));
}