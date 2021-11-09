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
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

	ShadowRender* m_Shadow;
	DeferredRender* m_Deferred;
	LightRender* m_Light;
};

//struct RenderData
//{
//	DirectX::XMMATRIX view;
//	DirectX::XMMATRIX proj;
//	DirectX::XMMATRIX world;
//
//	ID3D11Buffer* vb;
//	ID3D11Buffer* ib;
//
//	const UINT size;
//	const UINT offset;
//	UINT indexCount;
//};