#include "DirectDefine.h"
#include "ShaderManagerBase.h"
#include "ShaderBase.h"
#include "ShaderResourceBase.h"
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

	// �ش� Shader�� ã���� ���..
	if (shader != m_ShaderList.end()) 
		return shader->second;

	return nullptr;
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

	// Shader Sampler ����..
	SetSampler();
}

void ShaderManager::LoadShader(ShaderType shaderType, std::string shaderName)
{
	// Shader Type�� �´� Shader ����..
	IShader* newShader = IShader::CreateShader(shaderType, shaderName.c_str());

	// ������ ����� ���� ���Ͽ� �������� ���Ѱ�� nullptr..
	if (newShader == nullptr)
		return throw std::exception("ERROR: Can not Create Shader.\n");

	std::string shaderKey(shaderName);
	size_t pointPosition = shaderName.rfind(".");

	// Shader File Name ���� Key ����..
	if (pointPosition != std::string::npos)
		shaderKey = shaderName.substr(0, pointPosition - 1);

	// ���� ������ Shader ����..
	m_ShaderList.insert(std::make_pair(shaderKey, newShader));
}

void ShaderManager::SetSampler()
{
	// Pixel & Compute Shader Sampler ����..
	for (std::pair<std::string, IShader*> shader : m_ShaderList)
	{
		// Shader Type�� ���� ���� ������ �ǹǷ� ���� ����ȯ ���� �� Sampler ����..
		switch (shader.second->GetType())
		{
		case ShaderType::PIXEL:
		{
			PixelShader* pShader = reinterpret_cast<PixelShader*>(shader.second);
			for (std::pair<Hash_Code, ComPtr<ID3D11SamplerState>> sampler : m_SamplerList)
			{
				pShader->SetSamplerState(sampler.first, sampler.second);
			}
		}
		break;
		case ShaderType::COMPUTE:
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

	// Sampler ���� �� ����Ʈ �ʱ�ȭ..
	for (std::pair<Hash_Code, ComPtr<ID3D11SamplerState>> sampler : m_SamplerList)
	{
		RESET_COM(sampler.second);
	}

	m_SamplerList.clear();
}
