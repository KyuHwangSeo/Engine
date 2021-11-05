#pragma once

class IResource
{
public:
	enum class ResourceType
	{
		RTV,
		SRV,
		UAV,
		DSV
	};

public:
	IResource(ResourceType type) : m_Type(type) {}

public:
	ResourceType GetType() { return m_Type; }

protected:
	ResourceType m_Type;
};