#include "ResourceBufferHashTable.h"

std::unordered_map<std::string, Hash_Code> ShaderResourceHashTable::g_CBuffer_HashTable;
std::unordered_map<std::string, Hash_Code> ShaderResourceHashTable::g_Sampler_HashTable;
std::unordered_map<std::string, Hash_Code> ShaderResourceHashTable::g_SRV_HashTable;
std::unordered_map<std::string, Hash_Code> ShaderResourceHashTable::g_UAV_HashTable;

void ShaderResourceHashTable::Initialize()
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