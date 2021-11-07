#pragma once
#include <unordered_map>
#include <variant>

typedef size_t Hash_Code;

// Shader Type Enum Class..
enum class ShaderType
{
	VERTEX,
	PIXEL,
	COMPUTE
};

class IShader;
class VertexShader;
class PixelShader;
class ComputeShader;

// 원본 Shader 변환을 위한 Return Type 지정.. (C++17)
using OriginalShader = std::variant<IShader*, VertexShader*, PixelShader*, ComputeShader*>;

/// Shader Reflection MSDN
/// https://docs.microsoft.com/ko-kr/windows/win32/api/d3d11shader/nn-d3d11shader-id3d11shaderreflection?f1url=%3FappId%3DDev16IDEF1%26l%3DKO-KR%26k%3Dk(D3D11SHADER%252FID3D11ShaderReflection);k(ID3D11ShaderReflection);k(DevLang-C%252B%252B);k(TargetOS-Windows)%26rd%3Dtrue
/// Shader Desc MSDN
/// https://docs.microsoft.com/ko-kr/windows/win32/api/d3d11shader/ns-d3d11shader-d3d11_shader_desc?f1url=%3FappId%3DDev16IDEF1%26l%3DKO-KR%26k%3Dk(D3D11SHADER%252FD3D11_SHADER_DESC);k(D3D11_SHADER_DESC);k(DevLang-C%252B%252B);k(TargetOS-Windows)%26rd%3Dtrue

/// <summary>
/// Shader Base Class
/// </summary>
/// 
/// - 모든 Shader Class의 Base Class
/// - 모든 Shader가 Resource 생성 및 복제시 Device & DeviceContext를 필요로 하여 전역 변수로 관리

class IShader
{
protected:
	IShader(ShaderType shaderType) : m_ShaderType(shaderType) {}
	virtual ~IShader() = default;

public:
	// Device & Context..
	static Microsoft::WRL::ComPtr<ID3D11Device> g_Device;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> g_DeviceContext;

	// Shader File Route..
	static std::string g_ShaderRoute;

private:
	// 현재 Shader Type..
	ShaderType m_ShaderType;

public:
	// Shader Load 전에 필수로 해주어야 하는 작업..
	static void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

	// Shader Load 및 Create 함수..
	static IShader* CreateShader(ShaderType shaderType, const char* fileName);

	// Device & DeviceContext Reset 함수..
	static void Reset();

	// 기본 Shader 경로 설정 함수..
	static void SetShaderRoute(std::string fileRoute);

public:
	// 현재 Shader Type 반환 함수..
	ShaderType GetType();

	// 현재 Shader 원형 변환 함수..
	OriginalShader ConvertShader();

	virtual void LoadShader(std::string fileName) abstract;
	virtual void Update() abstract;
};

/// <summary>
/// UpdateSubResource VS Map / UnMap
/// </summary>
/// 
/// - 정확한 성능상의 비교는 힘들어 보인다 찾아본 결과 상황에 따라 맞게 쓰는 것이 중요할 것 같다
/// - UpdateSubResource : 매 프레임 업데이트 되지 않는 Buffer일 경우 사용하기 좋음
///						  프레임 당 한번 업데이트하는 텍스쳐 같은 경우까지는 사용하기 나쁘지 않다
/// - Map / UnMap : 매 프레임 한번 이상 자주 업데이트 되는 Dynamic Buffer에 사용하기 적합하다
///				    Map / UnMap을 사용하려면 Buffer를 생성시 Usage 옵션을 Dynamic으로 변경해주어야 한다
