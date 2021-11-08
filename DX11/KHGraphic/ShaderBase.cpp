#include "DirectDefine.h"
#include "ShaderBase.h"
#include "ShaderResourceBase.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"

Microsoft::WRL::ComPtr<ID3D11Device> IShader::g_Device = nullptr;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> IShader::g_DeviceContext = nullptr;
std::string IShader::g_ShaderRoute;

void IShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	g_Device = device;
	g_DeviceContext = context;
}

IShader* IShader::CreateShader(eShaderType shaderType, const char* fileName)
{
	IShader* newShader = nullptr;

	// 생성하려는 Shader Type에 따른 Shader 생성..
	switch (shaderType)
	{
	case eShaderType::VERTEX:
		newShader = new VertexShader(fileName);
		break;
	case eShaderType::PIXEL:
		newShader = new PixelShader(fileName);
		break;
	case eShaderType::COMPUTE:
		newShader = new ComputeShader(fileName);
		break;
	default:
		throw std::exception("ERROR: Fail Create Shader.\n"); 
		return newShader;
	}
	
	return newShader;
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
