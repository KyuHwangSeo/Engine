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
	GRAPHIC_DLL void OnReSize(int Change_Width, int Change_Height) override;			//��������
	GRAPHIC_DLL void Delete() override;

	GRAPHIC_DLL Indexbuffer* CreateIndexBuffer(ParserData::Mesh* mModel) override;	//�ε��� ���۸� ������ش�
	GRAPHIC_DLL Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mModel) override;	//���ؽ� ���۸� ������ش�
	GRAPHIC_DLL TextureBuffer* CreateTextureBuffer(std::string path) override;					//�ؽ��ĸ� ������ش�

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

