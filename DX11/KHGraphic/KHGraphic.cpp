#include "DirectDefine.h"
#include "D3D11Graphic.h"
#include "KHGraphic.h"

#include "ShaderManagerBase.h"
#include "ResourceManagerBase.h"
#include "ResourceFactory.h"
#include "RenderManager.h"
#include "RenderBase.h"

KHGraphic::KHGraphic()
	:m_ResourceFactory(nullptr), m_ResourceMananger(nullptr), m_RenderManager(nullptr)
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
	GraphicResourceFactory* resourceFactory = new GraphicResourceFactory(graphic);
	resourceFactory->Initialize();

	// Resource Manager Pointer..
	IGraphicResourceManager* resourceManager = resourceFactory->GetResourceManager();

	// Shader Manager Pointer..
	IShaderManager* shaderManager = resourceFactory->GetShaderManager();

	// Rendering Initialize..
	RenderBase::Initialize(graphic->GetContext(), resourceFactory, resourceManager, shaderManager);

	// Render Manager 생성 및 초기화..
	m_RenderManager = new RenderManager();
	m_RenderManager->Initialize();

	// Set InterFace Pointer..
	m_ResourceFactory = resourceFactory;
	m_ResourceMananger = resourceManager;
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