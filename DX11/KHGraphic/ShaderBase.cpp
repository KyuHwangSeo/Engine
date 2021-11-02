#include "ShaderBase.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"

void IShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	m_Device = device;
	m_DeviceContext = context;
}

IShader* IShader::CreateShader(ShaderType shaderType, const char* fileName)
{
	IShader* newShader = nullptr;

	// �����Ϸ��� Shader Type�� ���� Shader ����..
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
	m_Device.Reset();
	m_DeviceContext.Reset();
}

void IShader::SetShaderRoute(std::string fileRoute)
{
	m_ShaderRoute = fileRoute;
}

ShaderType IShader::GetType()
{
	return m_ShaderType;
}

OriginalShader IShader::ConvertShader()
{
	// �ش� Shader Type�� ���� ���� ��ȯ �۾�..
	// Shader Type�� ������ �������ش�..
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