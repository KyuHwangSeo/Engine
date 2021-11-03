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
	void OnReSize(int Change_Width, int Change_Height) override;			//��������
	void Delete() override;

	Indexbuffer* CreateIndexBuffer(ParserData::Model* mModel) override;	//�ε��� ���۸� ������ش�
	Vertexbuffer* CreateVertexBuffer(ParserData::Model* mModel) override;	//���ؽ� ���۸� ������ش�
	TextureBuffer* CreateTextureBuffer(std::string path) override;					//�ؽ��ĸ� ������ش�

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

