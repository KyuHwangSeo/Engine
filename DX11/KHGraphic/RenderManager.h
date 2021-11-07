#pragma once
#include "RenderManagerBase.h"

class ShadowRender;
class DeferredRender;
class LightRender;
class RenderManager : public IRenderManager
{
public:
	RenderManager();
	~RenderManager();

public:
	void Initialize() override;
	void Render(std::queue<MeshData*>* meshList, GlobalData* global) override;

private:
	ShadowRender* m_Shadow;
	DeferredRender* m_Deferred;
	LightRender* m_Light;
};

