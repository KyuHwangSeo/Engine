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

IShader* IShader::CreateShader(ShaderType shaderType, const char* fileName)
{
	IShader* newShader = nullptr;

	// 생성하려는 Shader Type에 따른 Shader 생성..
	switch (shaderType)
	{
	case ShaderType::VERTEX:
		newShader = new VertexShader(fileName);
		break;
	case ShaderType::PIXEL:
		newShader = new PixelShader(fileName);
		break;
	case ShaderType::COMPUTE:
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

ShaderType IShader::GetType()
{
	return m_ShaderType;
}

OriginalShader IShader::ConvertShader()
{
	// 해당 Shader Type에 따른 원본 변환 작업..
	// Shader Type은 생성시 설정해준다..
	switch (m_ShaderType)
	{
	case ShaderType::VERTEX:
		return reinterpret_cast<VertexShader*>(this);
	case ShaderType::PIXEL:
		return reinterpret_cast<PixelShader*>(this);
	case ShaderType::COMPUTE:
		return reinterpret_cast<ComputeShader*>(this);
	default:
		throw std::exception("ERROR: Can not Convert Shader.\n");
		return this;
	}
}