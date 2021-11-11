#pragma once
#include "RenderManagerBase.h"

class RenderPassBase;
class ShadowPass;
class DeferredPass;
class LightPass;
class RenderManager : public IRenderManager
{
public:
	RenderManager(D3D11Graphic* graphic, IGraphicResourceFactory* factory);
	~RenderManager();

public:
	void Initialize(int width, int height) override;
	void Release() override;

	void Render(std::queue<MeshData*>* meshList, GlobalData* global) override;
	void ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global) override; 
	void SSAORender() override;
	void UIRender(std::queue<MeshData*>* meshList, GlobalData* global) override;

	void OnResize(int width, int height) override;

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

	std::vector<RenderPassBase*> m_RenderPassList;

	ShadowPass* m_Shadow;
	DeferredPass* m_Deferred;
	LightPass* m_Light;
};
