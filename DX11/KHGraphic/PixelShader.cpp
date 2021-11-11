#include "DirectDefine.h"
#include "ShaderBase.h"
#include "ShaderResourceBase.h"
#include "PixelShader.h"

#include "ResourceBufferHashTable.h"
#include "CompilerDefine.h"
#include <sstream>
#include <fstream>

PixelShader::PixelShader(const char* fileName)
	:IShader(eShaderType::PIXEL)
{
	LoadShader(g_ShaderRoute + fileName);
}

PixelShader::~PixelShader()
{

}

void PixelShader::LoadShader(std::string fileName)
{
	ID3D11ShaderReflection* pReflector = nullptr;

	// Pixel HLSL Load..
	std::ifstream fin(fileName, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> pS(size);
	fin.read(&pS[0], size);
	fin.close();

	HR(g_Device->CreatePixelShader(&pS[0], size, nullptr, &m_PS));

	D3DReflect(&pS[0], size, IID_ID3D11ShaderReflection, (void**)&pReflector);

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
			ComPtr<ID3D11Buffer> constantBuffer = nullptr;
			CD3D11_BUFFER_DESC cBufferDesc(bufferDesc.Size, D3D11_BIND_CONSTANT_BUFFER);

			// 현재 읽은 ConstantBuffer Register Slot Check..
			int register_slot = -1;

			D3D11_SHADER_INPUT_BIND_DESC bindDesc;
			pReflector->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc);

			// 바인딩된 리소스 중 같은 이름이 있다면 해당 리소스의 Register Slot 설정..
			register_slot = bindDesc.BindPoint;

			if (FAILED(register_slot))	break;

			// 해당 Constant Buffer 생성..
			HR(g_Device->CreateBuffer(&cBufferDesc, nullptr, constantBuffer.GetAddressOf()));

			// Constant Buffer Hash Code..
			size_t hash_key = ShaderResourceHashTable::FindHashCode(ShaderResourceHashTable::BufferType::CBUFFER, bufferDesc.Name);

			// Key (Constant Buffer HashCode) && Value (Register Slot, Constant Buffer)
			m_ConstantBuffers.push_back(constantBuffer);
			m_ConstantBufferList.insert(std::make_pair(hash_key, new ConstantBuffer(bindDesc.Name, register_slot, constantBuffer)));
		}
	}

	// Resource 검색할 Hash Code..
	size_t hash_key = 0;

	// Resource 최대 사이즈..
	size_t sampler_size = 0;
	size_t srv_size = 0;

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

			m_SRVList.insert(std::make_pair(hash_key, new ShaderResourceBuffer(bindDesc.Name, bindDesc.BindPoint)));
			srv_size = bindDesc.BindPoint;
		}
			break;
		case D3D_SIT_SAMPLER:
		{
			// Sampler Hash Code..
			hash_key = ShaderResourceHashTable::FindHashCode(ShaderResourceHashTable::BufferType::SAMPLER, bindDesc.Name);
			
			m_SamplerList.insert(std::make_pair(hash_key, new SamplerState(bindDesc.Name, bindDesc.BindPoint)));
			sampler_size = bindDesc.BindPoint;
		}
			break;
		default:
			break;
		}
	}

	// 마지막으로 Binding 된 Resource Register Index 기준으로 사이즈 설정..
	m_SamplerStates.resize(sampler_size);
	m_ShaderResourceViews.resize(srv_size);

	pReflector->Release();
}

void PixelShader::Update()
{
	// Pixel Shader 연결..
	g_DeviceContext->PSSetShader(m_PS.Get(), nullptr, 0);

	// Pixel Shader SamplerState 설정..
	g_DeviceContext->PSSetSamplers(0, (UINT)m_SamplerStates.size(), m_SamplerStates[0].GetAddressOf());

	// Pixel Shader ConstantBuffer 설정..
	g_DeviceContext->PSSetConstantBuffers(0, (UINT)m_ConstantBuffers.size(), m_ConstantBuffers[0].GetAddressOf());

	// Pixel Shader ShaderResourceView 설정..
	g_DeviceContext->PSSetShaderResources(0, (UINT)m_ShaderResourceViews.size(), m_ShaderResourceViews[0].GetAddressOf());
}

void PixelShader::Release()
{
	RESET_COM(m_PS);

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

	m_ConstantBuffers.clear();
	m_SamplerStates.clear();
	m_ShaderResourceViews.clear();
	m_SamplerList.clear();
	m_ConstantBufferList.clear();
	m_SRVList.clear();
}
