#include "DirectDefine.h"
#include "RenderBase.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"

Microsoft::WRL::ComPtr<ID3D11DeviceContext> RenderBase::g_Context = nullptr;
IGraphicResourceFactory* RenderBase::g_Factory = nullptr;
IGraphicResourceManager* RenderBase::g_Resource = nullptr;
IShaderManager* RenderBase::g_Shader = nullptr;

void RenderBase::Initialize(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, IGraphicResourceFactory* factory, IGraphicResourceManager* resourceManager, IShaderManager* shaderManager)
{
	g_Context = context;
	g_Factory = factory;
	g_Resource = resourceManager;
	g_Shader = shaderManager;
}

void RenderBase::Reset()
{
	RESET_COM(g_Context);
	g_Factory = nullptr;
	g_Resource = nullptr;
	g_Shader = nullptr;
}
