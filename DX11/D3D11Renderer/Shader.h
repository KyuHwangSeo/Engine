#pragma once
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
/// ConstantBuffer
/// </summary>
/// 
/// - Shader ���ο� ���ǵ��ִ� Constant Buffer�� 1:1 �����ϴ� ����ü
/// - Shader���� ������ �������� ��ȣ�� ���۰� ��ġ�ϴ� �Ѱ��� Constant Buffer
/// - D3DCompiler���� ���� Shader���� ConstantBuffer�� ������ �ξ ������� ������
///   Register Slot�� �ø��� �ʴ´�..

typedef unsigned int register_slot;

struct ConstantBuffer
{
	ConstantBuffer(unsigned int rNum, ComPtr<ID3D11Buffer> cbuf) : register_number(rNum), cbuffer(cbuf) {}
	ConstantBuffer() = default;

	// Register Slot Number
	register_slot register_number;

	// Constant Buffer ���� Buffer
	ComPtr<ID3D11Buffer> cbuffer;
};

struct Sampler
{
	Sampler(unsigned int rNum, ComPtr<ID3D11SamplerState> sam) : register_number(rNum), sampler(sam) {}
	Sampler() = default;

	// Register Slot Number
	register_slot register_number;

	// Constant Buffer ���� Buffer
	ComPtr<ID3D11SamplerState> sampler;
};

/// <summary>
/// Shader Class
/// </summary>
///
/// - Vertex & Pixel Shader �� ������ Shader Class
///   
/// - Shader Load �� ���ÿ� Shader Reflection�� ���� Input Layout & ConstantBuffer ���� �� ����
/// - Effect11 ���������� Shader Reflection�� ���� Name���� Value�� �������°� ������ 
///	  Shader�� �Ľ��ϴ°��� Reflection�� �Ǵ� �� ������ ���� �� �𸣰ٴ�..
/// - �ش� Shader�� ConstantBuffer�� �����ϴ� struct�� �����θ� ��� Shader�� ȣȯ ����
/// - Input Layout ������ ������� �ʿ䰡 ����.
/// - Texture Resource ���� Constant Buffer�� ���� Binding�� Resource�� ����Ʈ�� Register Slot Number�� �Բ� ����
/// - Sampler ���� ���� ���� ������� �ε������� Name�� ���� �������� ���ο��� �������־�� �� �� ����.
///   ����ν� ���� �������� Sampler�� �̸��� ���߰� ResourceManager���� ���� �� �̸����� ���Ͽ� �־��ִ� ���.

class Shader
{
public:
	Shader(const char* shaderName);
	~Shader();

private:
	ComPtr<ID3D11DeviceContext> m_DeviceContext;

	ComPtr<ID3D11VertexShader> m_VS;
	ComPtr<ID3D11PixelShader> m_PS;
	ComPtr<ID3D11ComputeShader> m_CS;

	// Shader Input Layout
	ComPtr<ID3D11InputLayout> m_InputLayout;

	// Shader Sampler
	unordered_map<std::string, Sampler> m_Samplers;

	// Shader�� ����� ��� ���� ����Ʈ..
	unordered_map<std::string, ConstantBuffer> m_VSConstantBuffers;
	unordered_map<std::string, ConstantBuffer> m_PSConstantBuffers;
	unordered_map<std::string, ConstantBuffer> m_CSConstantBuffers;

	// �ѹ��� �����ϱ� ���� ���ӵ� �޸𸮰����� �ִ� ���۵�..
	vector<ComPtr<ID3D11Buffer>> m_VSBuffers;
	vector<ComPtr<ID3D11Buffer>> m_PSBuffers;
	vector<ComPtr<ID3D11Buffer>> m_CSBuffers;

	// Pixel Shader�� ����� ���ҽ� ����Ʈ..
	unordered_map<std::string, register_slot> m_SRVList;

	// Compute Shader�� ����� ���ҽ� ����Ʈ..
	unordered_map<std::string, register_slot> m_SRVComputeList;
	unordered_map<std::string, register_slot> m_UAVComputeList;

public:
	void CreateShader(const wchar_t* vertexFilename, const wchar_t* pixelFilename, const wchar_t* computeFilename);
	void ReflectVS(wstring vertexFilename);
	void ReflectPS(wstring pixelFilename);
	void ReflectCS(wstring computeFilename);
	void SetSamplerState(std::string samplerName, ComPtr<ID3D11SamplerState> sam);

	/// �ַ� ����ϴ� �Լ����� ����� ���� ����..
	///////////////////////////////////////////////////////////////////////////////////////////
	// Texture Resource Input Function
	static void SetShadowMap(ID3D11ShaderResourceView* tex);
	void SetDiffuseMap(ID3D11ShaderResourceView* tex);
	void SetNormalMap(ID3D11ShaderResourceView* tex);
	void SetCubeMap(ID3D11ShaderResourceView* tex);
	///////////////////////////////////////////////////////////////////////////////////////////

	// Vertex Shader ���� ConstantBuffer Update �Լ�
	template <typename T>
	void SetVertexConstantBuffer(T cBuffer);

	// Pixel Shader ���� ConstantBuffer Update �Լ�
	template <typename T>
	void SetPixelConstantBuffer(T cBuffer);

	// Vertex & Pixel Shader ���� Resource Set �Լ�
	static void SetShareSRV(UINT registerSlot, ID3D11ShaderResourceView* srv);
	void SetSRV(std::string resourceName, ID3D11ShaderResourceView* srv);
	void SetSRV(UINT registerSlot, ID3D11ShaderResourceView* srv);

	// Compute ���� ConstantBuffer Update �Լ�
	template <typename T>
	void SetComputeConstantBuffer(T cBuffer);

	// Compute Shader ���� Resource Set �Լ�
	void SetComputeSRV(std::string resourceName, ID3D11ShaderResourceView* srv);
	void SetComputeSRV(UINT registerSlot, ID3D11ShaderResourceView* srv);

	void SetComputeUAV(std::string resourceName, ID3D11UnorderedAccessView* uav);
	void SetComputeUAV(UINT registerSlot, ID3D11UnorderedAccessView* uav);

	static void ResetVSResources();
	static void ResetPSResources();
	static void ResetCSResources();

	// Sampler ���� �Լ�
	void SetSampler();

	// InputLayout ���� �Լ�
	void SetInputLayout();

	// Vertex Shader Update
	void VSUpdate();
	// Pixel Shader Update
	void PSUpdate();
	// Compute Shader Update
	void CSUpdate();
};

template <typename T>
inline void Shader::SetVertexConstantBuffer(T cBuffer)
{
	// �ش� Value ã��..
	unordered_map<std::string, ConstantBuffer>::iterator it = m_VSConstantBuffers.find(cBuffer.GetName());

	// �ش� Key�� ���� Value�� ���ٸ�..
	if (it == m_VSConstantBuffers.end()) return;

	// Resource ����..
	m_DeviceContext->UpdateSubresource(it->second.cbuffer.Get(), 0, nullptr, &cBuffer, 0, 0);
}

template <typename T>
inline void Shader::SetPixelConstantBuffer(T cBuffer)
{
	// �ش� Value ã��..
	unordered_map<std::string, ConstantBuffer>::iterator it = m_PSConstantBuffers.find(cBuffer.GetName());

	// �ش� Key�� ���� Value�� ���ٸ�..
	if (it == m_PSConstantBuffers.end()) return;

	// Resource ����..
	m_DeviceContext->UpdateSubresource(it->second.cbuffer.Get(), 0, nullptr, &cBuffer, 0, 0);
}

template <typename T>
inline void Shader::SetComputeConstantBuffer(T cBuffer)
{
	// �ش� Value ã��..
	unordered_map<std::string, ConstantBuffer>::iterator it = m_CSConstantBuffers.find(cBuffer.GetName());

	// �ش� Key�� ���� Value�� ���ٸ�..
	if (it == m_CSConstantBuffers.end()) return;

	// Resource ����..
	m_DeviceContext->UpdateSubresource(it->second.cbuffer.Get(), 0, nullptr, &cBuffer, 0, 0);
}
