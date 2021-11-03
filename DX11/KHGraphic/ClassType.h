#pragma once
#include <string>

#define CREATE_CLASS(ClassName) struct ClassName : public ClassType<ClassName>
#define CREATE_EMPTY_CLASS(ClassName) struct ClassName : public ClassType<ClassName> {};

/// <summary>
/// BufferName Struct
/// </summary>
/// 
/// - ��� Shader Resource�� ����� �Ǵ� Base Class
/// - Shader Reflection�� ���� Resource�� �̸��� ���ؾ� �ϱ� ������ Struct �̸��� �������� �Լ� ����
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