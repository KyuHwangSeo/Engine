#include "DirectDefine.h"
#include "ShaderManagerBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "ShaderManager.h"

#include "ResourceBufferHashTable.h"

#include "Texture2D.h"
#include "RenderTargetView.h"
#include "ShaderResourceView.h"
#include "UnorderedAccessView.h"
#include "DepthStecilView.h"
#include "ResourceManager.h"

using namespace Microsoft::WRL;

ShaderManager::ShaderManager(IGraphicResourceManager* manager)
{
	m_ResourceManager = reinterpret_cast<GraphicResourceManager*>(manager);
}

ShaderManager::~ShaderManager()
{

}

void ShaderManager::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	// Shader Global Initialize..
	IShader::Initialize(device, context);
	IShader::SetShaderRoute("../Resource/Shader/");

	// Shader Hash Table Initialize..
	ShaderResourceHashTable::Initialize();

	// SamplerState Create..
	CreateSampler(device);

	// Global Shader Create..
	CreateShader();

	// Shader Hash Table Reset..
	ShaderResourceHashTable::Reset();
}

IShader* ShaderManager::GetShader(std::string shaderName)
{
	std::unordered_map<std::string, IShader*>::iterator shader = m_ShaderList.find(shaderName);

	// 해당 Shader를 찾았을 경우..
	if (shader != m_ShaderList.end()) 
		return shader->second;

	return nullptr;
}

void ShaderManager::CreateSampler(ComPtr<ID3D11Device> device)
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

	HR(device->CreateSamplerState(&samplerDesc, &samplerState));
	m_SamplerHashList.push_back(samClampMinLinear::GetHashCode());
	m_ResourceManager->AddResource(samplerState);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HR(device->CreateSamplerState(&samplerDesc, &samplerState));
	m_SamplerHashList.push_back(samWrapAnisotropic::GetHashCode());
	m_ResourceManager->AddResource(samplerState);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HR(device->CreateSamplerState(&samplerDesc, &samplerState));
	m_SamplerHashList.push_back(samWrapMinLinear::GetHashCode());
	m_ResourceManager->AddResource(samplerState);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HR(device->CreateSamplerState(&samplerDesc, &samplerState));
	m_SamplerHashList.push_back(samMirrorMinLinear::GetHashCode());
	m_ResourceManager->AddResource(samplerState);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HR(device->CreateSamplerState(&samplerDesc, &samplerState));
	m_SamplerHashList.push_back(samClampMinLinearPoint::GetHashCode());
	m_ResourceManager->AddResource(samplerState);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1e5f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HR(device->CreateSamplerState(&samplerDesc, &samplerState));
	m_SamplerHashList.push_back(samBorderLinerPoint::GetHashCode());
	m_ResourceManager->AddResource(samplerState);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HR(device->CreateSamplerState(&samplerDesc, &samplerState));
	m_SamplerHashList.push_back(samWrapLinerPoint::GetHashCode());
	m_ResourceManager->AddResource(samplerState);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HR(device->CreateSamplerState(&samplerDesc, &samplerState));
	m_SamplerHashList.push_back(gShadowSam::GetHashCode());
	m_ResourceManager->AddResource(samplerState);
}

void ShaderManager::CreateShader()
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

	// Shader Sampler 설정..
	SetSampler();
}

void ShaderManager::LoadShader(ShaderType shaderType, std::string shaderName)
{
	// Shader Type에 맞는 Shader 생성..
	IShader* newShader = IShader::CreateShader(shaderType, shaderName.c_str());

	// 파일을 제대로 읽지 못하여 생성하지 못한경우 nullptr..
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

void ShaderManager::SetSampler()
{
	// Pixel & Compute Shader Sampler 설정..
	for (std::pair<std::string, IShader*> shader : m_ShaderList)
	{
		// Shader Type에 따른 형은 보장이 되므로 강제 형변환 실행 후 Sampler 설정..
		switch (shader.second->GetType())
		{
		case ShaderType::PIXEL:
		{
			PixelShader* pShader = reinterpret_cast<PixelShader*>(shader.second);
			for (size_t index = 0; index < m_SamplerHashList.size(); index++)
			{
				pShader->SetSamplerState(m_SamplerHashList[index], m_ResourceManager->m_SSList[index]);
			}
		}
		break;
		case ShaderType::COMPUTE:
		{
			ComputeShader* cShader = reinterpret_cast<ComputeShader*>(shader.second);
			for (size_t index = 0; index < m_SamplerHashList.size(); index++)
			{
				cShader->SetSamplerState(m_SamplerHashList[index], m_ResourceManager->m_SSList[index]);
			}
		}
		break;
		default:
			break;
		}
	}
}
