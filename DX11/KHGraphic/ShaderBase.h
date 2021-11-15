#pragma once
#include <unordered_map>
#include "ShaderResourceBase.h"

/// Shader Reflection MSDN
/// https://docs.microsoft.com/ko-kr/windows/win32/api/d3d11shader/nn-d3d11shader-id3d11shaderreflection?f1url=%3FappId%3DDev16IDEF1%26l%3DKO-KR%26k%3Dk(D3D11SHADER%252FID3D11ShaderReflection);k(ID3D11ShaderReflection);k(DevLang-C%252B%252B);k(TargetOS-Windows)%26rd%3Dtrue
/// Shader Desc MSDN
/// https://docs.microsoft.com/ko-kr/windows/win32/api/d3d11shader/ns-d3d11shader-d3d11_shader_desc?f1url=%3FappId%3DDev16IDEF1%26l%3DKO-KR%26k%3Dk(D3D11SHADER%252FD3D11_SHADER_DESC);k(D3D11_SHADER_DESC);k(DevLang-C%252B%252B);k(TargetOS-Windows)%26rd%3Dtrue

// Shader Type Enum Class..
enum class eShaderType
{
	VERTEX,
	PIXEL,
	COMPUTE
};

/// <summary>
/// IShader Class
/// </summary>
/// 
/// - ��� Shader Class�� Base Class
/// - ��� Shader�� Resource ���� �� ������ Device & DeviceContext�� �ʿ�� �Ͽ� ���� ������ ����
/// 

class IShader
{
protected:
	IShader() = default;
	virtual ~IShader() { Reset(); }

public:
	// Shader Load ���� �ʼ��� ���־�� �ϴ� �۾�..
	static void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

	// Device & DeviceContext Reset �Լ�..
	static void Reset();

	// �⺻ Shader ��� ���� �Լ�..
	static void SetShaderRoute(std::string fileRoute);

public:
	// Device & Context..
	static Microsoft::WRL::ComPtr<ID3D11Device> g_Device;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> g_DeviceContext;

	// Shader File Route..
	static std::string g_ShaderRoute;
};

/// <summary>
/// ShaderBase Class
/// </summary>
/// 
/// - ��� Shader Class�� Base Class
/// - Vertex, Pixel, Compute Shader�� �⺻������ ����ϴ� Resource�� ������ Base Class
/// 

class ShaderBase : public IShader
{
public:
	ShaderBase(eShaderType shaderType) : m_ShaderType(shaderType) {}

public:
	virtual void LoadShader(std::string fileName) abstract;
	virtual void Update() abstract;
	virtual void Release();

	// Shader SamplerState ����..
	template<typename T>
	void SetSamplerState(ID3D11SamplerState** sampler);

	// Shader ConstantBuffer Resource Update..
	template<typename T>
	void SetConstantBuffer(T cBuffer);

	// Shader ShaderResourceView ����..
	template<typename T>
	void SetShaderResourceView(ID3D11ShaderResourceView** srv);

public:
	// ���� Shader Type ��ȯ �Լ�..
	eShaderType GetType();

protected:
	// ���ӵ� �޸� ������ �ִ� ConstantBuffer List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_ConstantBuffers;

	// ���ӵ� �޸� ������ �ִ� SamplerState List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_SamplerStates;

	// ���ӵ� �޸� ������ �ִ� ShaderResourceView List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_ShaderResourceViews;

	// PixelShader ConstantBuffer List..
	std::unordered_map<Hash_Code, ConstantBuffer*> m_ConstantBufferList;

	// PixelShader SamplerState List..
	std::unordered_map<Hash_Code, SamplerBuffer*> m_SamplerList;

	// PixelShader ShaderResourceView List..
	std::unordered_map<Hash_Code, ShaderResourceBuffer*> m_SRVList;

private:
	// ���� Shader Type..
	eShaderType m_ShaderType;
};

template<typename T>
inline void ShaderBase::SetSamplerState(ID3D11SamplerState** sampler)
{
	// �ش� Value ã��..
	std::unordered_map<Hash_Code, SamplerBuffer*>::iterator it = m_SamplerList.find(typeid(T).hash_code());

	// �ش� Key�� ���� Value�� ���ٸ�..
	if (it == m_SamplerList.end()) return;

	// �ش� Register Slot�� ����..
	m_SamplerStates[it->second->register_number] = *sampler;
}

template<typename T>
inline void ShaderBase::SetConstantBuffer(T cBuffer)
{
	// �ش� Value ã��..
	std::unordered_map<Hash_Code, ConstantBuffer*>::iterator it = m_ConstantBufferList.find(typeid(T).hash_code());

	// �ش� Key�� ���� Value�� ���ٸ�..
	if (it == m_ConstantBufferList.end()) return;

	// Resource ����..
	g_DeviceContext->UpdateSubresource(it->second->cBuffer.Get(), 0, nullptr, &cBuffer, 0, 0);
}

template<typename T>
inline void ShaderBase::SetShaderResourceView(ID3D11ShaderResourceView** srv)
{
	// �ش� Value ã��..
	std::unordered_map<Hash_Code, ShaderResourceBuffer*>::iterator it = m_SRVList.find(typeid(T).hash_code());

	// �ش� Key�� ���� Value�� ���ٸ�..
	if (it == m_SRVList.end()) return;

	// �ش� Register Slot�� ����..
	m_ShaderResourceViews[it->second->register_number] = *srv;
}
