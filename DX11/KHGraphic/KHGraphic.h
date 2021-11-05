#pragma once
#include "GraphicsEngine.h"
#include "GraphicDLL.h"

class KHGraphic : public GraphicEngine
{
public:
	KHGraphic();
	~KHGraphic();

public:
	GRAPHIC_DLL void Initialize(HWND hwnd, int screenWidth, int screenHeight) override;

	GRAPHIC_DLL void Render(std::queue<MeshData*>* meshList, GlobalData* global) override;
	GRAPHIC_DLL void OnReSize(int Change_Width, int Change_Height) override;			//리사이즈
	GRAPHIC_DLL void Delete() override;

	GRAPHIC_DLL Indexbuffer* CreateIndexBuffer(ParserData::Mesh* mModel) override;	//인덱스 버퍼를 만들어준다
	GRAPHIC_DLL Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mModel) override;	//버텍스 버퍼를 만들어준다
	GRAPHIC_DLL TextureBuffer* CreateTextureBuffer(std::string path) override;					//텍스쳐를 만들어준다

	//void ShadowRender();
	//void MainRender();
	//void SSAORender();
	//void DeferredRender();
	//void AlphaRender();

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

