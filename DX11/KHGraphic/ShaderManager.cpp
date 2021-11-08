#include "DirectDefine.h"
#include "ShaderManagerBase.h"
#include "ShaderBase.h"
#include "ShaderResourceBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "ShaderManager.h"
#include "ResourceBufferHashTable.h"

using namespace Microsoft::WRL;

ShaderManager::ShaderManager()
{

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

	// Global Shader Create..
	CreateShader();

	// Shader Hash Table Reset..
	ShaderResourceHashTable::Reset();
}

void ShaderManager::AddSampler(Hash_Code hash_code, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler)
{
	m_SamplerList.insert(std::make_pair(hash_code, sampler));
}

IShader* ShaderManager::GetShader(std::string shaderName)
{
	std::unordered_map<std::string, IShader*>::iterator shader = m_ShaderList.find(shaderName);

	// 해당 Shader를 찾았을 경우..
	if (shader != m_ShaderList.end()) 
		return shader->second;

	return nullptr;
}

void ShaderManager::CreateShader()
{
	// Global Forward Shader
	LoadShader(eShaderType::VERTEX, "FinalVS.cso");
	LoadShader(eShaderType::PIXEL, "FinalPS.cso");

	// Global Deferred Shader
	LoadShader(eShaderType::VERTEX, "FullScreenVS.cso");
	LoadShader(eShaderType::PIXEL, "LightPS.cso");

	LoadShader(eShaderType::VERTEX, "ColorVS.cso");
	LoadShader(eShaderType::PIXEL, "ColorPS.cso");

	LoadShader(eShaderType::VERTEX, "SkyCubeVS.cso");
	LoadShader(eShaderType::PIXEL, "SkyCubePS.cso");

	LoadShader(eShaderType::VERTEX, "NormalShadowVS.cso");
	LoadShader(eShaderType::VERTEX, "SkinShadowVS.cso");

	LoadShader(eShaderType::VERTEX, "TextureVS.cso");
	LoadShader(eShaderType::VERTEX, "SkinVS.cso");
	LoadShader(eShaderType::PIXEL, "TextureDeferredPS.cso");

	LoadShader(eShaderType::VERTEX, "NormalTextureVS.cso");
	LoadShader(eShaderType::PIXEL, "NormalTextureDeferredPS.cso");

	LoadShader(eShaderType::VERTEX, "NormalSkinVS.cso");
	LoadShader(eShaderType::PIXEL, "NormalTextureDeferredPS.cso");

	// SSAO Shader
	LoadShader(eShaderType::VERTEX, "SSAOVS.cso");
	LoadShader(eShaderType::PIXEL, "SSAOPS.cso");

	LoadShader(eShaderType::VERTEX, "SSAOBlurVS.cso");
	LoadShader(eShaderType::PIXEL, "SSAOHorizonBlurPS.cso");
	LoadShader(eShaderType::PIXEL, "SSAOVerticalBlurPS.cso");

	// Terrain Shader
	LoadShader(eShaderType::VERTEX, "TerrainVS.cso");
	LoadShader(eShaderType::PIXEL, "TerrainPS.cso");

	// Screen Blur Shader
	LoadShader(eShaderType::COMPUTE, "HorizonBlurCS.cso");
	LoadShader(eShaderType::COMPUTE, "VerticalBlurCS.cso");

	// Shader Sampler 설정..
	SetSampler();
}

void ShaderManager::LoadShader(eShaderType shaderType, std::string shaderName)
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
		case eShaderType::PIXEL:
		{
			PixelShader* pShader = reinterpret_cast<PixelShader*>(shader.second);
			for (std::pair<Hash_Code, ComPtr<ID3D11SamplerState>> sampler : m_SamplerList)
			{
				pShader->SetSamplerState(sampler.first, sampler.second);
			}
		}
		break;
		case eShaderType::COMPUTE:
		{
			ComputeShader* cShader = reinterpret_cast<ComputeShader*>(shader.second);
			for (std::pair<Hash_Code, ComPtr<ID3D11SamplerState>> sampler : m_SamplerList)
			{
				cShader->SetSamplerState(sampler.first, sampler.second);
			}
		}
		break;
		default:
			break;
		}
	}

	// Sampler 설정 후 SamplerList 초기화..
	for (std::pair<Hash_Code, ComPtr<ID3D11SamplerState>> sampler : m_SamplerList)
	{
		RESET_COM(sampler.second);
	}

	m_SamplerList.clear();
}
