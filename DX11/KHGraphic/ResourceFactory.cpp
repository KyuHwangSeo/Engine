#include <vector>
#include "DirectDefine.h"
#include "D3D11Graphic.h"
#include "ViewPort.h"
#include "Texture2D.h"
#include "DepthStencilView.h"
#include "RenderTargetBase.h"
#include "BasicRenderTarget.h"
#include "ComputeRenderTarget.h"
#include "ResourceManager.h"
#include "ShaderBase.h"
#include "ShaderManager.h"
#include "ResourceFactory.h"

#include "VertexDefine.h"
#include "ResourceBufferHashTable.h"
#include "SamplerStateDefine.h"
#include "ToolKitDefine.h"

GraphicResourceFactory::GraphicResourceFactory(D3D11Graphic* graphic)
{
	// Graphic Resource & Shader Manager 积己..
	m_ShaderManager = new ShaderManager();
	m_ResourceManager = new GraphicResourceManager();

	m_Device = graphic->GetDevice();
	m_Context = graphic->GetContext();
	m_SwapChain = graphic->GetSwapChain();
}

GraphicResourceFactory::~GraphicResourceFactory()
{

}

void GraphicResourceFactory::Initialize(int width, int height)
{
	/// Global Resource 积己..
	CreateDepthStencilState();
	CreateRasterizerState();
	CreateSamplerState();
	CreateBlendState();

	CreateDepthStencilView();
	CreateViewPort(width, height);

	// Graphic Resource & Shader Manager 檬扁拳..
	m_ResourceManager->Initialize(m_Device, m_SwapChain);
	m_ShaderManager->Initialize(m_Device, m_Context);
}

void GraphicResourceFactory::Release()
{
	RESET_COM(m_Device);
	RESET_COM(m_Context);
	RESET_COM(m_SwapChain);

	SAFE_DELETE(m_ShaderManager);
	SAFE_DELETE(m_ResourceManager);
}

void GraphicResourceFactory::CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc, ID3D11Texture2D** tex2D)
{
	// Texture2D Resource 积己..
	HR(m_Device->CreateTexture2D(texDesc, 0, tex2D));
}

void GraphicResourceFactory::CreateRTV(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, ID3D11RenderTargetView** rtv)
{
	// RenderTargetView Resource 积己..
	HR(m_Device->CreateRenderTargetView(tex2D, rtvDesc, rtv));
}

void GraphicResourceFactory::CreateSRV(ID3D11Texture2D* tex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, ID3D11ShaderResourceView** srv)
{
	// ShaderResourceView Resource 积己..
	HR(m_Device->CreateShaderResourceView(tex2D, srvDesc, srv));
}

void GraphicResourceFactory::CreateUAV(ID3D11Texture2D* tex2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc, ID3D11UnorderedAccessView** uav)
{
	// UnorderedAccessView Resource 积己..
	HR(m_Device->CreateUnorderedAccessView(tex2D, uavDesc, uav));
}

void GraphicResourceFactory::CreateDSV(ID3D11Texture2D* tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, ID3D11DepthStencilView** dsv)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> newResource = nullptr;

	if (dsv != nullptr)
	{
		// DepthStencilView Resource 积己..
		HR(m_Device->CreateDepthStencilView(tex2D, dsvDesc, dsv));

		// Resource Point Copy..
		newResource = *dsv;
	}
	else
	{
		// DepthStencilView Resource 积己..
		HR(m_Device->CreateDepthStencilView(tex2D, dsvDesc, newResource.GetAddressOf()));
	}

	// DepthStencilView 积己..
	DepthStencilView* depthStencilView = new DepthStencilView(newResource.GetAddressOf());

	// Resoure 殿废..
	m_ResourceManager->AddResource(depthStencilView);
}

Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GraphicResourceFactory::CreateDSS(D3D11_DEPTH_STENCIL_DESC* dssDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> newResource = nullptr;
	
	// DepthStencilState Resource 积己..
	HR(m_Device->CreateDepthStencilState(dssDesc, newResource.GetAddressOf()));

	// Resoure 殿废..
	m_ResourceManager->AddResource(newResource);

	return newResource;
}

Microsoft::WRL::ComPtr<ID3D11RasterizerState> GraphicResourceFactory::CreateRS(D3D11_RASTERIZER_DESC* rsDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> newResource = nullptr;
	
	// RasterizerState Resource 积己..
	HR(m_Device->CreateRasterizerState(rsDesc, newResource.GetAddressOf()));

	// Resoure 殿废..
	m_ResourceManager->AddResource(newResource);

	return newResource;
}

Microsoft::WRL::ComPtr<ID3D11BlendState> GraphicResourceFactory::CreateBS(D3D11_BLEND_DESC* bsDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11BlendState> newResource = nullptr;
	
	// BlendState Resource 积己..
	HR(m_Device->CreateBlendState(bsDesc, newResource.GetAddressOf()));

	// Resoure 殿废..
	m_ResourceManager->AddResource(newResource);

	return newResource;
}

Microsoft::WRL::ComPtr<ID3D11SamplerState> GraphicResourceFactory::CreateSS(D3D11_SAMPLER_DESC* ssDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11SamplerState> newResource = nullptr;

	// SamplerState Resource 积己..
	HR(m_Device->CreateSamplerState(ssDesc, newResource.GetAddressOf()));

	// Resoure 殿废..
	m_ResourceManager->AddResource(newResource);

	return newResource;
}

D3D11_VIEWPORT* GraphicResourceFactory::CreateViewPort(float topX, float topY, float width, float height, float width_ratio, float height_ratio)
{
	// ViewPort 积己..
	ViewPort* viewPort = new ViewPort(topX, topY, width, height, width_ratio, height_ratio);

	// Resoure 殿废..
	m_ResourceManager->AddResource(viewPort);

	return viewPort->GetViewPort();
}

BasicRenderTarget* GraphicResourceFactory::CreateMainRenderTarget(UINT width, UINT height)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;

	// Swap Chain, Render Target View Resize
	HR(m_SwapChain->ResizeBuffers(1, (UINT)width, (UINT)height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	// Get Swap Chain Back Buffer Pointer..
	HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(tex2D.GetAddressOf())));

	CreateRTV(tex2D.Get(), nullptr, rtv.GetAddressOf());
	CreateSRV(tex2D.Get(), nullptr, srv.GetAddressOf());

	// Main RenderTarget 积己..
	BasicRenderTarget* mainRenderTarget = new BasicRenderTarget(rtv.GetAddressOf(), srv.GetAddressOf());

	// Resource 殿废..
	m_ResourceManager->AddMainRenderTarget(mainRenderTarget);

	// Reset Pointer..
	RESET_COM(tex2D);
	RESET_COM(rtv);
	RESET_COM(srv);

	return mainRenderTarget;
}

BasicRenderTarget* GraphicResourceFactory::CreateBasicRenderTarget(ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv)
{
	// Basic RenderTarget 积己..
	BasicRenderTarget* basicRenderTarget = new BasicRenderTarget(rtv, srv);

	// Resource 殿废..
	m_ResourceManager->AddResource(basicRenderTarget);

	return basicRenderTarget;
}

BasicRenderTarget* GraphicResourceFactory::CreateBasicRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	ComPtr<ID3D11RenderTargetView> rtv = nullptr;
	ComPtr<ID3D11ShaderResourceView> srv = nullptr;

	CreateRTV(tex2D, rtvDesc, rtv.GetAddressOf());
	CreateSRV(tex2D, srvDesc, srv.GetAddressOf());

	// Basic RenderTarget 积己..
	BasicRenderTarget* basicRenderTarget = new BasicRenderTarget(rtv.GetAddressOf(), srv.GetAddressOf());

	// Resource 殿废..
	m_ResourceManager->AddResource(basicRenderTarget);

	return basicRenderTarget;
}

ComputeRenderTarget* GraphicResourceFactory::CreateComputeRenderTarget(ID3D11RenderTargetView** rtv, ID3D11UnorderedAccessView** uav)
{
	// Compute RenderTarget 积己..
	ComputeRenderTarget* computeRenderTarget = new ComputeRenderTarget(rtv, uav);

	// Resource 殿废..
	m_ResourceManager->AddResource(computeRenderTarget);

	return computeRenderTarget;
}

ComputeRenderTarget* GraphicResourceFactory::CreateComputeRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	ComPtr<ID3D11RenderTargetView> rtv = nullptr;
	ComPtr<ID3D11UnorderedAccessView> uav = nullptr;

	CreateRTV(tex2D, rtvDesc, rtv.GetAddressOf());
	CreateUAV(tex2D, uavDesc, uav.GetAddressOf());

	// Compute RenderTarget 积己..
	ComputeRenderTarget* computeRenderTarget = new ComputeRenderTarget(rtv.GetAddressOf(), uav.GetAddressOf());

	// Resource 殿废..
	m_ResourceManager->AddResource(computeRenderTarget);

	return computeRenderTarget;
}

Indexbuffer* GraphicResourceFactory::CreateIndexBuffer(ParserData::Mesh* mesh)
{
	// 货肺款 IndexBufferData 积己..
	Indexbuffer* iBuffer = new Indexbuffer();

	ID3D11Buffer* IB = nullptr;

	// Face Count..
	UINT iCount = (UINT)mesh->m_IndexList.size();

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
	UINT vCount = (UINT)mesh->m_VertexList.size();

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
		HR(DirectX::CreateDDSTextureFromFile(m_Device.Get(), wPath.c_str(), &texResource, &newTex));
	}
	else
	{
		HR(DirectX::CreateWICTextureFromFile(m_Device.Get(), wPath.c_str(), &texResource, &newTex));
	}

	// 逞败拎具且 TextureBufferData 火涝..
	tBuffer->TextureBufferPointer = newTex;
	tBuffer->size = sizeof(ID3D11ShaderResourceView);
	texResource->Release();

	return tBuffer;
}

IShaderManager* GraphicResourceFactory::GetShaderManager()
{
	return m_ShaderManager;
}

IGraphicResourceManager* GraphicResourceFactory::GetResourceManager()
{
	return m_ResourceManager;
}

void GraphicResourceFactory::CreateDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Defalt DepthStencilState 积己..
	CreateDSS(&depthStencilDesc);

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// NoDepth DepthStencilState 积己..
	CreateDSS(&depthStencilDesc);
}

void GraphicResourceFactory::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.MultisampleEnable = true;
	rasterizerDesc.ScissorEnable = false;

	// Solid RasterizerState 积己..
	CreateRS(&rasterizerDesc);

	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.AntialiasedLineEnable = true;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;

	// WireFrame RasterizerState 积己..
	CreateRS(&rasterizerDesc);

	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	// NoCull RasterizerState 积己..
	CreateRS(&rasterizerDesc);

	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.DepthBias = 100000;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.005f;

	// Depth RasterizerState 积己..
	CreateRS(&rasterizerDesc);
}

void GraphicResourceFactory::CreateSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samClampMinLinear SamplerState 积己..
	m_ShaderManager->AddSampler(samClampMinLinear::GetHashCode(), CreateSS(&samplerDesc));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samWrapAnisotropic SamplerState 积己..
	m_ShaderManager->AddSampler(samWrapAnisotropic::GetHashCode(), CreateSS(&samplerDesc));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samWrapMinLinear SamplerState 积己..
	m_ShaderManager->AddSampler(samWrapMinLinear::GetHashCode(), CreateSS(&samplerDesc));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samMirrorMinLinear SamplerState 积己..
	m_ShaderManager->AddSampler(samMirrorMinLinear::GetHashCode(), CreateSS(&samplerDesc));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samClampMinLinearPoint SamplerState 积己..
	m_ShaderManager->AddSampler(samClampMinLinearPoint::GetHashCode(), CreateSS(&samplerDesc));
	
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1e5f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samBorderLinerPoint SamplerState 积己..
	m_ShaderManager->AddSampler(samBorderLinerPoint::GetHashCode(), CreateSS(&samplerDesc));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samWrapLinerPoint SamplerState 积己..
	m_ShaderManager->AddSampler(samWrapLinerPoint::GetHashCode(), CreateSS(&samplerDesc));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// gShadowSam SamplerState 积己..
	m_ShaderManager->AddSampler(gShadowSam::GetHashCode(), CreateSS(&samplerDesc));
}

void GraphicResourceFactory::CreateBlendState()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = TRUE;
	blendDesc.IndependentBlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// Blending First RenderTarget BlendState 积己..
	CreateBS(&blendDesc);
}

void GraphicResourceFactory::CreateDepthStencilView()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	CreateTexture2D(&texDesc, tex2D.GetAddressOf());

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	//dsvDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	// Defalt DepthStencilView 积己..
	CreateDSV(tex2D.Get(), &dsvDesc, nullptr);
}

void GraphicResourceFactory::CreateViewPort(int width, int height)
{
	// Defalt ViewPort 积己..
	CreateViewPort(0.0f, 0.0f, width, height);
}