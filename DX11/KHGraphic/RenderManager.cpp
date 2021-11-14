#include "DirectDefine.h"
#include "D3D11Graphic.h"
#include "ResourceFactoryBase.h"
#include "ShaderManagerBase.h"
#include "ResourceManagerBase.h"
#include "RenderPassBase.h"
#include "RenderManager.h"

#include "ShaderBase.h"
#include "ShaderResourceBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"

#include "MathDefine.h"
#include "ForwardPass.h"
#include "ShadowPass.h"
#include "DeferredPass.h"
#include "LightPass.h"
#include "VertexDefine.h"

RenderManager::RenderManager(D3D11Graphic* graphic, IGraphicResourceFactory* factory)
{
	// Rendering Initialize..
	RenderPassBase::Initialize(graphic->GetContext(), factory, factory->GetResourceManager(), factory->GetShaderManager());

	m_SwapChain = graphic->GetSwapChain();

	m_Farward = new ForwardPass();
	//m_Deferred = new DeferredPass();
	//m_Light = new LightPass();
	//m_Shadow = new ShadowPass();

	m_RenderPassList.push_back(m_Farward);
	//m_RenderPassList.push_back(m_Deferred);
	//m_RenderPassList.push_back(m_Light);
	//m_RenderPassList.push_back(m_Shadow);
}

RenderManager::~RenderManager()
{

}

void RenderManager::Initialize(int width, int height)
{
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		renderPass->Initialize(width, height);
	}
}

void RenderManager::Release()
{
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		RELEASE_COM(renderPass);
	}

	m_RenderPassList.clear();
}

void RenderManager::Render(std::queue<MeshData*>* meshList, GlobalData* global)
{
	m_Farward->BeginRender();

	while (meshList->size() != 0)
	{
		MeshData* mesh = meshList->front();

		switch (mesh->ObjType)
		{
		case OBJECT_TYPE::Base:
		case OBJECT_TYPE::Skinning:
			m_Farward->Update(mesh, global);
			m_Farward->Render(mesh);
			break;
		}

		meshList->pop();
	}

	// 최종 출력..
	m_SwapChain->Present(0, 0);
}

void RenderManager::ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global)
{

}

void RenderManager::SSAORender()
{

}

void RenderManager::UIRender(std::queue<MeshData*>* meshList, GlobalData* global)
{

}

void RenderManager::OnResize(int width, int height)
{
	RenderPassBase::g_Resource->OnResize(width, height);

	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		renderPass->OnResize(width, height);
	}
}
