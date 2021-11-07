#pragma once

/// <summary>
/// ComputeShader Class
/// </summary>
/// 
/// - 한개의 ComputeShader를 관리하는 클래스

class ComputeShader : public IShader
{
public:
	ComputeShader(const char* fileName);
	ComputeShader() = default;
	~ComputeShader();

public:
	void LoadShader(std::string fileName) override;
	void Update() override;

	// ComputeShader SamplerState 설정..
	void SetSamplerState(Hash_Code hash_code, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler);

	// ComputeShader ConstantBuffer Resource Update..
	template<typename T>
	void SetConstantBuffer(T cBuffer);

	// ComputeShader ShaderResourceView 설정..
	template<typename T>
	void SetShaderResourceView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);

	// ComputeShader UnorderedAccessView 설정..
	template<typename T>
	void SetUnorderedAccessView(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav);

private:
	// Compute Shader
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_CS;

	// 연속된 메모리 공간에 있는 ConstantBuffer List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_ConstantBuffers;
	
	// 연속된 메모리 공간에 있는 SamplerState List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_SamplerStates;

	// 연속된 메모리 공간에 있는 ShaderResourceView List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_ShaderResourceViews;

	// 연속된 메모리 공간에 있는 UnorderedAccessView List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>> m_UnorderedAccessViews;

	// ComputeShader SamplerState List..
	std::unordered_map<Hash_Code, SamplerState> m_SamplerList;

	// ComputeShader ConstantBuffer List..
	std::unordered_map<Hash_Code, ConstantBuffer> m_ConstantBufferList;

	// ComputeShader ShaderResourceView List..
	std::unordered_map<Hash_Code, ShaderResourceBuffer> m_SRVList;

	// ComputeShader UnorderedAccessView List..
	std::unordered_map<Hash_Code, UnorderedAccessBuffer> m_UAVList;
};

inline void ComputeShader::SetSamplerState(Hash_Code hash_code, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler)
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
inline void ComputeShader::SetConstantBuffer(T cBuffer)
{
	// 해당 Value 찾기..
	std::unordered_map<Hash_Code, ConstantBuffer>::iterator it = m_ConstantBufferList.find(typeid(T).hash_code());

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_ConstantBufferList.end())
		return throw std::exception("ERROR: Can not find ConstantBuffer.\n");

	// Resource 복제..
	g_DeviceContext->UpdateSubresource(it->second.cbuffer.Get(), 0, nullptr, &cBuffer, 0, 0);
}

template<typename T>
void ComputeShader::SetShaderResourceView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
{
	// 해당 Value 찾기..
	std::unordered_map<Hash_Code, ShaderResourceBuffer>::iterator it = m_SRVList.find(typeid(T).hash_code());

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_SRVList.end())
		return throw std::exception("ERROR: Can not find ShaderResourceView.\n");

	// ShaderResourceView 설정..
	it->second.srv = srv;

	// 해당 Register Slot에 삽입..
	m_ShaderResourceViews[it->second.register_number] = srv;
}

template<typename T>
void ComputeShader::SetUnorderedAccessView(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav)
{
	// 해당 Value 찾기..
	std::unordered_map<Hash_Code, UnorderedAccessBuffer>::iterator it = m_UAVList.find(typeid(T).hash_code());

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_UAVList.end())
		return throw std::exception("ERROR: Can not find UnorderedAccessView.\n");

	// UnorderedAccessView 설정..
	it->second.uav = uav;

	// 해당 Register Slot에 삽입..
	m_UnorderedAccessViews[it->second.register_number] = uav;
}