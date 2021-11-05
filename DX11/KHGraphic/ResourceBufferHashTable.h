#pragma once
#include <unordered_map>
#include "ConstantBufferDefine.h"
#include "SamplerStateDefine.h"
#include "ShaderResourceViewDefine.h"
#include "UnorderedAccessViewDefine.h"

typedef size_t Hash_Code;

/// 
/// ConstantBufferHashTable Class
/// 
/// - ConstantBuffer Struct 기준 Hash Code 키값으로 설정하기 위한 Class
/// - Shader Reflect Load 시 Description과 비교하기 위해 필요하므로 Shader Load 전에 초기화 필수
///

class ShaderResourceHashTable
{
public:
	enum class BufferType
	{
		CBUFFER,
		SAMPLER,
		SRV,
		UAV
	};

public:
	// ConstantBuffer Hash Table..
	static std::unordered_map<std::string, Hash_Code> g_CBuffer_HashTable;
	// Sampler Hash Table..
	static std::unordered_map<std::string, Hash_Code> g_Sampler_HashTable;
	// ShaderResourceView Hash Table..
	static std::unordered_map<std::string, Hash_Code> g_SRV_HashTable;
	// UnorderedAccessView Hash Table..
	static std::unordered_map<std::string, Hash_Code> g_UAV_HashTable;

	// 해당 Resource Hash Table 생성 함수..
	static void Initialize();

	// Hash Table Reset 함수..
	static void Reset();

	static size_t FindHashCode(BufferType type, std::string cBufName);
};
