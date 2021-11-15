#pragma once

/// <summary>
/// ComputeShader Class
/// </summary>
/// 
/// - �Ѱ��� ComputeShader�� �����ϴ� Ŭ����

class ComputeShader : public ShaderBase
{
public:
	ComputeShader(const char* fileName);
	~ComputeShader();

public:
	void LoadShader(std::string fileName) override;
	void Update() override;
	void Release() override;

	// ComputeShader UnorderedAccessView ����..
	template<typename T>
	void SetUnorderedAccessView(ID3D11UnorderedAccessView** uav);

private:
	// Compute Shader
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_CS;

	// ���ӵ� �޸� ������ �ִ� UnorderedAccessView List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>> m_UnorderedAccessViews;

	// ComputeShader UnorderedAccessView List..
	std::unordered_map<Hash_Code, UnorderedAccessBuffer*> m_UAVList;
};

template<typename T>
void ComputeShader::SetUnorderedAccessView(ID3D11UnorderedAccessView** uav)
{
	// �ش� Value ã��..
	std::unordered_map<Hash_Code, UnorderedAccessBuffer*>::iterator it = m_UAVList.find(typeid(T).hash_code());

	// �ش� Key�� ���� Value�� ���ٸ�..
	if (it == m_UAVList.end()) return;

	// �ش� Register Slot�� ����..
	m_UnorderedAccessViews[it->second->register_number] = *uav;
}