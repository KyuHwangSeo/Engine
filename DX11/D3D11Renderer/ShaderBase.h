#pragma once
#include <wrl.h>
#include <wrl/client.h>
#include <unordered_map>
#include "d3d11.h"

#include "ShaderResourceBase.h"

/// Shader Reflection MSDN
/// https://docs.microsoft.com/ko-kr/windows/win32/api/d3d11shader/nn-d3d11shader-id3d11shaderreflection?f1url=%3FappId%3DDev16IDEF1%26l%3DKO-KR%26k%3Dk(D3D11SHADER%252FID3D11ShaderReflection);k(ID3D11ShaderReflection);k(DevLang-C%252B%252B);k(TargetOS-Windows)%26rd%3Dtrue
/// Shader Desc MSDN
/// https://docs.microsoft.com/ko-kr/windows/win32/api/d3d11shader/ns-d3d11shader-d3d11_shader_desc?f1url=%3FappId%3DDev16IDEF1%26l%3DKO-KR%26k%3Dk(D3D11SHADER%252FD3D11_SHADER_DESC);k(D3D11_SHADER_DESC);k(DevLang-C%252B%252B);k(TargetOS-Windows)%26rd%3Dtrue

/// <summary>
/// UpdateSubResource VS Map / UnMap
/// </summary>
/// 
/// - ��Ȯ�� ���ɻ��� �񱳴� ����� ���δ� ã�ƺ� ��� ��Ȳ�� ���� �°� ���� ���� �߿��� �� ����
/// - UpdateSubResource : �� ������ ������Ʈ ���� �ʴ� Buffer�� ��� ����ϱ� ����
///						  ������ �� �ѹ� ������Ʈ�ϴ� �ؽ��� ���� �������� ����ϱ� ������ �ʴ�
/// - Map / UnMap : �� ������ �ѹ� �̻� ���� ������Ʈ �Ǵ� Dynamic Buffer�� ����ϱ� �����ϴ�
///				    Map / UnMap�� ����Ϸ��� Buffer�� ������ Usage �ɼ��� Dynamic���� �������־�� �Ѵ�

/// <summary>
/// Shader Base Class
/// </summary>
/// 
/// - ��� Shader Class�� Base Class
/// - ��� Shader�� Resource ���� �� ������ Device & DeviceContext�� �ʿ�� �Ͽ� ���� ������ ����

class ShaderBase
{
public:
	ShaderBase() = default;
	virtual ~ShaderBase() = default;

protected:
	// Device & Context..
	static Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;

	// Shader File Route..
	static std::string m_ShaderRoute;

public:
	// Shader Load ���� �ʼ��� ���־�� �ϴ� �۾�..
	static void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

	// Shader Load �� Device & DeviceContext Reset �Լ�..
	static void Reset();

	// �⺻ Shader ��� ���� �Լ�..
	static void SetShaderRoute(std::string fileRoute);

	virtual void CreateShader(const char* fileName) abstract;
	virtual void Update() abstract;
};

void ShaderBase::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	m_Device = device;
	m_DeviceContext = context;
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
