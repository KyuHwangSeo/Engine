#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Object.h"
#include "Shader.h"

Shader::Shader(const char* shaderName)
{
	SetName(shaderName);
	m_DeviceContext = D3DEngine::GetInstance()->GetDeviceContext();
}

Shader::~Shader()
{
	RESET_COM(m_VS);
	RESET_COM(m_PS);
	RESET_COM(m_InputLayout);

	for (auto& sampler : m_Samplers)
	{
		RESET_COM(sampler.second.sampler);
	}

	for (auto& buf : m_VSConstantBuffers)
	{
		RESET_COM(buf.second.cbuffer);
	}

	for (auto& buf : m_PSConstantBuffers)
	{
		RESET_COM(buf.second.cbuffer);
	}

	for (auto& buf : m_VSBuffers)
	{
		RESET_COM(buf);
	}

	for (auto& buf : m_PSBuffers)
	{
		RESET_COM(buf);
	}
}

void Shader::CreateShader(const wchar_t* vertexFilename, const wchar_t* pixelFilename, const wchar_t* computeFilename)
{
	std::wstring _path = L"../Resource/FX/";

	// 현재는 Vertex, Pixel Shader만 사용하므로 두개만 적용..
	if (wcscmp(vertexFilename, L"") != 0)
		ReflectVS(_path + vertexFilename);

	if (wcscmp(pixelFilename, L"") != 0)
		ReflectPS(_path + pixelFilename);

	if (wcscmp(computeFilename, L"") != 0)
		ReflectCS(_path + computeFilename);
}

void Shader::ReflectVS(wstring vertexFilename)
{
	ID3D11ShaderReflection* pReflector = nullptr;

	// Vertex HLSL Load..
	std::ifstream fin(vertexFilename, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> vS(size);
	fin.read(&vS[0], size);
	fin.close();

	D3DEngine::GetInstance()->GetDevice()->CreateVertexShader(&vS[0], size, nullptr, &m_VS);

	D3DReflect(&vS[0], size, IID_ID3D11ShaderReflection, (void**)&pReflector);

	D3D11_SHADER_DESC shaderDesc;
	pReflector->GetDesc(&shaderDesc);

	/// Input Layout Reflection
	// Shader Input Layout..
	vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
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

	D3DEngine::GetInstance()->GetDevice()->CreateInputLayout(&inputLayoutDesc[0], (UINT)inputLayoutDesc.size(), &vS[0], size, &m_InputLayout);

	/// ConstantBuffer Reflection
	// Vertex Shader ConstantBuffer..
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
			D3DEngine::GetInstance()->GetDevice()->CreateBuffer(&cBufferDesc, nullptr, constantBuffer.GetAddressOf());

			// Key (Constant Buffer Name) && Value (Register Slot, Constant Buffer)
			m_VSBuffers.push_back(constantBuffer);
			m_VSConstantBuffers.insert(make_pair(bufferDesc.Name, ConstantBuffer(register_slot, std::move(constantBuffer))));
		}
	}

	pReflector->Release();
}

void Shader::ReflectPS(wstring pixelFilename)
{
	ID3D11ShaderReflection* pReflector = nullptr;

	// Pixel HLSL Load..
	std::ifstream fin(pixelFilename, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> pS(size);
	fin.read(&pS[0], size);
	fin.close();

	D3DEngine::GetInstance()->GetDevice()->CreatePixelShader(&pS[0], size, nullptr, &m_PS);

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
			D3DEngine::GetInstance()->GetDevice()->CreateBuffer(&cBufferDesc, nullptr, constantBuffer.GetAddressOf());

			// Key (Constant Buffer Name) && Value (Register Slot, Constant Buffer)
			m_PSBuffers.push_back(constantBuffer);
			m_PSConstantBuffers.insert(make_pair(bufferDesc.Name, ConstantBuffer(register_slot, std::move(constantBuffer))));
		}
	}

	/// Texture Resource Reflection
	// Shader Texture Resource..
	for (unsigned int rsindex = 0; rsindex < shaderDesc.BoundResources; rsindex++)
	{
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		pReflector->GetResourceBindingDesc(rsindex, &bindDesc);

		// 현재 리소스가 Texture라면 리스트에 삽입..
		switch (bindDesc.Type)
		{
		case D3D_SIT_TEXTURE:
			m_SRVList.insert(make_pair(bindDesc.Name, bindDesc.BindPoint));
			break;
		case D3D_SIT_SAMPLER:
			m_Samplers.insert(make_pair(bindDesc.Name, Sampler(bindDesc.BindPoint, nullptr)));
			break;
		default:
			break;
		}
	}

	pReflector->Release();
}

void Shader::ReflectCS(wstring computeFilename)
{
	ID3D11ShaderReflection* pReflector = nullptr;

	// Pixel HLSL Load..
	std::ifstream fin(computeFilename, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> cS(size);
	fin.read(&cS[0], size);
	fin.close();

	D3DEngine::GetInstance()->GetDevice()->CreateComputeShader(&cS[0], size, nullptr, &m_CS);

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

			// 현재 읽은 ConstantBuffer Register Slot Check..
			int register_slot = -1;

			D3D11_SHADER_INPUT_BIND_DESC bindDesc;
			pReflector->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc);

			// 바인딩된 리소스 중 같은 이름이 있다면 해당 리소스의 Register Slot 설정..
			register_slot = bindDesc.BindPoint;

			if (FAILED(register_slot))	break;

			// 해당 Constant Buffer 생성..
			D3DEngine::GetInstance()->GetDevice()->CreateBuffer(&cBufferDesc, nullptr, constantBuffer.GetAddressOf());

			// Key (Constant Buffer Name) && Value (Register Slot, Constant Buffer)
			m_CSBuffers.push_back(constantBuffer);
			m_CSConstantBuffers.insert(make_pair(bufferDesc.Name, ConstantBuffer(register_slot, std::move(constantBuffer))));
		}
	}

	/// Texture Resource Reflection
	// Shader Texture Resource..
	for (unsigned int rsindex = 0; rsindex < shaderDesc.BoundResources; rsindex++)
	{
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		pReflector->GetResourceBindingDesc(rsindex, &bindDesc);

		// 현재 리소스가 Texture라면 리스트에 삽입..
		switch (bindDesc.Type)
		{
		case D3D_SIT_TEXTURE:
			m_SRVComputeList.insert(make_pair(bindDesc.Name, bindDesc.BindPoint));
			break;
		case D3D_SIT_SAMPLER:
			m_Samplers.insert(make_pair(bindDesc.Name, Sampler(bindDesc.BindPoint, nullptr)));
			break;
		case D3D_SIT_UAV_RWTYPED:
			m_UAVComputeList.insert(make_pair(bindDesc.Name, bindDesc.BindPoint));
			break;
		default:
			break;
		}
	}

	pReflector->Release();
}

void Shader::SetSamplerState(string samplerName, ComPtr<ID3D11SamplerState> sam)
{
	for (auto& sampler : m_Samplers)
	{
		// 이름이 일치한다면 해당 샘플러로 판정..
		if (sampler.first == samplerName)
		{
			sampler.second.sampler = sam;
			break;
		}
	}
}

void Shader::SetShadowMap(ID3D11ShaderResourceView* tex)
{
	D3DEngine::GetInstance()->GetDeviceContext()->PSSetShaderResources(16, 1, &tex);
}

void Shader::SetDiffuseMap(ID3D11ShaderResourceView* tex)
{
	if (m_SRVList.find("gDiffuseMap") == m_SRVList.end()) return;

	// Pixel Shader Resource 연결..
	m_DeviceContext->PSSetShaderResources(m_SRVList["gDiffuseMap"], 1, &tex);
}

void Shader::SetNormalMap(ID3D11ShaderResourceView* tex)
{
	if (m_SRVList.find("gNormalMap") == m_SRVList.end()) return;

	// Pixel Shader Resource 연결..
	m_DeviceContext->PSSetShaderResources(m_SRVList["gNormalMap"], 1, &tex);
}

void Shader::SetCubeMap(ID3D11ShaderResourceView* tex)
{
	if (m_SRVList.find("gCubeMap") == m_SRVList.end()) return;

	// Pixel Shader Resource 연결..
	m_DeviceContext->PSSetShaderResources(m_SRVList["gCubeMap"], 1, &tex);
}

void Shader::SetShareSRV(UINT registerSlot, ID3D11ShaderResourceView* srv)
{
	// 공유 자원 Resource 업데이트시 사용..
	D3DEngine::GetInstance()->GetDeviceContext()->PSSetShaderResources(registerSlot, 1, &srv);
}

void Shader::SetSRV(string resourceName, ID3D11ShaderResourceView* srv)
{
	if (m_SRVList.find(resourceName) == m_SRVList.end()) return;
	
	m_DeviceContext->PSSetShaderResources(m_SRVList[resourceName], 1, &srv);
}

void Shader::SetSRV(UINT registerSlot, ID3D11ShaderResourceView* srv)
{
	m_DeviceContext->PSSetShaderResources(registerSlot, 1, &srv);
}

void Shader::SetComputeSRV(string resourceName, ID3D11ShaderResourceView* srv)
{
	if (m_SRVComputeList.find(resourceName) == m_SRVComputeList.end()) return;

	m_DeviceContext->CSSetShaderResources(m_SRVComputeList[resourceName], 1, &srv);
}

void Shader::SetComputeSRV(UINT registerSlot, ID3D11ShaderResourceView* srv)
{
	m_DeviceContext->CSSetShaderResources(registerSlot, 1, &srv);
}

void Shader::SetComputeUAV(string resourceName, ID3D11UnorderedAccessView* uav)
{
	if (m_UAVComputeList.find(resourceName) == m_UAVComputeList.end()) return;

	m_DeviceContext->CSSetUnorderedAccessViews(m_UAVComputeList[resourceName], 1, &uav, 0);
}

void Shader::SetComputeUAV(UINT registerSlot, ID3D11UnorderedAccessView* uav)
{
	m_DeviceContext->CSSetUnorderedAccessViews(registerSlot, 1, &uav, 0);
}

void Shader::ResetVSResources()
{
	ID3D11Buffer* cbuf[14] = { nullptr, };
	D3DEngine::GetInstance()->GetDeviceContext()->VSSetConstantBuffers(0, 14, cbuf);
}

void Shader::ResetPSResources()
{
	ID3D11ShaderResourceView* srv[16] = { nullptr, };
	D3DEngine::GetInstance()->GetDeviceContext()->PSSetShaderResources(0, 16, srv);

	ID3D11Buffer* cbuf[14] = { nullptr, };
	D3DEngine::GetInstance()->GetDeviceContext()->PSSetConstantBuffers(0, 14, cbuf);
}

void Shader::ResetCSResources()
{
	ID3D11ShaderResourceView* srv[16] = { nullptr, };
	D3DEngine::GetInstance()->GetDeviceContext()->CSSetShaderResources(0, 16, srv);

	ID3D11UnorderedAccessView* uav[8] = { nullptr, };
	D3DEngine::GetInstance()->GetDeviceContext()->CSSetUnorderedAccessViews(0, 8, uav, 0);
}

void Shader::SetSampler()
{
	// 현 Shader에 Binding된 모든 Sampler Data 설정..
	for (pair<string, Sampler> sampler : m_Samplers)
	{
		m_DeviceContext->PSSetSamplers(sampler.second.register_number, 1, sampler.second.sampler.GetAddressOf());
	}
}

void Shader::SetInputLayout()
{
	// InputLayout 설정.. 
	m_DeviceContext->IASetInputLayout(m_InputLayout.Get());
}

void Shader::VSUpdate()
{
	// Vertex Shader Resource 설정..
	if (!m_VSBuffers.empty())
		m_DeviceContext->VSSetConstantBuffers(0, (UINT)m_VSBuffers.size(), m_VSBuffers[0].GetAddressOf());

	// Vertex Shader 연결..
	if (m_VS != nullptr)
		m_DeviceContext->VSSetShader(m_VS.Get(), nullptr, 0);
	else
		m_DeviceContext->VSSetShader(nullptr, nullptr, 0);

	// Shader InputLayout 설정..
	if (m_InputLayout != nullptr)
		SetInputLayout();
}

void Shader::PSUpdate()
{
	// Pixel Shader Resource 설정..
	if (!m_PSBuffers.empty())
		m_DeviceContext->PSSetConstantBuffers(0, (UINT)m_PSBuffers.size(), m_PSBuffers[0].GetAddressOf());

	// Pixel Shader 연결..
	if (m_PS != nullptr)
		m_DeviceContext->PSSetShader(m_PS.Get(), nullptr, 0);
	else
		m_DeviceContext->PSSetShader(nullptr, nullptr, 0);

	// Shader Sampler 설정..
	if (!m_Samplers.empty())
		SetSampler();
}

void Shader::CSUpdate()
{
	// Compute Shader Resource 설정..
	if (!m_CSBuffers.empty())
		m_DeviceContext->CSSetConstantBuffers(0, (UINT)m_CSBuffers.size(), m_CSBuffers[0].GetAddressOf());

	// Compute Shader 연결..
	if (m_CS != nullptr)
		m_DeviceContext->CSSetShader(m_CS.Get(), nullptr, 0);
}

