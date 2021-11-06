#pragma once
#include "KHGraphicBase.h"

class IShaderManager;
class IRenderManager;
class IGraphicResourceManager;
class IGraphicResourceFactory;
class KHGraphic : public IKHGraphic
{
public:
	KHGraphic();
	~KHGraphic();

public:
	GRAPHIC_DLL void Initialize(HWND hwnd, int screenWidth, int screenHeight) override;

	GRAPHIC_DLL void Render(std::queue<MeshData*>* meshList, GlobalData* global) override;
	GRAPHIC_DLL void OnReSize(int screenWidth, int screenheight) override;			//��������
	GRAPHIC_DLL void Delete() override;

	GRAPHIC_DLL Indexbuffer* CreateIndexBuffer(ParserData::Mesh* mesh) override;	//�ε��� ���۸� ������ش�
	GRAPHIC_DLL Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mesh) override;	//���ؽ� ���۸� ������ش�
	GRAPHIC_DLL TextureBuffer* CreateTextureBuffer(std::string path) override;					//�ؽ��ĸ� ������ش�

	//void ShadowRender();
	//void MainRender();
	//void SSAORender();
	//void DeferredRender();
	//void AlphaRender();

private:
	D3D11Graphic* m_Graphic;
	IGraphicResourceManager* m_ResourceMananger;
	IShaderManager* m_ShaderManager;

	IRenderManager* m_RenderManager;

	IGraphicResourceFactory* m_ResourceFactory;
};

