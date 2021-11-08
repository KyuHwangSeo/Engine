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

RenderManager::RenderManager(D3D11Graphic* graphic, IGraphicResourceFactory* factory, IGraphicResourceManager* resource, IShaderManager* shader)
{
	// Rendering Initialize..
	RenderBase::Initialize(graphic->GetContext(), factory, resource, shader);
}

RenderManager::~RenderManager()
{

}

void RenderManager::Initialize(int width, int height)
{

}

void RenderManager::Render(std::queue<MeshData*>* meshList, GlobalData* global)
{

}
