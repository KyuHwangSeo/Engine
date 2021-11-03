#pragma once
#include <string>

#define CREATE_CLASS(ClassName) struct ClassName : public ClassType<ClassName>
#define CREATE_EMPTY_CLASS(ClassName) struct ClassName : public ClassType<ClassName> {};

/// <summary>
/// BufferName Struct
/// </summary>
/// 
/// - 모든 Shader Resource의 기반이 되는 Base Class
/// - Shader Reflection을 통해 Resource의 이름을 비교해야 하기 때문에 Struct 이름만 가져오는 함수 보유
///

template <typename T>
struct ClassType
{
	static std::string GetName()
	{
		std::string name = typeid(T).name();
		size_t spacePosition = name.find_first_of(" ");
		if (spacePosition != std::string::npos)
			return name.substr(spacePosition + 1, name.length());
		else
			return name;
	}

	static size_t GetHashCode()
	{
		return typeid(T).hash_code();
	}
};