#include <vector>
#include "ResourceFactoryBase.h"
#include "ShaderManagerBase.h"
#include "ResourceManagerBase.h"
#include "RenderManager.h"

RenderManager::RenderManager(IGraphicResourceFactory* factory, IGraphicResourceManager* resourceManager, IShaderManager* shaderManager)
	:m_Factory(factory), m_Resource(resourceManager), m_Shader(shaderManager)
{
}

RenderManager::~RenderManager()
{

}

void RenderManager::Initialize()
{

}

void RenderManager::Render(std::queue<MeshData*>* meshList, GlobalData* global)
{

}
