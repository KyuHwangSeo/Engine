#pragma once

/// <summary>
/// ComputeShader Class
/// </summary>
/// 
/// - �Ѱ��� ComputeShader�� �����ϴ� Ŭ����

class ComputeShader : public IShader
{
public:
	ComputeShader(const char* fileName);
	ComputeShader() = default;
	~ComputeShader();

public:
	void LoadShader(std::string fileName) override;
	void Update() override;

	// ComputeShader SamplerState ����..
	void SetSamplerState(Hash_Code hash_code, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler);

	// ComputeShader ConstantBuffer Resource Update..
	template<typename T>
	void SetConstantBuffer(T cBuffer);

	// ComputeShader ShaderResourceView ����..
	template<typename T>
	void SetShaderResourceView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);

	// ComputeShader UnorderedAccessView ����..
	template<typename T>
	void SetUnorderedAccessView(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav);

private:
	// Compute Shader
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_CS;

	// ���ӵ� �޸� ������ �ִ� ConstantBuffer List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_ConstantBuffers;
	
	// ���ӵ� �޸� ������ �ִ� SamplerState List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_SamplerStates;

	// ���ӵ� �޸� ������ �ִ� ShaderResourceView List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_ShaderResourceViews;

	// ���ӵ� �޸� ������ �ִ� UnorderedAccessView List..
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
inline void ComputeShader::SetConstantBuffer(T cBuffer)
{
	// �ش� Value ã��..
	std::unordered_map<Hash_Code, ConstantBuffer>::iterator it = m_ConstantBufferList.find(typeid(T).hash_code());

	// �ش� Key�� ���� Value�� ���ٸ�..
	if (it == m_ConstantBufferList.end())
		return throw std::exception("ERROR: Can not find ConstantBuffer.\n");

	// Resource ����..
	g_DeviceContext->UpdateSubresource(it->second.cbuffer.Get(), 0, nullptr, &cBuffer, 0, 0);
}

template<typename T>
void ComputeShader::SetShaderResourceView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
{
	// �ش� Value ã��..
	std::unordered_map<Hash_Code, ShaderResourceBuffer>::iterator it = m_SRVList.find(typeid(T).hash_code());

	// �ش� Key�� ���� Value�� ���ٸ�..
	if (it == m_SRVList.end())
		return throw std::exception("ERROR: Can not find ShaderResourceView.\n");

	// ShaderResourceView ����..
	it->second.srv = srv;

	// �ش� Register Slot�� ����..
	m_ShaderResourceViews[it->second.register_number] = srv;
}

template<typename T>
void ComputeShader::SetUnorderedAccessView(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav)
{
	// �ش� Value ã��..
	std::unordered_map<Hash_Code, UnorderedAccessBuffer>::iterator it = m_UAVList.find(typeid(T).hash_code());

	// �ش� Key�� ���� Value�� ���ٸ�..
	if (it == m_UAVList.end())
		return throw std::exception("ERROR: Can not find UnorderedAccessView.\n");

	// UnorderedAccessView ����..
	it->second.uav = uav;

	// �ش� Register Slot�� ����..
	m_UnorderedAccessViews[it->second.register_number] = uav;
}