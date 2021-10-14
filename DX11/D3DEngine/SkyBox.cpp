#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "SkyBox.h"

SkyBox::SkyBox()
	:m_Enable(true)
{
	SetName<SkyBox>();

	m_DeviceContext = D3DEngine::GetInstance()->GetDeviceContext();
	m_Shader = D3DEngine::GetInstance()->GetResourceManager()->GetShader("SkyCubeShader");
	m_Camera = &Camera::g_MainCamera;

	SetVertexBuffer(D3DEngine::GetInstance()->GetResourceManager()->GetVertexBuffer("Sky"));

	D3D11_DEPTH_STENCIL_DESC skycubeDesc;
	skycubeDesc.DepthEnable = true;
	skycubeDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	skycubeDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	skycubeDesc.StencilEnable = false;

	HR(D3DEngine::GetInstance()->GetDevice()->CreateDepthStencilState(&skycubeDesc, m_SkyDeptStencil.GetAddressOf()));
}

SkyBox::~SkyBox()
{

}

void SkyBox::Render(DXMatrix4X4 view, DXMatrix4X4 proj)
{
	if (m_Enable == false) return;

	D3DEngine::GetInstance()->SetNoCull();

	m_DeviceContext->OMGetDepthStencilState(m_BeforDeptStencil.GetAddressOf(), 0);
	m_DeviceContext->OMSetDepthStencilState(m_SkyDeptStencil.Get(), 0);
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DXMatrix4X4 world = KH_MATH::CreateTranslation((*m_Camera)->GetPosition());
	DXMatrix4X4 worldViewProj = world * view * proj;

	m_ObjectData.gWorldViewProj = worldViewProj;
	m_Shader->SetVertexConstantBuffer(m_ObjectData);
	m_Shader->SetCubeMap(m_CubeMapSRV.Get());

	// Vertex Shader Update
	m_Shader->VSUpdate();

	// Pixel Shader Update
	m_Shader->PSUpdate();

	m_DeviceContext->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &m_Stride, &m_Offset);
	m_DeviceContext->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);

	m_DeviceContext->DrawIndexed(m_IndexCount, 0, 0);

	m_DeviceContext->OMSetDepthStencilState(m_BeforDeptStencil.Get(), 0);

	Shader::ResetVSResources();
	Shader::ResetPSResources();
}

void SkyBox::SetVertexBuffer(VertexBuffer* vBuffer)
{
	if (vBuffer == nullptr) return;

	m_VB = vBuffer->VB;
	m_IB = vBuffer->IB;

	m_Stride = vBuffer->Stride;
	m_IndexCount = vBuffer->IndexCount;
	m_Offset = vBuffer->Offset;
}

void SkyBox::SetCubeMap(ID3D11ShaderResourceView* cubeMap)
{
	m_CubeMapSRV = cubeMap;
}
