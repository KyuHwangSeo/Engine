#include "DirectDefine.h"
#include "ShaderBase.h"
#include "VertexShader.h"

#include "ResourceBufferHashTable.h"
#include "CompilerDefine.h"
#include <sstream>
#include <fstream>

VertexShader::VertexShader(const char* fileName)
	:ShaderBase(eShaderType::VERTEX)
{
	LoadShader(g_ShaderRoute + fileName);
}

VertexShader::~VertexShader()
{

}

void VertexShader::LoadShader(std::string fileName)
{
	ID3D11ShaderReflection* pReflector = nullptr;

	size_t cbuffer_register_slot = 0;	// ConstantBuffer Max Register Slot
	size_t sampler_register_slot = 0;	// Sampler Max Register Slot
	size_t srv_register_slot = 0;		// ShaderResourceView Max Register Slot
	size_t hash_key = 0;				// Resource Hash Code

	// Vertex HLSL Load..
	std::ifstream fin(fileName, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> vS(size);
	fin.read(&vS[0], size);
	fin.close();

	HR(g_Device->CreateVertexShader(&vS[0], size, nullptr, &m_VS));

	D3DReflect(&vS[0], size, IID_ID3D11ShaderReflection, (void**)&pReflector);

	D3D11_SHADER_DESC shaderDesc;
	pReflector->GetDesc(&shaderDesc);

	/// Input Layout Reflection
	// Shader Input Layout..
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (unsigned inputIndex = 0; inputIndex < shaderDesc.InputParameters; inputIndex++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pReflector->GetInputParameterDesc(inputIndex, &paramDesc);
		
		// Shader Input Data를 기반으로 생성..
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// Shader Data 기반으로 DXGI format 설정..
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		// 현 InputLayout 데이터 삽입..
		inputLayoutDesc.push_back(elementDesc);
	}

	// Shader InputLayout 생성..
	HR(g_Device->CreateInputLayout(&inputLayoutDesc[0], (UINT)inputLayoutDesc.size(), &vS[0], size, &m_InputLayout));

	/// ConstantBuffer Reflection
	// Vertex Shader ConstantBuffer..
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
			m_SamplerList.insert(std::make_pair(hash_key, new SamplerBuffer(bindDesc.Name, sampler_register_slot)));
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

	// Constant Buffer List 최초 설정..
	for (auto& cBuffer : m_ConstantBufferList)
	{
		m_ConstantBuffers[cBuffer.second->register_number] = cBuffer.second->cBuffer;
	}

	pReflector->Release();
}

void VertexShader::Update()
{
	// Vertex Shader 설정..
	g_DeviceContext->VSSetShader(m_VS.Get(), nullptr, 0);

	// Vertex Shader SamplerState 설정..
	if (!m_SamplerStates.empty())
		g_DeviceContext->VSSetSamplers(0, (UINT)m_SamplerStates.size(), m_SamplerStates[0].GetAddressOf());

	// Vertex Shader ConstantBuffer 설정..
	if (!m_ConstantBuffers.empty())
		g_DeviceContext->VSSetConstantBuffers(0, (UINT)m_ConstantBuffers.size(), m_ConstantBuffers[0].GetAddressOf());
	
	// Vertex Shader ShaderResourceView 설정..
	if (!m_ShaderResourceViews.empty())
		g_DeviceContext->VSSetShaderResources(0, (UINT)m_ShaderResourceViews.size(), m_ShaderResourceViews[0].GetAddressOf());

	// Shader InputLayout 설정.. 
	g_DeviceContext->IASetInputLayout(m_InputLayout.Get());
}

void VertexShader::Release()
{
	ShaderBase::Release();

	RESET_COM(m_VS);
	RESET_COM(m_InputLayout);
}
