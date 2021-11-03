#pragma once
#include "GraphicsEngine.h"

class D3D11Renderer : public GraphicEngine
{
public:
	D3D11Renderer();
	~D3D11Renderer();

public:
	void Initialize(HWND hwnd, int screenWidth, int screenHeight) override;

	void Render(std::queue<MeshData*>* meshList, GlobalData* global) override;
	void OnReSize(int Change_Width, int Change_Height) override;			//리사이즈
	void Delete() override;

	Indexbuffer* CreateIndexBuffer(ParserData::Model* mModel) override;	//인덱스 버퍼를 만들어준다
	Vertexbuffer* CreateVertexBuffer(ParserData::Model* mModel) override;	//버텍스 버퍼를 만들어준다
	TextureBuffer* CreateTextureBuffer(std::string path) override;					//텍스쳐를 만들어준다

	void ShadowRender();
	void MainRender();
	void SSAORender();
	void DeferredRender();
	void AlphaRender();

private:
	ComPtr<ID3D11Device> m_Device = nullptr;
	ComPtr<ID3D11DeviceContext> m_DeviceContext = nullptr;
	ComPtr<IDXGISwapChain> m_SwapChain = nullptr;

private:
	POINT* m_ScreenSize;

private:
	D3D11Graphic* m_Graphic;
	IShaderManager* m_ShaderManager;
};

