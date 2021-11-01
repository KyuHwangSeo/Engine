#pragma once
#include "ShaderBase.h"

/// <summary>
/// VetexShader Class
/// </summary>
/// 
/// - �Ѱ��� VertexShader�� �����ϴ� Ŭ����

class VertexShader : public ShaderBase
{
public:
	VertexShader(const char* fileName);
	VertexShader() = default;
	~VertexShader();

public:
	void LoadShader(std::string fileName);
	void Update();

	// VertexShader ConstantBuffer Resource Update..
	template<typename T>
	void UpdateConstantBuffer(T cBuffer);

private:
	// Vertex Shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;

	// VertexShader Input Layout
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;

	// ���ӵ� �޸� ������ �ִ� ConstantBuffer List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_ConstantBuffers;

	// VertexShader ConstantBuffer List..
	std::unordered_map<Hash_Code, ConstantBuffer> m_ConstantBufferList;
};

template<typename T>
inline void VertexShader::UpdateConstantBuffer(T cBuffer)
{
	// �ش� Value ã��..
	std::unordered_map<Hash_Code, ConstantBuffer>::iterator it = m_ConstantBufferList.find(typeid(T).hash_code());

	// �ش� Key�� ���� Value�� ���ٸ�..
	if (it == m_ConstantBufferList.end())
		return throw std::exception("ERROR: Can not find ConstantBuffer.\n");

	// Resource ����..
	m_DeviceContext->UpdateSubresource(it->second.cbuffer.Get(), 0, nullptr, &cBuffer, 0, 0);
}
