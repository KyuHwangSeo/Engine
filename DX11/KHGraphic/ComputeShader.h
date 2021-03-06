#pragma once

/// <summary>
/// ComputeShader Class
/// </summary>
/// 
/// - 한개의 ComputeShader를 관리하는 클래스

class ComputeShader : public ShaderBase
{
public:
	ComputeShader(const char* fileName);
	~ComputeShader();

public:
	void LoadShader(std::string fileName) override;
	void Update() override;
	void Release() override;

	// ComputeShader UnorderedAccessView 설정..
	template<typename T>
	void SetUnorderedAccessView(ID3D11UnorderedAccessView** uav);

private:
	// Compute Shader
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_CS;

	// 연속된 메모리 공간에 있는 UnorderedAccessView List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>> m_UnorderedAccessViews;

	// ComputeShader UnorderedAccessView List..
	std::unordered_map<Hash_Code, UnorderedAccessBuffer*> m_UAVList;
};

template<typename T>
void ComputeShader::SetUnorderedAccessView(ID3D11UnorderedAccessView** uav)
{
	// 해당 Value 찾기..
	std::unordered_map<Hash_Code, UnorderedAccessBuffer*>::iterator it = m_UAVList.find(typeid(T).hash_code());

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_UAVList.end()) return;

	// 해당 Register Slot에 삽입..
	m_UnorderedAccessViews[it->second->register_number] = *uav;
}