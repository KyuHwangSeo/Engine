#include "DirectDefine.h"
#include "ShaderBase.h"
#include "ShaderResourceBase.h"
#include "ComputeShader.h"

#include "ResourceBufferHashTable.h"
#include "CompilerDefine.h"
#include <sstream>
#include <fstream>

ComputeShader::ComputeShader(const char* fileName)
	:IShader(eShaderType::COMPUTE)
{
	LoadShader(g_ShaderRoute + fileName);
}

ComputeShader::~ComputeShader()
{

}

void ComputeShader::LoadShader(std::string fileName)
{
	ID3D11ShaderReflection* pReflector = nullptr;

	size_t cbuffer_register_slot = 0;	// ConstantBuffer Max Register Slot
	size_t sampler_register_slot = 0;	// Sampler Max Register Slot
	size_t srv_register_slot = 0;		// ShaderResourceView Max Register Slot
	size_t uav_register_slot = 0;		// UnorderedAccessView Max Register Slot
	size_t hash_key = 0;				// Resource Hash Code

	// Pixel HLSL Load..
	std::ifstream fin(fileName, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> cS(size);
	fin.read(&cS[0], size);
	fin.close();

	HR(g_Device->CreateComputeShader(&cS[0], size, nullptr, &m_CS));

	D3DReflect(&cS[0], size, IID_ID3D11ShaderReflection, (void**)&pReflector);

	D3D11_SHADER_DESC shaderDesc;
	pReflector->GetDesc(&shaderDesc);

	/// ConstantBuffer Reflection
	// Pixel Shader ConstantBuffer..
	for (unsigned int cbindex = 0; cbindex < shaderDesc.ConstantBuffers; cbindex++)
	{
		ID3D11ShaderReflectionConstantBuffer* cBuffer = pReflector->GetConstantBufferByIndex(cbindex);
		D3D11_SHADER_BUFFER_DESC bufferDesc;

		if (SUCCEEDED(cBuffer->GetDesc(&bufferDesc)))
		{
			ID3D11Buffer* cBuffer = nullptr;
			CD3D11_BUFFER_DESC cBufferDesc(bufferDesc.Size, D3D11_BIND_CONSTANT_BUFFER);

			// 현재 읽은 ConstantBuffer Register Slot Check..

			D3D11_SHADER_INPUT_BIND_DESC bindDesc;
			pReflector->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc);

			// 해당 Constant Buffer 생성..
			HR(g_Device->CreateBuffer(&cBufferDesc, nullptr, &cBuffer));

			// Constant Buffer Hash Code..
			hash_key = ShaderResourceHashTable::FindHashCode(ShaderResourceHashTable::BufferType::CBUFFER, bufferDesc.Name);

			// Constant Buffer Register Slot Number..
			cbuffer_register_slot = bindDesc.BindPoint;

			// Key (Constant Buffer HashCode) && Value (Register Slot, Constant Buffer)
			m_ConstantBufferList.insert(std::make_pair(hash_key, new ConstantBuffer(bindDesc.Name, cbuffer_register_slot, &cBuffer)));
		}
	}

	/// Shader Resource Reflection
	// Shader Resource..
	for (unsigned int rsindex = 0; rsindex < shaderDesc.BoundResources; rsindex++)
	{
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		pReflector->GetResourceBindingDesc(rsindex, &bindDesc);

		// Resource Type에 맞는 해당 List에 삽입..
		switch (bindDesc.Type)
		{
		case D3D_SIT_TEXTURE:
		{
			// SRV Hash Code..
			hash_key = ShaderResourceHashTable::FindHashCode(ShaderResourceHashTable::BufferType::SRV, bindDesc.Name);

			// SRV Register Slot Number..
			srv_register_slot = bindDesc.BindPoint;

			// SRV 추가..
			m_SRVList.insert(std::make_pair(hash_key, new ShaderResourceBuffer(bindDesc.Name, srv_register_slot)));
		}
		break;
		case D3D_SIT_SAMPLER:
		{
			// Sampler Hash Code..
			hash_key = ShaderResourceHashTable::FindHashCode(ShaderResourceHashTable::BufferType::SAMPLER, bindDesc.Name);

			// Sampler Register Slot Number..
			sampler_register_slot = bindDesc.BindPoint;

			// Sampler 추가..
			m_SamplerList.insert(std::make_pair(hash_key, new SamplerState(bindDesc.Name, sampler_register_slot)));
		}
		break;
		case D3D_SIT_UAV_RWTYPED:
		{
			// UAV Hash Code..
			hash_key = ShaderResourceHashTable::FindHashCode(ShaderResourceHashTable::BufferType::UAV, bindDesc.Name);

			// UAV Register Slot Number..
			uav_register_slot = bindDesc.BindPoint;

			// UAV 추가..
			m_UAVList.insert(std::make_pair(hash_key, new UnorderedAccessBuffer(bindDesc.Name, uav_register_slot)));
		}
		break;
		default:
			break;
		}
	}

	// 마지막으로 Binding 된 Resource Register Index 기준으로 사이즈 설정..
	m_ConstantBuffers.resize(++cbuffer_register_slot);
	m_SamplerStates.resize(++sampler_register_slot);
	m_ShaderResourceViews.resize(++srv_register_slot);
	m_UnorderedAccessViews.resize(++uav_register_slot);

	// Constant Buffer List 최초 설정..
	for (auto& cBuffer : m_ConstantBufferList)
	{
		m_ConstantBuffers[cBuffer.second->register_number] = cBuffer.second->cBuffer;
	}

	pReflector->Release();
}

void ComputeShader::Update()
{
	// Compute Shader 연결..
	g_DeviceContext->CSSetShader(m_CS.Get(), nullptr, 0);

	// Compute Shader ShaderSampler 설정..
	if (!m_SamplerStates.empty())
		g_DeviceContext->CSSetSamplers(0, (UINT)m_SamplerStates.size(), m_SamplerStates[0].GetAddressOf());

	// Compute Shader ConstantBuffer 설정..
	if (!m_ConstantBuffers.empty())
		g_DeviceContext->CSSetConstantBuffers(0, (UINT)m_ConstantBuffers.size(), m_ConstantBuffers[0].GetAddressOf());

	// Compute Shader ShaderResourceView 설정..
	if (!m_ShaderResourceViews.empty())
		g_DeviceContext->CSSetShaderResources(0, (UINT)m_ShaderResourceViews.size(), m_ShaderResourceViews[0].GetAddressOf());

	// Compute Shader UnorderedAccessView 설정..
	if (!m_UnorderedAccessViews.empty())
		g_DeviceContext->CSSetUnorderedAccessViews(0, (UINT)m_UnorderedAccessViews.size(), m_UnorderedAccessViews[0].GetAddressOf(), 0);
}

void ComputeShader::Release()
{
	RESET_COM(m_CS);

	for (Microsoft::WRL::ComPtr<ID3D11Buffer> cBuffer : m_ConstantBuffers)
	{
		RESET_COM(cBuffer);
	}

	for (Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler : m_SamplerStates)
	{
		RESET_COM(sampler);
	}

	for (Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv : m_ShaderResourceViews)
	{
		RESET_COM(srv);
	}

	for (Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav : m_UnorderedAccessViews)
	{
		RESET_COM(uav);
	}

	for (std::pair<Hash_Code, ConstantBuffer*> cBuffer : m_ConstantBufferList)
	{
		SAFE_DELETE(cBuffer.second);
	}

	for (std::pair<Hash_Code, SamplerState*> sampler : m_SamplerList)
	{
		SAFE_DELETE(sampler.second);
	}

	for (std::pair<Hash_Code, ShaderResourceBuffer*> srv : m_SRVList)
	{
		SAFE_DELETE(srv.second);
	}

	for (std::pair<Hash_Code, UnorderedAccessBuffer*> uav : m_UAVList)
	{
		SAFE_DELETE(uav.second);
	}

	m_ConstantBuffers.clear();
	m_SamplerStates.clear();
	m_ShaderResourceViews.clear();
	m_UnorderedAccessViews.clear();
	m_SamplerList.clear();
	m_ConstantBufferList.clear();
	m_SRVList.clear();
	m_UAVList.clear();
}
