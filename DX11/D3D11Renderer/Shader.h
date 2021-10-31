#pragma once
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
/// ConstantBuffer
/// </summary>
/// 
/// - Shader 내부에 정의되있는 Constant Buffer와 1:1 대응하는 구조체
/// - Shader에서 설정한 레지스터 번호와 버퍼가 일치하는 한개의 Constant Buffer
/// - D3DCompiler에서 현재 Shader에서 ConstantBuffer을 선언해 두어도 사용하지 않으면
///   Register Slot에 올리지 않는다..

typedef unsigned int register_slot;

struct ConstantBuffer
{
	ConstantBuffer(unsigned int rNum, ComPtr<ID3D11Buffer> cbuf) : register_number(rNum), cbuffer(cbuf) {}
	ConstantBuffer() = default;

	// Register Slot Number
	register_slot register_number;

	// Constant Buffer 전용 Buffer
	ComPtr<ID3D11Buffer> cbuffer;
};

struct Sampler
{
	Sampler(unsigned int rNum, ComPtr<ID3D11SamplerState> sam) : register_number(rNum), sampler(sam) {}
	Sampler() = default;

	// Register Slot Number
	register_slot register_number;

	// Constant Buffer 전용 Buffer
	ComPtr<ID3D11SamplerState> sampler;
};

/// <summary>
/// Shader Class
/// </summary>
///
/// - Vertex & Pixel Shader 한 묶음의 Shader Class
///   
/// - Shader Load 와 동시에 Shader Reflection을 통해 Input Layout & ConstantBuffer 생성 및 보관
/// - Effect11 내부적으로 Shader Reflection을 통해 Name으로 Value를 가져오는거 같은데 
///	  Shader를 파싱하는건지 Reflection이 되는 것 인지는 아직 잘 모르겟다..
/// - 해당 Shader의 ConstantBuffer와 대응하는 struct만 만들어두면 모든 Shader와 호환 가능
/// - Input Layout 조차도 만들어줄 필요가 없다.
/// - Texture Resource 또한 Constant Buffer와 같이 Binding된 Resource는 리스트에 Register Slot Number와 함께 보관
/// - Sampler 또한 위와 같은 방식으로 로드하지만 Name에 따른 설정값은 내부에서 지정해주어야 할 것 같다.
///   현재로썬 같은 설정값의 Sampler는 이름을 맞추고 ResourceManager에서 생성 후 이름값을 비교하여 넣어주는 방식.

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

	// Shader와 연결된 상수 버퍼 리스트..
	unordered_map<std::string, ConstantBuffer> m_VSConstantBuffers;
	unordered_map<std::string, ConstantBuffer> m_PSConstantBuffers;
	unordered_map<std::string, ConstantBuffer> m_CSConstantBuffers;

	// 한번에 설정하기 위한 연속된 메모리공간에 있는 버퍼들..
	vector<ComPtr<ID3D11Buffer>> m_VSBuffers;
	vector<ComPtr<ID3D11Buffer>> m_PSBuffers;
	vector<ComPtr<ID3D11Buffer>> m_CSBuffers;

	// Pixel Shader와 연결된 리소스 리스트..
	unordered_map<std::string, register_slot> m_SRVList;

	// Compute Shader와 연결된 리소스 리스트..
	unordered_map<std::string, register_slot> m_SRVComputeList;
	unordered_map<std::string, register_slot> m_UAVComputeList;

public:
	void CreateShader(const wchar_t* vertexFilename, const wchar_t* pixelFilename, const wchar_t* computeFilename);
	void ReflectVS(wstring vertexFilename);
	void ReflectPS(wstring pixelFilename);
	void ReflectCS(wstring computeFilename);
	void SetSamplerState(std::string samplerName, ComPtr<ID3D11SamplerState> sam);

	/// 주로 사용하는 함수들은 만들어 놓고 쓰자..
	///////////////////////////////////////////////////////////////////////////////////////////
	// Texture Resource Input Function
	static void SetShadowMap(ID3D11ShaderResourceView* tex);
	void SetDiffuseMap(ID3D11ShaderResourceView* tex);
	void SetNormalMap(ID3D11ShaderResourceView* tex);
	void SetCubeMap(ID3D11ShaderResourceView* tex);
	///////////////////////////////////////////////////////////////////////////////////////////

	// Vertex Shader 전용 ConstantBuffer Update 함수
	template <typename T>
	void SetVertexConstantBuffer(T cBuffer);

	// Pixel Shader 전용 ConstantBuffer Update 함수
	template <typename T>
	void SetPixelConstantBuffer(T cBuffer);

	// Vertex & Pixel Shader 전용 Resource Set 함수
	static void SetShareSRV(UINT registerSlot, ID3D11ShaderResourceView* srv);
	void SetSRV(std::string resourceName, ID3D11ShaderResourceView* srv);
	void SetSRV(UINT registerSlot, ID3D11ShaderResourceView* srv);

	// Compute 전용 ConstantBuffer Update 함수
	template <typename T>
	void SetComputeConstantBuffer(T cBuffer);

	// Compute Shader 전용 Resource Set 함수
	void SetComputeSRV(std::string resourceName, ID3D11ShaderResourceView* srv);
	void SetComputeSRV(UINT registerSlot, ID3D11ShaderResourceView* srv);

	void SetComputeUAV(std::string resourceName, ID3D11UnorderedAccessView* uav);
	void SetComputeUAV(UINT registerSlot, ID3D11UnorderedAccessView* uav);

	static void ResetVSResources();
	static void ResetPSResources();
	static void ResetCSResources();

	// Sampler 설정 함수
	void SetSampler();

	// InputLayout 설정 함수
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
	// 해당 Value 찾기..
	unordered_map<std::string, ConstantBuffer>::iterator it = m_VSConstantBuffers.find(cBuffer.GetName());

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_VSConstantBuffers.end()) return;

	// Resource 복제..
	m_DeviceContext->UpdateSubresource(it->second.cbuffer.Get(), 0, nullptr, &cBuffer, 0, 0);
}

template <typename T>
inline void Shader::SetPixelConstantBuffer(T cBuffer)
{
	// 해당 Value 찾기..
	unordered_map<std::string, ConstantBuffer>::iterator it = m_PSConstantBuffers.find(cBuffer.GetName());

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_PSConstantBuffers.end()) return;

	// Resource 복제..
	m_DeviceContext->UpdateSubresource(it->second.cbuffer.Get(), 0, nullptr, &cBuffer, 0, 0);
}

template <typename T>
inline void Shader::SetComputeConstantBuffer(T cBuffer)
{
	// 해당 Value 찾기..
	unordered_map<std::string, ConstantBuffer>::iterator it = m_CSConstantBuffers.find(cBuffer.GetName());

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_CSConstantBuffers.end()) return;

	// Resource 복제..
	m_DeviceContext->UpdateSubresource(it->second.cbuffer.Get(), 0, nullptr, &cBuffer, 0, 0);
}
