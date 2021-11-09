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
	// DirectX11 Device ����..
	D3D11Graphic* graphic = new D3D11Graphic();
	graphic->Initialize(hwnd, screenWidth, screenHeight);

	// Resource Factory ���� �� �ʱ�ȭ..
	GraphicResourceFactory* resourceFactory = new GraphicResourceFactory(graphic);
	resourceFactory->Initialize(screenWidth, screenHeight);

	// Resource Manager Pointer..
	IGraphicResourceManager* resourceManager = resourceFactory->GetResourceManager();

	// Shader Manager Pointer..
	IShaderManager* shaderManager = resourceFactory->GetShaderManager();

	// Render Manager ���� �� �ʱ�ȭ..
	m_RenderManager = new RenderManager(graphic, resourceFactory, resourceManager, shaderManager);
	m_RenderManager->Initialize(screenWidth, screenHeight);

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