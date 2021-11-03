#pragma once
#include "ShaderBase.h"
#include "ShaderResourceBase.h"

/// <summary>
/// PixelShader Class
/// </summary>
/// 
/// - 한개의 PixelShader를 관리하는 클래스

class PixelShader : public IShader
{
public:
	PixelShader(const char* fileName);
	PixelShader() = default;
	~PixelShader();

public:
	void LoadShader(std::string fileName) override;
	void Update() override;

	// PixelShader ConstantBuffer Resource Update..
	template<typename T>
	void UpdateConstantBuffer(T cBuffer);

	// PixelShader SamplerState 설정..
	template<typename T>
	void SetSamplerState(Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler);
	void SetSamplerState(Hash_Code hash_code,Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler);
	
	// PixelShader ShaderResourceView 설정..
	template<typename T>
	void SetShaderResourceView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);

private:
	// Pixel Shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;

	// 연속된 메모리 공간에 있는 ConstantBuffer List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_ConstantBuffers;

	// 연속된 메모리 공간에 있는 SamplerState List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_SamplerStates;
	
	// 연속된 메모리 공간에 있는 ShaderResourceView List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_ShaderResourceViews;

	// PixelShader ConstantBuffer List..
	std::unordered_map<Hash_Code, ConstantBuffer> m_ConstantBufferList;

	// PixelShader SamplerState List..
	std::unordered_map<Hash_Code, SamplerState> m_SamplerList;

	// PixelShader ShaderResourceView List..
	std::unordered_map<Hash_Code, ShaderResourceView> m_SRVList;
};

template<typename T>
inline void PixelShader::UpdateConstantBuffer(T cBuffer)
{
	// 해당 Value 찾기..
	std::unordered_map<Hash_Code, ConstantBuffer>::iterator it = m_ConstantBufferList.find(typeid(T).hash_code());

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_ConstantBufferList.end())
		return throw std::exception("ERROR: Can not find ConstantBuffer.\n");

	// Resource 복제..
	m_DeviceContext->UpdateSubresource(it->second.cbuffer.Get(), 0, nullptr, &cBuffer, 0, 0);
}

template<typename T>
inline void PixelShader::SetSamplerState(Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler)
{
	// 해당 Value 찾기..
	std::unordered_map<Hash_Code, SamplerState>::iterator it = m_SamplerList.find(typeid(T).hash_code());

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_SamplerList.end())
		return throw std::exception("ERROR: Can not find SamplerState.\n");

	// SamplerState 설정..
	it->second.sampler = sampler;

	// 해당 Register Slot에 삽입..
	m_SamplerStates[it->second.register_number] = sampler;
}

inline void PixelShader::SetSamplerState(Hash_Code hash_code, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler)
{
	// 해당 Value 찾기..
	std::unordered_map<Hash_Code, SamplerState>::iterator it = m_SamplerList.find(hash_code);

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_SamplerList.end())
		return throw std::exception("ERROR: Can not find SamplerState.\n");

	// SamplerState 설정..
	it->second.sampler = sampler;

	// 해당 Register Slot에 삽입..
	m_SamplerStates[it->second.register_number] = sampler;
}

template<typename T>
void PixelShader::SetShaderResourceView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
{
	// 해당 Value 찾기..
	std::unordered_map<Hash_Code, ShaderResourceView>::iterator it = m_SRVList.find(typeid(T).hash_code());

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_SRVList.end())
		return throw std::exception("ERROR: Can not find ShaderResourceView.\n");

	// ShaderResourceView 설정..
	it->second.srv = srv;

	// 해당 Register Slot에 삽입..
	m_ShaderResourceViews[it->second.register_number] = srv;
}