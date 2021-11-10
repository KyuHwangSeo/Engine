#pragma once

/// <summary>
/// PixelShader Class
/// </summary>
/// 
/// - 한개의 PixelShader를 관리하는 클래스

class PixelShader : public IShader
{
public:
	PixelShader(const char* fileName);
	~PixelShader();

public:
	void LoadShader(std::string fileName) override;
	void Update() override;

	// PixelShader SamplerState 설정..
	void SetSamplerState(Hash_Code hash_code, ID3D11SamplerState** sampler);

	// PixelShader ConstantBuffer Resource Update..
	template<typename T>
	void SetConstantBuffer(T cBuffer);

	// PixelShader ShaderResourceView 설정..
	template<typename T>
	void SetShaderResourceView(ID3D11ShaderResourceView** srv);

private:
	// Pixel Shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;

	// 연속된 메모리 공간에 있는 ConstantBuffer List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_ConstantBuffers;

	// 연속된 메모리 공간에 있는 SamplerState List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_SamplerStates;
	
	// 연속된 메모리 공간에 있는 ShaderResourceView List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_ShaderResourceViews;

	// PixelShader SamplerState List..
	std::unordered_map<Hash_Code, SamplerState> m_SamplerList;

	// PixelShader ConstantBuffer List..
	std::unordered_map<Hash_Code, ConstantBuffer> m_ConstantBufferList;

	// PixelShader ShaderResourceView List..
	std::unordered_map<Hash_Code, ShaderResourceBuffer> m_SRVList;
};

inline void PixelShader::SetSamplerState(Hash_Code hash_code, ID3D11SamplerState** sampler)
{
	// 해당 Value 찾기..
	std::unordered_map<Hash_Code, SamplerState>::iterator it = m_SamplerList.find(hash_code);

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_SamplerList.end()) return;

	// SamplerState 설정..
	it->second.sampler = *sampler;

	// 해당 Register Slot에 삽입..
	m_SamplerStates[it->second.register_number] = *sampler;
}

template<typename T>
inline void PixelShader::SetConstantBuffer(T cBuffer)
{
	// 해당 Value 찾기..
	std::unordered_map<Hash_Code, ConstantBuffer>::iterator it = m_ConstantBufferList.find(typeid(T).hash_code());

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_ConstantBufferList.end()) return;

	// Resource 복제..
	g_DeviceContext->UpdateSubresource(it->second.cbuffer.Get(), 0, nullptr, &cBuffer, 0, 0);
}

template<typename T>
void PixelShader::SetShaderResourceView(ID3D11ShaderResourceView** srv)
{
	// 해당 Value 찾기..
	std::unordered_map<Hash_Code, ShaderResourceBuffer>::iterator it = m_SRVList.find(typeid(T).hash_code());

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_SRVList.end()) return;

	// ShaderResourceView 설정..
	it->second.srv = *srv;

	// 해당 Register Slot에 삽입..
	m_ShaderResourceViews[it->second.register_number] = *srv;
}