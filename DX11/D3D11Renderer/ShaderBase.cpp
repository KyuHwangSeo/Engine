#include "ShaderBase.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"

void ShaderBase::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	m_Device = device;
	m_DeviceContext = context;
}

ShaderBase* ShaderBase::CreateShader(ShaderType shaderType, const char* fileName)
{
	ShaderBase* newShader = nullptr;

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
		return nullptr;
	}

	// ���� Shader Type ����..
	newShader->m_ShaderType = shaderType;
	
	return newShader;
}

void ShaderBase::Reset()
{
	m_Device.Reset();
	m_DeviceContext.Reset();
}

void ShaderBase::SetShaderRoute(std::string fileRoute)
{
	m_ShaderRoute = fileRoute;
}

OriginalShader ShaderBase::ConvertShader()
{
	// �ش� Shader Type�� ���� ���� ��ȯ �۾�..
	// �ش� �Լ��� ȣ���ϴ� ���� �̹� �ڽ� Ŭ������ �θ� Ŭ������ ����ȯ�� �� �����̹Ƿ� static_cast�� �ص� ��� ����..
	switch (m_ShaderType)
	{
	case ShaderType::VERTEX:
		return static_cast<VertexShader*>(this);
	case ShaderType::PIXEL:
		return static_cast<PixelShader*>(this);
	case ShaderType::COMPUTE:
		return static_cast<ComputeShader*>(this);
	default:
		throw std::exception("ERROR: Can not Convert Shader.\n");
		return this;
	}
}