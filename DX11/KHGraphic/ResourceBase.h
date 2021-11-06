#pragma once

class IResource
{
public:
	enum class ResourceType
	{
		RTV,	// RenderTargetView
		SRV,	// ShaderResourceView
		UAV,	// UnorderedResourceView
		DSV,	// DepthStencilView
		DSS,	// DepthStencilState
		RS,		// RasterizerState
		BS		// BlendState
	};

public:
	IResource(ResourceType type) : m_Type(type) {}

public:
	ResourceType GetType() { return m_Type; }

protected:
	ResourceType m_Type;
};