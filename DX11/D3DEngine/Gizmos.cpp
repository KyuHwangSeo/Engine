#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Gizmos.h"

Gizmos::Gizmos(eGizmoType gizmoType, DXVector3 scale, DXVector3 rot, DXVector3 pos)
	:m_ObjectTrans(nullptr), m_GizmoType(gizmoType), m_Enable(true)
{
	SetName<Gizmos>();

	m_OffsetTM = KH_MATH::CreateScale(scale) * KH_MATH::CreateRotationEuler(rot) * KH_MATH::CreateTranslation(pos);

	m_DeviceContext = D3DEngine::GetInstance()->GetDeviceContext();
	m_Shader = ResourceManager::GetInstance()->GetShader("ColorShader");

	switch (m_GizmoType)
	{
	case eGizmoType::Axis:
		SetVertexBuffer(ResourceManager::GetInstance()->GetVertexBuffer("AxisGizmo"));
		m_TopologyType = eTopologyType::Line;
		break;
	case eGizmoType::Camera:
		SetVertexBuffer(ResourceManager::GetInstance()->GetVertexBuffer("CameraGizmo"));
		m_TopologyType = eTopologyType::Triangle;
		break;
	case eGizmoType::Light:
		SetVertexBuffer(ResourceManager::GetInstance()->GetVertexBuffer("LightGizmo"));
		m_TopologyType = eTopologyType::Triangle;
		break;
	default:
		break;
	}
}

Gizmos::~Gizmos()
{

}

void Gizmos::Render(DXMatrix4X4 view, DXMatrix4X4 proj)
{
#ifdef _DEBUG
	if (D3DEngine::GetInstance()->GetDebug() == false) return;

	switch (m_TopologyType)
	{
	case eTopologyType::Triangle:
		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	case eTopologyType::Line:
		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		break;
	default:
		break;
	}

	D3DEngine::GetInstance()->SetWireFrame();

	// Set constants
	DXMatrix4X4 world = m_OffsetTM * m_ObjectTrans->m_WorldTM;
	DXMatrix4X4 worldViewProj = world * view * proj;

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
#endif
}

void Gizmos::Release()
{
	RESET_COM(m_DeviceContext);

	RESET_COM(m_VB);
	RESET_COM(m_IB);
}

void Gizmos::SetVertexBuffer(VertexBuffer* vBuffer)
{
	if (vBuffer == nullptr) return;

	m_VB = vBuffer->VB;
	m_IB = vBuffer->IB;

	m_Stride = vBuffer->Stride;
	m_Offset = vBuffer->Offset;

	m_IndexCount = vBuffer->IndexCount;
}

void Gizmos::SetGameObject(GameObject* obj)
{
	m_ObjectTrans = obj->GetTransform();
}
