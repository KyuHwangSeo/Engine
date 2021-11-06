#pragma once
#include "GraphicsEngine.h"
#include "GraphicDLL.h"

class IKHGraphic : public GraphicEngine
{
public:
	IKHGraphic();

public:
	GRAPHIC_DLL void Initialize(HWND hwnd, int screenWidth, int screenHeight) abstract;

	GRAPHIC_DLL void Render(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
	GRAPHIC_DLL void OnReSize(int screenWidth, int screenheight) abstract;			//리사이즈
	GRAPHIC_DLL void Delete() abstract;

	GRAPHIC_DLL Indexbuffer* CreateIndexBuffer(ParserData::Mesh* mesh) abstract;	//인덱스 버퍼를 만들어준다
	GRAPHIC_DLL Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mesh) abstract;	//버텍스 버퍼를 만들어준다
	GRAPHIC_DLL TextureBuffer* CreateTextureBuffer(std::string path) abstract;					//텍스쳐를 만들어준다
};