#pragma once
#include "ShaderBase.h"
#include "ShaderResourceBase.h"

/// <summary>
/// PixelShader Class
/// </summary>
/// 
/// - �Ѱ��� PixelShader�� �����ϴ� Ŭ����

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

	// PixelShader SamplerState ����..
	template<typename T>
	void SetSamplerState(Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler);
	void SetSamplerState(Hash_Code hash_code,Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler);
	
	// PixelShader ShaderResourceView ����..
	template<typename T>
	void SetShaderResourceView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);

private:
	// Pixel Shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;

	// ���ӵ� �޸� ������ �ִ� ConstantBuffer List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_ConstantBuffers;

	// ���ӵ� �޸� ������ �ִ� SamplerState List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_SamplerStates;
	
	// ���ӵ� �޸� ������ �ִ� ShaderResourceView List..
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
	// �ش� Value ã��..
	std::unordered_map<Hash_Code, ConstantBuffer>::iterator it = m_ConstantBufferList.find(typeid(T).hash_code());

	// �ش� Key�� ���� Value�� ���ٸ�..
	if (it == m_ConstantBufferList.end())
		return throw std::exception("ERROR: Can not find ConstantBuffer.\n");

	// Resource ����..
	m_DeviceContext->UpdateSubresource(it->second.cbuffer.Get(), 0, nullptr, &cBuffer, 0, 0);
}

template<typename T>
inline void PixelShader::SetSamplerState(Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler)
{
	// �ش� Value ã��..
	std::unordered_map<Hash_Code, SamplerState>::iterator it = m_SamplerList.find(typeid(T).hash_code());

	// �ش� Key�� ���� Value�� ���ٸ�..
	if (it == m_SamplerList.end())
		return throw std::exception("ERROR: Can not find SamplerState.\n");

	// SamplerState ����..
	it->second.sampler = sampler;

	// �ش� Register Slot�� ����..
	m_SamplerStates[it->second.register_number] = sampler;
}

inline void PixelShader::SetSamplerState(Hash_Code hash_code, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler)
{
	// �ش� Value ã��..
	std::unordered_map<Hash_Code, SamplerState>::iterator it = m_SamplerList.find(hash_code);

	// �ش� Key�� ���� Value�� ���ٸ�..
	if (it == m_SamplerList.end())
		return throw std::exception("ERROR: Can not find SamplerState.\n");

	// SamplerState ����..
	it->second.sampler = sampler;

	// �ش� Register Slot�� ����..
	m_SamplerStates[it->second.register_number] = sampler;
}

template<typename T>
void PixelShader::SetShaderResourceView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
{
	// �ش� Value ã��..
	std::unordered_map<Hash_Code, ShaderResourceView>::iterator it = m_SRVList.find(typeid(T).hash_code());

	// �ش� Key�� ���� Value�� ���ٸ�..
	if (it == m_SRVList.end())
		return throw std::exception("ERROR: Can not find ShaderResourceView.\n");

	// ShaderResourceView ����..
	it->second.srv = srv;

	// �ش� Register Slot�� ����..
	m_ShaderResourceViews[it->second.register_number] = srv;
}