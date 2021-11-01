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
		return nullptr;
	}

	// 현재 Shader Type 설정..
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
	// 해당 Shader Type에 따른 원본 변환 작업..
	// 해당 함수를 호출하는 것은 이미 자식 클래스가 부모 클래스로 형변환이 된 상태이므로 static_cast로 해도 상관 없다..
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