#pragma once
#include "RenderManagerBase.h"

class ShadowRender;
class DeferredRender;
class LightRender;
class RenderManager : public IRenderManager
{
public:
	RenderManager(D3D11Graphic* graphic, IGraphicResourceFactory* factory, IGraphicResourceManager* resource, IShaderManager* shader);
	~RenderManager();

public:
	void Initialize(int width, int height) override;
	void Render(std::queue<MeshData*>* meshList, GlobalData* global) override;

private:
	ShadowRender* m_Shadow;
	DeferredRender* m_Deferred;
	LightRender* m_Light;
};

