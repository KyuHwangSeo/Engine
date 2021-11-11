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
#include "ShadowPass.h"
#include "DeferredPass.h"
#include "LightPass.h"
#include "VertexDefine.h"

RenderManager::RenderManager(D3D11Graphic* graphic, IGraphicResourceFactory* factory)
{
	// Rendering Initialize..
	RenderPassBase::Initialize(graphic->GetContext(), factory, factory->GetResourceManager(), factory->GetShaderManager());

	m_SwapChain = graphic->GetSwapChain();

	m_Deferred = new DeferredPass();
	m_Light = new LightPass();
	m_Shadow = new ShadowPass();

	m_RenderPassList.push_back(m_Deferred);
	m_RenderPassList.push_back(m_Light);
	m_RenderPassList.push_back(m_Shadow);
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
	DirectX::XMMATRIX view = *global->mViewMX;
	DirectX::XMMATRIX proj = *global->mProj;

	ID3D11Buffer* iBuffer = nullptr;
	ID3D11Buffer* vBuffer = nullptr;
	UINT indexCount = 0;
	UINT size = 0;
	UINT offset = 0;

	DirectX::XMMATRIX world;

	m_Deferred->BeginRender();

	while (meshList->size() != 0)
	{
		MeshData* mesh = meshList->front();

		// Camera 제외..
		if (mesh->ObjType == OBJECT_TYPE::Camera)
		{
			meshList->pop();
			continue;
		}

		// Buffer가 없는 Mesh 제외..
		if (mesh->IB == nullptr && mesh->VB == nullptr)
		{
			meshList->pop();
			continue;
		}

		// 현재 Mesh World Matrix 삽입..
		world = mesh->mWorld;

		iBuffer = reinterpret_cast<ID3D11Buffer*>(mesh->IB->IndexBufferPointer);
		vBuffer = reinterpret_cast<ID3D11Buffer*>(mesh->VB->VertexbufferPointer);

		indexCount = mesh->IB->Count;
		size = mesh->VB->VertexDataSize;

		m_Deferred->Render(view, proj, world, vBuffer, iBuffer, size, offset, indexCount);
	}

	// LightPass..
	m_Light->Render();

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
