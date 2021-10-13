#pragma once
#include "AlignedAllocation.h"
#include "DLLDefine.h"
#include <typeindex>
#include <unordered_map>
#include <algorithm>

using namespace std;

/// <summary>
/// Object Class
/// </summary>
/// 
/// ��� Ŭ������ ����̵Ǵ� �ֻ��� Ŭ����

class Object : public AlignedAllocation<16>
{
public:
	Object() = default;
	virtual ~Object() = default;

protected:
	std::string m_Name;

public:
	template <typename T>
	void SetName()					{ m_Name = typeid(T).name(); }
	void SetName(string name)		{ m_Name = name; }

	std::string GetName()				{ return m_Name; }
	bool CompareName(string name)	{ return m_Name == name; }
};

