#include "DirectDefine.h"
#include "ShaderBase.h"
#include "ShaderResourceBase.h"

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

eShaderType IShader::GetType()
{
	return m_ShaderType;
}
