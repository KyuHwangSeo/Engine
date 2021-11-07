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

// ���� Shader ��ȯ�� ���� Return Type ����.. (C++17)
using OriginalShader = std::variant<IShader*, VertexShader*, PixelShader*, ComputeShader*>;

/// Shader Reflection MSDN
/// https://docs.microsoft.com/ko-kr/windows/win32/api/d3d11shader/nn-d3d11shader-id3d11shaderreflection?f1url=%3FappId%3DDev16IDEF1%26l%3DKO-KR%26k%3Dk(D3D11SHADER%252FID3D11ShaderReflection);k(ID3D11ShaderReflection);k(DevLang-C%252B%252B);k(TargetOS-Windows)%26rd%3Dtrue
/// Shader Desc MSDN
/// https://docs.microsoft.com/ko-kr/windows/win32/api/d3d11shader/ns-d3d11shader-d3d11_shader_desc?f1url=%3FappId%3DDev16IDEF1%26l%3DKO-KR%26k%3Dk(D3D11SHADER%252FD3D11_SHADER_DESC);k(D3D11_SHADER_DESC);k(DevLang-C%252B%252B);k(TargetOS-Windows)%26rd%3Dtrue

/// <summary>
/// Shader Base Class
/// </summary>
/// 
/// - ��� Shader Class�� Base Class
/// - ��� Shader�� Resource ���� �� ������ Device & DeviceContext�� �ʿ�� �Ͽ� ���� ������ ����

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
	// ���� Shader Type..
	ShaderType m_ShaderType;

public:
	// Shader Load ���� �ʼ��� ���־�� �ϴ� �۾�..
	static void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

	// Shader Load �� Create �Լ�..
	static IShader* CreateShader(ShaderType shaderType, const char* fileName);

	// Device & DeviceContext Reset �Լ�..
	static void Reset();

	// �⺻ Shader ��� ���� �Լ�..
	static void SetShaderRoute(std::string fileRoute);

public:
	// ���� Shader Type ��ȯ �Լ�..
	ShaderType GetType();

	// ���� Shader ���� ��ȯ �Լ�..
	OriginalShader ConvertShader();

	virtual void LoadShader(std::string fileName) abstract;
	virtual void Update() abstract;
};

/// <summary>
/// UpdateSubResource VS Map / UnMap
/// </summary>
/// 
/// - ��Ȯ�� ���ɻ��� �񱳴� ����� ���δ� ã�ƺ� ��� ��Ȳ�� ���� �°� ���� ���� �߿��� �� ����
/// - UpdateSubResource : �� ������ ������Ʈ ���� �ʴ� Buffer�� ��� ����ϱ� ����
///						  ������ �� �ѹ� ������Ʈ�ϴ� �ؽ��� ���� �������� ����ϱ� ������ �ʴ�
/// - Map / UnMap : �� ������ �ѹ� �̻� ���� ������Ʈ �Ǵ� Dynamic Buffer�� ����ϱ� �����ϴ�
///				    Map / UnMap�� ����Ϸ��� Buffer�� ������ Usage �ɼ��� Dynamic���� �������־�� �Ѵ�
