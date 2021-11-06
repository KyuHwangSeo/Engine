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
	GRAPHIC_DLL void OnReSize(int screenWidth, int screenheight) abstract;			//��������
	GRAPHIC_DLL void Delete() abstract;

	GRAPHIC_DLL Indexbuffer* CreateIndexBuffer(ParserData::Mesh* mesh) abstract;	//�ε��� ���۸� ������ش�
	GRAPHIC_DLL Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mesh) abstract;	//���ؽ� ���۸� ������ش�
	GRAPHIC_DLL TextureBuffer* CreateTextureBuffer(std::string path) abstract;					//�ؽ��ĸ� ������ش�
};