#include "DirectDefine.h"
#include "D3D11Graphic.h"
#include "KHGraphic.h"

#include "ShaderManager.h"
#include "ResourceManager.h"
#include "ResourceFactory.h"
#include "RenderManager.h"

KHGraphic::KHGraphic()
{

}

KHGraphic::~KHGraphic()
{

}

void KHGraphic::Initialize(HWND hwnd, int screenWidth, int screenHeight)
{
	// Device 생성..
	m_Graphic = new D3D11Graphic();
	m_Graphic->Initialize(hwnd, screenWidth, screenHeight);

	// Resource Manager 생성 및 초기화..
	m_ResourceMananger = new GraphicResourceManager(m_Graphic->GetDevice(), m_Graphic->GetSwapChain());

	// Shader Manager 생성 및 초기화..
	m_ShaderManager = new ShaderManager(m_ResourceMananger);
	m_ShaderManager->Initialize(m_Graphic->GetDevice(), m_Graphic->GetContext());

	// Resource Factory 생성 및 초기화..
	m_ResourceFactory = new GraphicResourceFactory(m_ResourceMananger);

	// Render Manager 생성 및 초기화..
	m_RenderManager = new RenderManager();
	m_RenderManager->Initialize();
}

void KHGraphic::Render(std::queue<MeshData*>* meshList, GlobalData* global)
{
	m_RenderManager->Render(meshList, global);
}

void KHGraphic::OnReSize(int screenWidth, int screenheight)
{
	m_ResourceMananger->OnResize(screenWidth, screenheight);
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