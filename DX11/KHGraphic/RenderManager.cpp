#include "DirectDefine.h"
#include "D3D11Graphic.h"
#include "ResourceFactoryBase.h"
#include "ShaderManagerBase.h"
#include "ResourceManagerBase.h"
#include "RenderBase.h"
#include "RenderManager.h"

#include "ShaderBase.h"
#include "ShaderResourceBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "ShadowRender.h"
#include "DeferredRender.h"
#include "LightRender.h"
#include "VertexDefine.h"

RenderManager::RenderManager(D3D11Graphic* graphic, IGraphicResourceFactory* factory, IGraphicResourceManager* resource, IShaderManager* shader)
{
	// Rendering Initialize..
	RenderBase::Initialize(graphic->GetContext(), factory, resource, shader);

	m_SwapChain = graphic->GetSwapChain();

	m_Deferred = new DeferredRender();
	m_Light = new LightRender();
	m_Shadow = new ShadowRender();
}

RenderManager::~RenderManager()
{

}

void RenderManager::Initialize(int width, int height)
{

}

void RenderManager::Render(std::queue<MeshData*>* meshList, GlobalData* global)
{
	DirectX::XMMATRIX view = *global->mViewMX;
	DirectX::XMMATRIX proj = *global->mProj;

	ID3D11Buffer* iBuffer = nullptr;
	ID3D11Buffer* vBuffer = nullptr;
	UINT indexCount = 0;
	const UINT size = sizeof(NormalMapVertex);
	const UINT offset = 0;

	DirectX::XMMATRIX world;

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

		indexCount = mesh->indexCount;

		m_Deferred->Render(view, proj, world, vBuffer, iBuffer, size, offset, indexCount);
		



	}

	// 최종 출력..
	m_SwapChain->Present(0, 0);
}
