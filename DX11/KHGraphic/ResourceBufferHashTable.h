#pragma once
#include <unordered_map>

typedef size_t Hash_Code;

/// 
/// ConstantBufferHashTable Class
/// 
/// - ConstantBuffer Struct ���� Hash Code Ű������ �����ϱ� ���� Class
/// - Shader Reflect Load �� Description�� ���ϱ� ���� �ʿ��ϹǷ� Shader Load ���� �ʱ�ȭ �ʼ�
/// - �������� �ش� Resource Class ����� ���ÿ� Hash Table�� �ø�������� ������ ����� �������� �ʴ´�..
///   �ϴ� Initalize�κп� �������� �ش� Resource Class�� �÷�����..

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

	// �ش� Resource Hash Table ���� �Լ�..
	static void Initialize();

	// Hash Table Reset �Լ�..
	static void Reset();

	static size_t FindHashCode(BufferType type, std::string cBufName);
};
