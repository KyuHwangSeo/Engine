#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "GeometryGenerator.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Collider.h"

Collider::Collider()
	:m_IndexCount(0), m_ColType(eColliderType::Box)
{

}

Collider::~Collider()
{

}

void Collider::Start()
{
	m_DeviceContext = D3DEngine::GetInstance()->GetDeviceContext();

	switch (m_ColType)
	{
	case eColliderType::Box2D:
	case eColliderType::Circle2D:
		m_Shader = ResourceManager::GetInstance()->GetShader("UIColliderShader");
		break;
	case eColliderType::Box:
	case eColliderType::Sphere:
	case eColliderType::Mesh:
		m_Shader = ResourceManager::GetInstance()->GetShader("ColorShader");
		break;
	default:
		break;
	}
}

void Collider::Render(DXMatrix4X4 view, DXMatrix4X4 proj)
{
//#ifdef _DEBUG
	// 피킹중이 아닐 경우 출력하지 않음..
	if (D3DEngine::GetInstance()->GetDebug() == false) return;
	//if (m_GameObject->GetPick() == false) return;

	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DEngine::GetInstance()->SetWireFrame();
	DXMatrix4X4 world = m_GameObject->GetWorld();
	//DXMatrix4X4 world = XMMatrixIdentity();

	DXMatrix4X4 worldViewProj = world * view * proj;

	switch (m_ColType)
	{
	case eColliderType::Box2D:
	case eColliderType::Circle2D:
		worldViewProj = world * proj;
	default:
		break;
	}

	m_ObjectData.gWorldViewProj = worldViewProj;
	m_Shader->SetVertexConstantBuffer(m_ObjectData);

	// Vertex Shader Update
	m_Shader->VSUpdate();

	// Pixel Shader Update
	m_Shader->PSUpdate();

	m_DeviceContext->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &m_Stride, &m_Offset);

	m_DeviceContext->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_DeviceContext->DrawIndexed(m_IndexCount, 0, 0);

	Shader::ResetVSResources();
	Shader::ResetPSResources();
//#endif
}

void Collider::Release()
{
	RESET_COM(m_VB);
	RESET_COM(m_IB);
}