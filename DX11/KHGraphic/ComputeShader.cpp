#include "ComputeShader.h"
#include "ShaderResourceHash.h"
#include <d3dcompiler.h>
#include <sstream>
#include <fstream>

using namespace Microsoft::WRL;

ComputeShader::ComputeShader(const char* fileName)
	:IShader(ShaderType::COMPUTE)
{
	LoadShader(m_ShaderRoute + fileName);
}

ComputeShader::~ComputeShader()
{

}

void ComputeShader::LoadShader(std::string fileName)
{
	ID3D11ShaderReflection* pReflector = nullptr;

	// Pixel HLSL Load..
	std::ifstream fin(fileName, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> cS(size);
	fin.read(&cS[0], size);
	fin.close();

	m_Device->CreateComputeShader(&cS[0], size, nullptr, &m_CS);

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
			ComPtr<ID3D11Buffer> constantBuffer = nullptr;
			CD3D11_BUFFER_DESC cBufferDesc(bufferDesc.Size, D3D11_BIND_CONSTANT_BUFFER);

			// ���� ���� ConstantBuffer Register Slot Check..
			int register_slot = -1;

			D3D11_SHADER_INPUT_BIND_DESC bindDesc;
			pReflector->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc);

			// ���ε��� ���ҽ� �� ���� �̸��� �ִٸ� �ش� ���ҽ��� Register Slot ����..
			register_slot = bindDesc.BindPoint;

			if (FAILED(register_slot))	break;

			// �ش� Constant Buffer ����..
			m_Device->CreateBuffer(&cBufferDesc, nullptr, constantBuffer.GetAddressOf());

			// Constant Buffer Hash Code..
			size_t hash_key = ShaderResourceHashTable::FindHashCode(ShaderResourceHashTable::ResourceType::CBUFFER, bufferDesc.Name);

			// Key (Constant Buffer HashCode) && Value (Register Slot, Constant Buffer)
			m_ConstantBuffers.push_back(constantBuffer);
			m_ConstantBufferList.insert(std::make_pair(hash_key, ConstantBuffer(bindDesc.Name, register_slot, constantBuffer)));
		}
	}

	// Resource �˻��� Hash Code..
	size_t hash_key;

	// Resource �ִ� ������..
	size_t sampler_size;
	size_t srv_size;
	size_t uav_size;

	/// Shader Resource Reflection
	// Shader Resource..
	for (unsigned int rsindex = 0; rsindex < shaderDesc.BoundResources; rsindex++)
	{
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		pReflector->GetResourceBindingDesc(rsindex, &bindDesc);

		// Resource Type�� �´� �ش� List�� ����..
		switch (bindDesc.Type)
		{
		case D3D_SIT_TEXTURE:
		{
			// SRV Hash Code..
			hash_key = ShaderResourceHashTable::FindHashCode(ShaderResourceHashTable::ResourceType::SRV, bindDesc.Name);

			m_SRVList.insert(std::make_pair(hash_key, ShaderResourceView(bindDesc.Name, bindDesc.BindPoint)));
			srv_size = bindDesc.BindPoint;
		}
		break;
		case D3D_SIT_SAMPLER:
		{
			// Constant Buffer Hash Code..
			hash_key = ShaderResourceHashTable::FindHashCode(ShaderResourceHashTable::ResourceType::SAMPLER, bindDesc.Name);

			m_SamplerList.insert(std::make_pair(hash_key, SamplerState(bindDesc.Name, bindDesc.BindPoint)));
			sampler_size = bindDesc.BindPoint;
		}
		break;
		case D3D_SIT_UAV_RWTYPED:
		{
			// UAV Hash Code..
			hash_key = ShaderResourceHashTable::FindHashCode(ShaderResourceHashTable::ResourceType::UAV, bindDesc.Name);

			m_UAVList.insert(std::make_pair(hash_key, UnorderedAccessView(bindDesc.Name, bindDesc.BindPoint)));
			uav_size = bindDesc.BindPoint;
		}
		break;
		default:
			break;
		}
	}

	// ���������� Binding �� Resource Register Index �������� ������ ����..
	m_SamplerStates.resize(sampler_size);
	m_ShaderResourceViews.resize(srv_size);
	m_UnorderedAccessViews.resize(uav_size);

	pReflector->Release();
}

void ComputeShader::Update()
{
	// Compute Shader ����..
	m_DeviceContext->CSSetShader(m_CS.Get(), nullptr, 0);

	// Compute Shader ShaderSampler ����..
	m_DeviceContext->CSSetSamplers(0, (UINT)m_SamplerStates.size(), m_SamplerStates[0].GetAddressOf());

	// Compute Shader ConstantBuffer ����..
	m_DeviceContext->CSSetConstantBuffers(0, (UINT)m_ConstantBuffers.size(), m_ConstantBuffers[0].GetAddressOf());

	// Compute Shader ShaderResourceView ����..
	m_DeviceContext->CSSetShaderResources(0, (UINT)m_ShaderResourceViews.size(), m_ShaderResourceViews[0].GetAddressOf());

	// Compute Shader UnorderedAccessView ����..
	m_DeviceContext->CSSetUnorderedAccessViews(0, (UINT)m_UnorderedAccessViews.size(), m_UnorderedAccessViews[0].GetAddressOf(), 0);
}