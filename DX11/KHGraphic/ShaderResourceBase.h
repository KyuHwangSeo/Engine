#pragma once
#define RESET_COM(x) { if(x != nullptr) {x.Reset();} }

typedef unsigned int register_slot;

// 모든 Shader Resource들의 Base Class..
// 해당 Resource Buffer Name & Binding Register Index 보유..
struct ShaderResourceBase
{
	ShaderResourceBase(std::string name, register_slot rNum) : buffer_name(name), register_number(rNum) {}
	virtual ~ShaderResourceBase() = default;

	// Buffer Name
	std::string buffer_name;

	// Register Slot Number
	register_slot register_number;
};

// ConstantBuffer Resource Data Class
struct ConstantBuffer : public ShaderResourceBase
{
	ConstantBuffer(std::string name, unsigned int rNum, Microsoft::WRL::ComPtr<ID3D11Buffer> cBuf = nullptr) : ShaderResourceBase(name, rNum), cbuffer(cBuf) {}
	~ConstantBuffer() { RESET_COM(cbuffer); }

	// Constant Buffer Resource
	Microsoft::WRL::ComPtr<ID3D11Buffer> cbuffer;
};

// SamplerState Resource Data Class
struct SamplerState : public ShaderResourceBase
{
	SamplerState(std::string name, unsigned int rNum) : ShaderResourceBase(name, rNum), sampler(nullptr) {}
	~SamplerState() { RESET_COM(sampler); }

	// Sampler State Resource
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
};

// ShaderResourceView Resource Data Class
struct ShaderResourceBuffer : public ShaderResourceBase
{
	ShaderResourceBuffer(std::string name, unsigned int rNum) : ShaderResourceBase(name, rNum) {}
	~ShaderResourceBuffer() { RESET_COM(srv); }

	// Shader Resource View Resource
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
};

// UnorderedAccessView Resource Data Class
struct UnorderedAccessBuffer : public ShaderResourceBase
{
	UnorderedAccessBuffer(std::string name, unsigned int rNum) : ShaderResourceBase(name, rNum) {}
	~UnorderedAccessBuffer() { RESET_COM(uav); }

	// Unordered Access View Resource
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav;
};
