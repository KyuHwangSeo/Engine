#pragma once
#include <unordered_map>
#include "ConstantBufferDefine.h"
#include "SamplerStateDefine.h"
#include "ShaderResourceViewDefine.h"
#include "UnorderedAccessViewDefine.h"

typedef size_t Hash_Code;
typedef std::unordered_map<std::string, Hash_Code>::iterator Hash_Pair;

/// 
/// ConstantBufferHashTable Class
/// 
/// - ConstantBuffer Struct 기준 Hash Code 키값으로 설정하기 위한 Class
/// - Shader Reflect Load 시 Description과 비교하기 위해 필요하므로 Shader Load 전에 초기화 필수
///

class ShaderResourceHashTable
{
public:
	enum Type
	{
		CBUFFER,
		SAMPLER,
		SRV,
		UAV
	};

public:
	static std::unordered_map<std::string, Hash_Code> g_CBuffer_HashTable;
	static std::unordered_map<std::string, Hash_Code> g_Sampler_HashTable;
	static std::unordered_map<std::string, Hash_Code> g_SRV_HashTable;
	static std::unordered_map<std::string, Hash_Code> g_UAV_HashTable;

	// 해당 Resource Hash Table 생성 함수..
	static void Initialize();

	// Hash Table Reset 함수..
	static void Reset();

	static size_t FindHashCode(Type type, std::string cBufName);
};

inline void ShaderResourceHashTable::Initialize()
{
	// Constant Buffer Hash Table Create..
	g_CBuffer_HashTable.insert(std::make_pair(cbPerObject::GetName(), cbPerObject::GetHashCode()));
	g_CBuffer_HashTable.insert(std::make_pair(cbLights::GetName(), cbLights::GetHashCode()));
	g_CBuffer_HashTable.insert(std::make_pair(cbCamera::GetName(), cbCamera::GetHashCode()));
	g_CBuffer_HashTable.insert(std::make_pair(cbShadow::GetName(), cbShadow::GetHashCode()));
	g_CBuffer_HashTable.insert(std::make_pair(cbSkinned::GetName(), cbSkinned::GetHashCode()));
	g_CBuffer_HashTable.insert(std::make_pair(cbID::GetName(), cbID::GetHashCode()));
	g_CBuffer_HashTable.insert(std::make_pair(cbLightList::GetName(), cbLightList::GetHashCode()));
	g_CBuffer_HashTable.insert(std::make_pair(cbMaterialList::GetName(), cbMaterialList::GetHashCode()));
	g_CBuffer_HashTable.insert(std::make_pair(cbTexViewProj::GetName(), cbTexViewProj::GetHashCode()));
	g_CBuffer_HashTable.insert(std::make_pair(cbTexel::GetName(), cbTexel::GetHashCode()));
	g_CBuffer_HashTable.insert(std::make_pair(cbSsaoFrame::GetName(), cbSsaoFrame::GetHashCode()));
	g_CBuffer_HashTable.insert(std::make_pair(cbFullScreen::GetName(), cbFullScreen::GetHashCode()));
	g_CBuffer_HashTable.insert(std::make_pair(cbPerUI::GetName(), cbPerUI::GetHashCode()));
	g_CBuffer_HashTable.insert(std::make_pair(cbAlpha::GetName(), cbAlpha::GetHashCode()));

	// Sampler State Hash Table Create..
	g_Sampler_HashTable.insert(std::make_pair(gShadowSam::GetName(), gShadowSam::GetHashCode()));
	g_Sampler_HashTable.insert(std::make_pair(samWrapMinLinear::GetName(), samWrapMinLinear::GetHashCode()));
	g_Sampler_HashTable.insert(std::make_pair(samWrapAnisotropic::GetName(), samWrapAnisotropic::GetHashCode()));
	g_Sampler_HashTable.insert(std::make_pair(samClampMinLinear::GetName(), samClampMinLinear::GetHashCode()));
	
	// Shader Resource View Hash Table Create..
	g_SRV_HashTable.insert(std::make_pair(gDiffuseMap::GetName(), gDiffuseMap::GetHashCode()));
	g_SRV_HashTable.insert(std::make_pair(gNormalMap::GetName(), gNormalMap::GetHashCode()));
	g_SRV_HashTable.insert(std::make_pair(gCubeMap::GetName(), gCubeMap::GetHashCode()));
	g_SRV_HashTable.insert(std::make_pair(gShadowMap::GetName(), gShadowMap::GetHashCode()));
	g_SRV_HashTable.insert(std::make_pair(gSSAOMap::GetName(), gSSAOMap::GetHashCode()));
	g_SRV_HashTable.insert(std::make_pair(gNormalDepthMap::GetName(), gNormalDepthMap::GetHashCode()));
	g_SRV_HashTable.insert(std::make_pair(gRandomVecMap::GetName(), gRandomVecMap::GetHashCode()));
	g_SRV_HashTable.insert(std::make_pair(gInputMap::GetName(), gInputMap::GetHashCode()));
	g_SRV_HashTable.insert(std::make_pair(AlbedoSRV::GetName(), AlbedoSRV::GetHashCode()));
	g_SRV_HashTable.insert(std::make_pair(NormalSRV::GetName(), NormalSRV::GetHashCode()));
	g_SRV_HashTable.insert(std::make_pair(PositionSRV::GetName(), PositionSRV::GetHashCode()));
	g_SRV_HashTable.insert(std::make_pair(ShadowSRV::GetName(), ShadowSRV::GetHashCode()));
	g_SRV_HashTable.insert(std::make_pair(SsaoSRV::GetName(), SsaoSRV::GetHashCode()));
	g_SRV_HashTable.insert(std::make_pair(gInput::GetName(), gInput::GetHashCode()));
	g_SRV_HashTable.insert(std::make_pair(gDepthMap::GetName(), gDepthMap::GetHashCode()));
	g_SRV_HashTable.insert(std::make_pair(gOrigin::GetName(), gOrigin::GetHashCode()));
	g_SRV_HashTable.insert(std::make_pair(gVelocity::GetName(), gVelocity::GetHashCode()));

	// Unordered Access View Hash Table Create..
	g_UAV_HashTable.insert(std::make_pair(gOutput::GetName(), gOutput::GetHashCode()));
}

inline void ShaderResourceHashTable::Reset()
{
	g_CBuffer_HashTable.clear();
	g_Sampler_HashTable.clear();
	g_SRV_HashTable.clear();
	g_UAV_HashTable.clear();
}

inline size_t ShaderResourceHashTable::FindHashCode(Type type, std::string cBufName)
{
	Hash_Pair cHash;

	switch (type)
	{
	case ShaderResourceHashTable::CBUFFER:
	{
		cHash = g_CBuffer_HashTable.find(cBufName);

		if (cHash == g_CBuffer_HashTable.end())
		{
			throw std::exception("ERROR: Can not find Constant Buffer Hash Code.\n");
			return 0;
		}
	}
		break;
	case ShaderResourceHashTable::SAMPLER:
	{
		cHash = g_Sampler_HashTable.find(cBufName);

		if (cHash == g_Sampler_HashTable.end())
		{
			throw std::exception("ERROR: Can not find Constant Buffer Hash Code.\n");
			return 0;
		}
	}
	break;
	case ShaderResourceHashTable::SRV:
	{
		cHash = g_SRV_HashTable.find(cBufName);

		if (cHash == g_SRV_HashTable.end())
		{
			throw std::exception("ERROR: Can not find Shader Resource View Hash Code.\n");
			return 0;
		}
	}
		break;
	case ShaderResourceHashTable::UAV:
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