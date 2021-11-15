#include "DirectDefine.h"
#include "ShaderBase.h"

Microsoft::WRL::ComPtr<ID3D11Device> IShader::g_Device = nullptr;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> IShader::g_DeviceContext = nullptr;
std::string IShader::g_ShaderRoute;

void IShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	g_Device = device;
	g_DeviceContext = context;
}

void IShader::Reset()
{
	g_Device.Reset();
	g_DeviceContext.Reset();
}

void IShader::SetShaderRoute(std::string fileRoute)
{
	g_ShaderRoute = fileRoute;
}

void ShaderBase::Release()
{
	for (Microsoft::WRL::ComPtr<ID3D11Buffer> cBuffer : m_ConstantBuffers)
	{
		RESET_COM(cBuffer);
	}

	for (Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler : m_SamplerStates)
	{
		RESET_COM(sampler);
	}

	for (Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv : m_ShaderResourceViews)
	{
		RESET_COM(srv);
	}

	for (std::pair<Hash_Code, ConstantBuffer*> cBuffer : m_ConstantBufferList)
	{
		SAFE_DELETE(cBuffer.second);
	}

	for (std::pair<Hash_Code, SamplerBuffer*> sampler : m_SamplerList)
	{
		SAFE_DELETE(sampler.second);
	}

	for (std::pair<Hash_Code, ShaderResourceBuffer*> srv : m_SRVList)
	{
		SAFE_DELETE(srv.second);
	}

	m_ConstantBuffers.clear();
	m_SamplerStates.clear();
	m_ShaderResourceViews.clear();
	m_SamplerList.clear();
	m_ConstantBufferList.clear();
	m_SRVList.clear();
}

eShaderType ShaderBase::GetType()
{
	return m_ShaderType;
}
