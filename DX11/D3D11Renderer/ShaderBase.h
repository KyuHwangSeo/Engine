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
/// - 정확한 성능상의 비교는 힘들어 보인다 찾아본 결과 상황에 따라 맞게 쓰는 것이 중요할 것 같다
/// - UpdateSubResource : 매 프레임 업데이트 되지 않는 Buffer일 경우 사용하기 좋음
///						  프레임 당 한번 업데이트하는 텍스쳐 같은 경우까지는 사용하기 나쁘지 않다
/// - Map / UnMap : 매 프레임 한번 이상 자주 업데이트 되는 Dynamic Buffer에 사용하기 적합하다
///				    Map / UnMap을 사용하려면 Buffer를 생성시 Usage 옵션을 Dynamic으로 변경해주어야 한다

/// <summary>
/// Shader Base Class
/// </summary>
/// 
/// - 모든 Shader Class의 Base Class
/// - 모든 Shader가 Resource 생성 및 복제시 Device & DeviceContext를 필요로 하여 전역 변수로 관리

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
	// Shader Load 전에 필수로 해주어야 하는 작업..
	static void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

	// Shader Load 후 Device & DeviceContext Reset 함수..
	static void Reset();

	// 기본 Shader 경로 설정 함수..
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
