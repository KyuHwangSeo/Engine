#include "DirectDefine.h"
#include "ShaderManager.h"
#include "D3D11Graphic.h"
#include "KHGraphic.h"

#include "ResourceManager.h"
#include "ResourceFactory.h"

KHGraphic::KHGraphic()
{

}

KHGraphic::~KHGraphic()
{

}

void KHGraphic::Initialize(HWND hwnd, int screenWidth, int screenHeight)
{
	m_ScreenSize = new POINT();
	m_ScreenSize->x = screenWidth;
	m_ScreenSize->y = screenHeight;

	// Device 积己..
	m_Graphic = new D3D11Graphic();
	m_Graphic->Initialize(hwnd, screenWidth, screenHeight);

	// Shader Manager 积己 棺 檬扁拳..
	m_ShaderManager = new ShaderManager();
	m_ShaderManager->Initialize(m_Device, m_DeviceContext);

}

void KHGraphic::Render(std::queue<MeshData*>* meshList, GlobalData* global)
{

}

void KHGraphic::OnReSize(int Change_Width, int Change_Height)
{

}

void KHGraphic::Delete()
{

}

Indexbuffer* KHGraphic::CreateIndexBuffer(ParserData::Mesh* mModel)
{
	return nullptr;
}

Vertexbuffer* KHGraphic::CreateVertexBuffer(ParserData::Mesh* mModel)
{
	return nullptr;
}

TextureBuffer* KHGraphic::CreateTextureBuffer(std::string path)
{
	return nullptr;
}