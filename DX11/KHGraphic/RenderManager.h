#pragma once
#include "RenderManagerBase.h"

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

	void Render(std::queue<MeshData*>* meshList, GlobalData* global) override;
	void ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global) override; 
	void SSAORender() override;
	void UIRender(std::queue<MeshData*>* meshList, GlobalData* global) override;

	void OnResize(int width, int height) override;
	void Release() override;

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

	ShadowPass* m_Shadow;
	DeferredPass* m_Deferred;
	LightPass* m_Light;
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