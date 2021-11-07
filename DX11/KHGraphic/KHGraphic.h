#pragma once
#include "GraphicsEngine.h"
#include "GraphicDLL.h"

interface IRenderManager;
interface IGraphicResourceManager;
interface IGraphicResourceFactory;

class KHGraphic : public GraphicEngine
{
public:
	GRAPHIC_DLL KHGraphic();
	~KHGraphic();

public:
	GRAPHIC_DLL void Initialize(HWND hwnd, int screenWidth, int screenHeight) override;

	GRAPHIC_DLL void Render(std::queue<MeshData*>* meshList, GlobalData* global) override;
	GRAPHIC_DLL void OnReSize(int screenWidth, int screenheight) override;
	GRAPHIC_DLL void Delete() override;

	GRAPHIC_DLL Indexbuffer* CreateIndexBuffer(ParserData::Mesh* mesh) override;
	GRAPHIC_DLL Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mesh) override;
	GRAPHIC_DLL TextureBuffer* CreateTextureBuffer(std::string path) override;

private:
	IGraphicResourceFactory* m_ResourceFactory;

	IGraphicResourceManager* m_ResourceMananger;

	IRenderManager* m_RenderManager;
};

