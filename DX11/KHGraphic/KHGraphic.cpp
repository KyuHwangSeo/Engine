#include "DirectDefine.h"
#include "D3D11Graphic.h"
#include "KHGraphic.h"

#include "ShaderManagerBase.h"
#include "ResourceManagerBase.h"
#include "ResourceFactory.h"
#include "RenderManager.h"
#include "RenderPassBase.h"

KHGraphic::KHGraphic()
	:m_ResourceFactory(nullptr), m_RenderManager(nullptr)
{

}

KHGraphic::~KHGraphic()
{

}

void KHGraphic::Initialize(HWND hwnd, int screenWidth, int screenHeight)
{
	// DirectX11 Device 생성..
	D3D11Graphic* graphic = new D3D11Graphic();
	graphic->Initialize(hwnd, screenWidth, screenHeight);

	// Resource Factory 생성 및 초기화..
	m_ResourceFactory = new GraphicResourceFactory(graphic);
	m_ResourceFactory->Initialize(screenWidth, screenHeight);

	// Render Manager 생성 및 초기화..
	m_RenderManager = new RenderManager(graphic, m_ResourceFactory);
	m_RenderManager->Initialize(screenWidth, screenHeight);
}

void KHGraphic::Render(std::queue<MeshData*>* meshList, GlobalData* global)
{
	m_RenderManager->Render(meshList, global);
}

void KHGraphic::ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global)
{
	m_RenderManager->ShadowRender(meshList, global);
}

void KHGraphic::SSAORender()
{

}

void KHGraphic::UIRender(std::queue<MeshData*>* meshList, GlobalData* global)
{

}

void KHGraphic::OnReSize(int screenWidth, int screenheight)
{
	m_RenderManager->OnResize(screenWidth, screenheight);
}

void KHGraphic::Delete()
{

}

Indexbuffer* KHGraphic::CreateIndexBuffer(ParserData::Mesh* mesh)
{
	return m_ResourceFactory->CreateIndexBuffer(mesh);
}

Vertexbuffer* KHGraphic::CreateVertexBuffer(ParserData::Mesh* mesh)
{
	return m_ResourceFactory->CreateVertexBuffer(mesh);
}

TextureBuffer* KHGraphic::CreateTextureBuffer(std::string path)
{
	return m_ResourceFactory->CreateTextureBuffer(path);
}