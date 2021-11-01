#include "PixelShader.h"
#include "ShaderResourceHash.h"
#include <d3dcompiler.h>
#include <sstream>
#include <fstream>

using namespace Microsoft::WRL;

PixelShader::PixelShader(const char* fileName)
{
	CreateShader(m_ShaderRoute + fileName);
}

PixelShader::~PixelShader()
{

}

void PixelShader::CreateShader(std::string fileName)
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

	m_Device->CreatePixelShader(&pS[0], size, nullptr, &m_PS);

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
			size_t hash_key = ShaderResourceHashTable::FindHashCode(ShaderResourceHashTable::Type::CBUFFER, bufferDesc.Name);

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
			hash_key = ShaderResourceHashTable::FindHashCode(ShaderResourceHashTable::Type::SRV, bindDesc.Name);

			m_SRVList.insert(std::make_pair(hash_key, ShaderResourceView(bindDesc.Name, bindDesc.BindPoint)));
			srv_size = bindDesc.BindPoint;
		}
			break;
		case D3D_SIT_SAMPLER:
		{
			// Constant Buffer Hash Code..
			hash_key = ShaderResourceHashTable::FindHashCode(ShaderResourceHashTable::Type::SAMPLER, bindDesc.Name);

			m_SamplerList.insert(std::make_pair(hash_key, SamplerState(bindDesc.Name, bindDesc.BindPoint)));
			sampler_size = bindDesc.BindPoint;
		}
			break;
		default:
			break;
		}
	}

	// ���������� Binding �� Resource Register Index �������� ������ ����..
	m_SamplerStates.resize(sampler_size);
	m_ShaderResourceViews.resize(srv_size);

	pReflector->Release();
}

void PixelShader::Update()
{
	// Pixel Shader ����..
	m_DeviceContext->PSSetShader(m_PS.Get(), nullptr, 0);

	// Pixel Shader SamplerState ����..
	m_DeviceContext->PSSetSamplers(0, (UINT)m_SamplerStates.size(), m_SamplerStates[0].GetAddressOf());

	// Pixel Shader ConstantBuffer ����..
	m_DeviceContext->PSSetConstantBuffers(0, (UINT)m_ConstantBuffers.size(), m_ConstantBuffers[0].GetAddressOf());

	// Pixel Shader ShaderResourceView ����..
	m_DeviceContext->PSSetShaderResources(0, (UINT)m_ShaderResourceViews.size(), m_ShaderResourceViews[0].GetAddressOf());
}
