#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include "DirectDefine.h"
#include "ResourceFactory.h"
#include "MacroDefine.h"
#include "Texture2D.h"
#include "RenderTargetView.h"
#include "ShaderResourceView.h"
#include "UnorderedAccessView.h"
#include "DepthStecilView.h"
#include "ResourceManager.h"
#include "VertexDefine.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"

GraphicResourceFactory::GraphicResourceFactory(IGraphicResourceManager* manager)
{
	m_ResourceManager = reinterpret_cast<GraphicResourceManager*>(manager);
	m_Device = m_ResourceManager->GetDevice();
	m_SwapChain = m_ResourceManager->GetSwapChain();
}

GraphicResourceFactory::~GraphicResourceFactory()
{

}

Microsoft::WRL::ComPtr<ID3D11Texture2D> GraphicResourceFactory::CreateBackBuffer(UINT width, UINT height)
{
	// Swap Chain, Render Target View Resize
	HR(m_SwapChain->ResizeBuffers(1, (UINT)width, (UINT)height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer = nullptr;

	// Get Swap Chain Back Buffer Pointer..
	HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));

	return backBuffer;
}

Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GraphicResourceFactory::CreateBackBufferRTV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D)
{
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr;

	// RenderTargetView 积己..
	HR(m_Device->CreateRenderTargetView(tex2D.Get(), nullptr, rtv.GetAddressOf()));

	m_ResourceManager->AddBackBufferRTV(new RenderTargetView(tex2D, rtv));

	return rtv;
}

Microsoft::WRL::ComPtr<ID3D11Texture2D> GraphicResourceFactory::CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;

	HR(m_Device->CreateTexture2D(texDesc, 0, tex2D.GetAddressOf()));

	return tex2D;
}

Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GraphicResourceFactory::CreateRTV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc)
{
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr;

	// RenderTargetView 积己..
	HR(m_Device->CreateRenderTargetView(tex2D.Get(), rtvDesc, rtv.GetAddressOf()));

	m_ResourceManager->AddResource(new RenderTargetView(tex2D, rtv));

	return rtv;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GraphicResourceFactory::CreateSRV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;

	// ShaderResourceView 积己..
	HR(m_Device->CreateShaderResourceView(tex2D.Get(), srvDesc, srv.GetAddressOf()));

	m_ResourceManager->AddResource(new ShaderResourceView(tex2D, srv));

	return srv;
}

Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> GraphicResourceFactory::CreateUAV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav = nullptr;

	// UnorderedAccessView 积己..
	HR(m_Device->CreateUnorderedAccessView(tex2D.Get(), uavDesc, uav.GetAddressOf()));

	m_ResourceManager->AddResource(new UnorderedAccessView(tex2D, uav));

	return uav;
}

Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GraphicResourceFactory::CreateDSV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc)
{
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv = nullptr;

	// DepthStencilView 积己..
	HR(m_Device->CreateDepthStencilView(tex2D.Get(), dsvDesc, dsv.GetAddressOf()));

	m_ResourceManager->AddResource(new DepthStecilView(tex2D, dsv));

	return dsv;
}

Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GraphicResourceFactory::CreateDSS(D3D11_DEPTH_STENCIL_DESC* dssDesc)
{
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dss = nullptr;

	// DepthStencilState 积己..
	HR(m_Device->CreateDepthStencilState(dssDesc, dss.GetAddressOf()));

	m_ResourceManager->AddResource(dss);

	return dss;
}

Microsoft::WRL::ComPtr<ID3D11RasterizerState> GraphicResourceFactory::CreateRS(D3D11_RASTERIZER_DESC* rsDesc)
{
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rs = nullptr;

	// RasterizerState 积己..
	HR(m_Device->CreateRasterizerState(rsDesc, rs.GetAddressOf()));

	m_ResourceManager->AddResource(rs);
	
	return rs;
}

Microsoft::WRL::ComPtr<ID3D11BlendState> GraphicResourceFactory::CreateBS(D3D11_BLEND_DESC* bsDesc)
{
	Microsoft::WRL::ComPtr<ID3D11BlendState> bs = nullptr;

	// BlendState 积己..
	HR(m_Device->CreateBlendState(bsDesc, bs.GetAddressOf()));

	m_ResourceManager->AddResource(bs);
	
	return bs;
}

Indexbuffer* GraphicResourceFactory::CreateIndexBuffer(ParserData::Mesh* mesh)
{
	// 货肺款 IndexBufferData 积己..
	Indexbuffer* iBuffer = new Indexbuffer();

	ID3D11Buffer* IB = nullptr;

	// Face Count..
	size_t iCount = mesh->m_IndexList.size();

	std::vector<UINT> indices(iCount * 3);
	for (UINT i = 0; i < iCount; ++i)
	{
		indices[i * 3 + 0] = mesh->m_IndexList[i]->m_Index[0];
		indices[i * 3 + 1] = mesh->m_IndexList[i]->m_Index[1];
		indices[i * 3 + 2] = mesh->m_IndexList[i]->m_Index[2];
	}

	// 货肺款 IndexBuffer 积己..
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * iCount * 3;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &IB));

	// 逞败拎具且 IndexBufferData 火涝..
	iBuffer->Count = iCount * 3;
	iBuffer->IndexBufferPointer = IB;
	iBuffer->size = sizeof(ID3D11Buffer);
	
	return iBuffer;
}

Vertexbuffer* GraphicResourceFactory::CreateVertexBuffer(ParserData::Mesh* mesh)
{
	// 货肺款 VertexBufferData 积己..
	Vertexbuffer* vBuffer = new Vertexbuffer();

	ID3D11Buffer* VB = nullptr;

	// Vertex Count..
	int vCount = mesh->m_VertexList.size();

	std::vector<NormalMapVertex> vertices(vCount);
	for (UINT i = 0; i < vCount; i++)
	{
		vertices[i].Pos = mesh->m_VertexList[i]->m_Pos;

		vertices[i].Normal = mesh->m_VertexList[i]->m_Normal;

		vertices[i].Tex.x = mesh->m_VertexList[i]->m_U;
		vertices[i].Tex.y = mesh->m_VertexList[i]->m_V;

		vertices[i].Tangent = mesh->m_VertexList[i]->m_Tanget;
	}

	// 货肺款 VertexBuffer 积己..
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * vCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &vertices[0];
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &VB));

	// 逞败拎具且 VertexBufferData 火涝..
	vBuffer->Count = vCount;
	vBuffer->VertexbufferPointer = VB;
	vBuffer->size = sizeof(ID3D11Buffer);

	return vBuffer;
}

TextureBuffer* GraphicResourceFactory::CreateTextureBuffer(std::string path)
{
	TextureBuffer* tBuffer = new TextureBuffer();
	
	ID3D11Resource* texResource = nullptr;
	ID3D11ShaderResourceView* newTex = nullptr;
	
	std::wstring wPath(path.begin(), path.end());
	std::wstring file_extension(wPath);
	size_t dotIndex = path.rfind(".");
	file_extension = file_extension.substr(dotIndex, path.size() - dotIndex);

	// 犬厘磊俊 蝶弗 咆胶贸 颇老 肺靛 规侥..
	if (file_extension.compare(L".dds") == 0)
	{
		//HR(CreateDDSTextureFromFile(m_Device.Get(), wPath.c_str(), &texResource, &newTex));
	}
	else
	{
		//HR(CreateWICTextureFromFile(m_Device.Get(), wPath.c_str(), &texResource, &newTex));
	}

	// 逞败拎具且 TextureBufferData 火涝..
	tBuffer->TextureBufferPointer = newTex;
	tBuffer->size = sizeof(ID3D11ShaderResourceView);
	texResource->Release();

	return tBuffer;
}
