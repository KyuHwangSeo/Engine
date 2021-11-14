#include "ResourceBufferHashTable.h"
#include "ConstantBufferDefine.h"
#include "SamplerStateDefine.h"
#include "ShaderResourceViewDefine.h"
#include "UnorderedAccessViewDefine.h"

#define ADD_CONSTANT_BUFFER(ClassName) g_CBuffer_HashTable.insert(std::make_pair(ClassName::GetName(), ClassName::GetHashCode()));
#define ADD_SAMPLER_STATE(ClassName) g_Sampler_HashTable.insert(std::make_pair(ClassName::GetName(), ClassName::GetHashCode()));
#define ADD_SHADER_RESOURCE_VIEW(ClassName) g_SRV_HashTable.insert(std::make_pair(ClassName::GetName(), ClassName::GetHashCode()));
#define ADD_UNORDERED_ACCESS_VIEW(ClassName) g_UAV_HashTable.insert(std::make_pair(ClassName::GetName(), ClassName::GetHashCode()));

// ShaderResource Hash Table..
std::unordered_map<std::string, Hash_Code> ShaderResourceHashTable::g_CBuffer_HashTable;
std::unordered_map<std::string, Hash_Code> ShaderResourceHashTable::g_Sampler_HashTable;
std::unordered_map<std::string, Hash_Code> ShaderResourceHashTable::g_SRV_HashTable;
std::unordered_map<std::string, Hash_Code> ShaderResourceHashTable::g_UAV_HashTable;

void ShaderResourceHashTable::Initialize()
{
	// Constant Buffer Hash Table Create..
	ADD_CONSTANT_BUFFER(cbPerObject)
	ADD_CONSTANT_BUFFER(cbLights)
	ADD_CONSTANT_BUFFER(cbCamera)
	ADD_CONSTANT_BUFFER(cbShadow)
	ADD_CONSTANT_BUFFER(cbSkinned)
	ADD_CONSTANT_BUFFER(cbID)
	ADD_CONSTANT_BUFFER(cbLightList)
	ADD_CONSTANT_BUFFER(cbMaterialList)
	ADD_CONSTANT_BUFFER(cbTexViewProj)
	ADD_CONSTANT_BUFFER(cbTexel)
	ADD_CONSTANT_BUFFER(cbSsaoFrame)
	ADD_CONSTANT_BUFFER(cbFullScreen)
	ADD_CONSTANT_BUFFER(cbPerUI)
	ADD_CONSTANT_BUFFER(cbAlpha)

	// Sampler State Hash Table Create..
	//ADD_SAMPLER_STATE(gShadowSam)
	ADD_SAMPLER_STATE(samWrapMinLinear)
	ADD_SAMPLER_STATE(samWrapAnisotropic)
	ADD_SAMPLER_STATE(samClampMinLinear)

	// Shader Resource View Hash Table Create..
	ADD_SHADER_RESOURCE_VIEW(gDiffuseMap)
	ADD_SHADER_RESOURCE_VIEW(gNormalMap)
	ADD_SHADER_RESOURCE_VIEW(gCubeMap)
	ADD_SHADER_RESOURCE_VIEW(gShadowMap)
	ADD_SHADER_RESOURCE_VIEW(gSSAOMap)
	ADD_SHADER_RESOURCE_VIEW(gNormalDepthMap)
	ADD_SHADER_RESOURCE_VIEW(gRandomVecMap)
	ADD_SHADER_RESOURCE_VIEW(gInputMap)
	ADD_SHADER_RESOURCE_VIEW(AlbedoSRV)
	ADD_SHADER_RESOURCE_VIEW(NormalSRV)
	ADD_SHADER_RESOURCE_VIEW(PositionSRV)
	ADD_SHADER_RESOURCE_VIEW(ShadowSRV)
	ADD_SHADER_RESOURCE_VIEW(SsaoSRV)
	ADD_SHADER_RESOURCE_VIEW(gInput)
	ADD_SHADER_RESOURCE_VIEW(gDepthMap)
	ADD_SHADER_RESOURCE_VIEW(gOrigin)
	ADD_SHADER_RESOURCE_VIEW(gVelocity)

	// Unordered Access View Hash Table Create..
	ADD_UNORDERED_ACCESS_VIEW(gOutput)
}

void ShaderResourceHashTable::Reset()
{
	g_CBuffer_HashTable.clear();
	g_Sampler_HashTable.clear();
	g_SRV_HashTable.clear();
	g_UAV_HashTable.clear();
}

size_t ShaderResourceHashTable::FindHashCode(BufferType type, std::string cBufName)
{
	std::unordered_map<std::string, Hash_Code>::iterator cHash;

	switch (type)
	{
	case BufferType::CBUFFER:
	{
		cHash = g_CBuffer_HashTable.find(cBufName);

		if (cHash == g_CBuffer_HashTable.end())
		{
			throw std::exception("ERROR: Can not find Constant Buffer Hash Code.\n");
			return 0;
		}
	}
	break;
	case BufferType::SAMPLER:
	{
		cHash = g_Sampler_HashTable.find(cBufName);

		if (cHash == g_Sampler_HashTable.end())
		{
			throw std::exception("ERROR: Can not find Constant Buffer Hash Code.\n");
			return 0;
		}
	}
	break;
	case BufferType::SRV:
	{
		cHash = g_SRV_HashTable.find(cBufName);

		if (cHash == g_SRV_HashTable.end())
		{
			throw std::exception("ERROR: Can not find Shader Resource View Hash Code.\n");
			return 0;
		}
	}
	break;
	case BufferType::UAV:
	{
		cHash = g_UAV_HashTable.find(cBufName);

		if (cHash == g_UAV_HashTable.end())
		{
			throw std::exception("ERROR: Can not find Unordered Access View Hash Code.\n");
			return 0;
		}
	}
	break;
	default:
	{
		throw std::exception("ERROR: Can not find Resource Type.\n");
		return 0;
	}
	break;
	}


	return cHash->second;
}