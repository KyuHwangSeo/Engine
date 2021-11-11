#pragma once

/// <summary>
/// VetexShader Class
/// </summary>
/// 
/// - 한개의 VertexShader를 관리하는 클래스

class VertexShader : public IShader
{
public:
	VertexShader(const char* fileName);
	~VertexShader();

public:
	void LoadShader(std::string fileName) override;
	void Update() override;
	void Release() override;

	// VertexShader ConstantBuffer Resource Update..
	template<typename T>
	void SetConstantBuffer(T cBuffer);

private:
	// Vertex Shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;

	// VertexShader Input Layout
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;

	// 연속된 메모리 공간에 있는 ConstantBuffer List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_ConstantBuffers;

	// VertexShader ConstantBuffer List..
	std::unordered_map<Hash_Code, ConstantBuffer*> m_ConstantBufferList;
};

template<typename T>
inline void VertexShader::SetConstantBuffer(T cBuffer)
{
	// 해당 Value 찾기..
	std::unordered_map<Hash_Code, ConstantBuffer*>::iterator it = m_ConstantBufferList.find(typeid(T).hash_code());

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_ConstantBufferList.end()) return;

	// Resource 복제..
	g_DeviceContext->UpdateSubresource(it->second->cBuffer.Get(), 0, nullptr, &cBuffer, 0, 0);
}

