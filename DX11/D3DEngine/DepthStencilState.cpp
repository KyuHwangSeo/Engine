#include "D3DDefine.h"
#include "D3DEngine.h"
#include "DepthStencilState.h"

DepthStencilState::DepthStencilState(string dssName)
	:m_Name(dssName)
{
	m_Device = D3DEngine::GetInstance()->GetDevice();
}

DepthStencilState::~DepthStencilState()
{

}

void DepthStencilState::Create(D3D11_DEPTH_STENCIL_DESC descDSS)
{
	RESET_COM(m_DSS);

	HR(m_Device->CreateDepthStencilState(&descDSS, m_DSS.GetAddressOf()));
}
