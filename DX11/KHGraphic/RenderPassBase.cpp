#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"

Microsoft::WRL::ComPtr<ID3D11DeviceContext> RenderPassBase::g_Context = nullptr;
IGraphicResourceFactory* RenderPassBase::g_Factory = nullptr;
IGraphicResourceManager* RenderPassBase::g_Resource = nullptr;
IShaderManager* RenderPassBase::g_Shader = nullptr;

void RenderPassBase::Initialize(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, IGraphicResourceFactory* factory, IGraphicResourceManager* resourceManager, IShaderManager* shaderManager)
{
	g_Context = context;
	g_Factory = factory;
	g_Resource = resourceManager;
	g_Shader = shaderManager;
}

void RenderPassBase::Reset()
{
	RESET_COM(g_Context);
	g_Factory = nullptr;
	g_Resource = nullptr;
	g_Shader = nullptr;
}
