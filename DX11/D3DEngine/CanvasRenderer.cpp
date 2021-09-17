#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Material.h"
#include "Renderer.h"
#include "CanvasRenderer.h"

CanvasRenderer::CanvasRenderer(eRasterizerType renderType, eTopologyType topolType)
	:Renderer(renderType, topolType)
{
	SetName<CanvasRenderer>();
}

CanvasRenderer::~CanvasRenderer()
{
	
}

void CanvasRenderer::Render(DXMatrix4X4 view, DXMatrix4X4 proj)
{
	DXMatrix4X4 world = m_GameObject->GetWorld();
	DXMatrix4X4 identity = XMMatrixIdentity();

	m_UIObjectData.gWorldViewProj = world * proj;
	m_Material->SetVertexConstantBuffer(m_UIObjectData);

	// Material & Shader Update
	m_Material->Update(world, identity, proj);

	// Rendering Type
	switch (m_RasterizerType)
	{
	case eRasterizerType::Wire:
		D3DEngine::GetInstance()->SetWireFrame();
		break;
	case eRasterizerType::Solid:
		D3DEngine::GetInstance()->SetSolid();
		break;
	default:
		break;
	}

	// Rendering..
	Renderer::Render();
}
