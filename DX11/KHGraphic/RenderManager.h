#pragma once
#include "RenderManagerBase.h"

interface IShaderManager;
interface IGraphicResourceManager;
interface IGraphicResourceFactory;

class RenderManager : public IRenderManager
{
public:
	RenderManager(IGraphicResourceFactory* factory, IGraphicResourceManager* resourceManager, IShaderManager* shaderManager);
	~RenderManager();

public:
	void Initialize() override;
	void Render(std::queue<MeshData*>* meshList, GlobalData* global) override;

private:
	IGraphicResourceFactory* m_Factory;
	IGraphicResourceManager* m_Resource;
	IShaderManager* m_Shader;
};

