#pragma once
#include "AlignedAllocation.h"
#include "DLLDefine.h"
#include <typeindex>
#include <unordered_map>
#include <algorithm>
#include <string>

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
	string m_Name;

public:
	template <typename T>
	void SetName()					{ m_Name = typeid(T).name(); }
	void SetName(string name)		{ m_Name = name; }

	string GetName()				{ return m_Name; }
	bool CompareName(string name)	{ return m_Name == name; }
};

