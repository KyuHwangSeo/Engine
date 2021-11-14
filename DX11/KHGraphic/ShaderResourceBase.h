#pragma once
typedef unsigned int register_slot;

// 모든 Shader Resource들의 Base Class..
// 해당 Resource Buffer Name & Binding Register Index 보유..
class ShaderResourceBase
{
public:
	ShaderResourceBase(std::string name, register_slot rNum) : buffer_name(name), register_number(rNum) {}

public:
	// Buffer Name
	std::string buffer_name;

	// Register Slot Number
	register_slot register_number;
};

// ConstantBuffer Resource Data Class
class ConstantBuffer : public ShaderResourceBase
{
public:
	ConstantBuffer(std::string name, unsigned int rNum, ID3D11Buffer** cbuf) : ShaderResourceBase(name, rNum), cBuffer(*cbuf) {}

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cBuffer;
};

// SamplerState Resource Data Class
class SamplerState : public ShaderResourceBase
{
public:
	SamplerState(std::string name, unsigned int rNum) : ShaderResourceBase(name, rNum) {}
};

// ShaderResourceView Resource Data Class
class ShaderResourceBuffer : public ShaderResourceBase
{
public:
	ShaderResourceBuffer(std::string name, unsigned int rNum) : ShaderResourceBase(name, rNum) {}
};

// UnorderedAccessView Resource Data Class
class UnorderedAccessBuffer : public ShaderResourceBase
{
public:
	UnorderedAccessBuffer(std::string name, unsigned int rNum) : ShaderResourceBase(name, rNum) {}
};
