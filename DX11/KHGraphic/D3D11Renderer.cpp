#include "DirectDefine.h"
#include "EnumDefine.h"
#include "ShaderManager.h"
#include "D3D11Renderer.h"
#include "D3D11Graphic.h"

D3D11Renderer::D3D11Renderer()
{

}

D3D11Renderer::~D3D11Renderer()
{

}

void D3D11Renderer::Initialize(HWND hwnd, int screenWidth, int screenHeight)
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

void D3D11Renderer::Render(std::queue<MeshData*>* meshList, GlobalData* global)
{

}

void D3D11Renderer::OnReSize(int Change_Width, int Change_Height)
{

}

void D3D11Renderer::Delete()
{

}

Indexbuffer* D3D11Renderer::CreateIndexBuffer(ParserData::Model* mModel)
{
	return nullptr;
}

Vertexbuffer* D3D11Renderer::CreateVertexBuffer(ParserData::Model* mModel)
{
	return nullptr;
}

TextureBuffer* D3D11Renderer::CreateTextureBuffer(std::string path)
{
	return nullptr;
}