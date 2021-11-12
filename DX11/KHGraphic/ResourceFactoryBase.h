#pragma once
#include <string>
#include <vector>
#include "ResourcesData.h"
#include "ParserData.h"

///
/// 2021/11/07 22:59
/// SeoKyuHwang
///
/// GraphicResourceFactory Interface Class
///
/// - GraphicResource를 생성해주는 Factory Class
/// - 내부적으로 Resource Manager에서 Resource 관리

class BasicRenderTarget;
class ComputeRenderTarget;
interface IShaderManager;
interface IGraphicResourceManager;

interface IGraphicResourceFactory
{
public:
	virtual void Initialize(int width, int height) abstract;
	virtual void Release() abstract;

public:
	// Create Texture 2D Graphic Resource..
	virtual void CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc, ID3D11Texture2D** tex2D) abstract;
	// Create RenderTargetView Graphic Resource..
	virtual void CreateRTV(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, ID3D11RenderTargetView** rtv) abstract;
	// Create ShaderResourceView Graphic Resource..
	virtual void CreateSRV(ID3D11Texture2D* tex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, ID3D11ShaderResourceView** srv) abstract;
	// Create UnorderedAccessView Graphic Resource..
	virtual void CreateUAV(ID3D11Texture2D* tex2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc, ID3D11UnorderedAccessView** uav) abstract;
	// Create DepthStencilView Graphic Resource..
	virtual void CreateDSV(ID3D11Texture2D* tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, ID3D11DepthStencilView** dsv) abstract;

	// Create DepthStencilState Graphic Resource..
	virtual Microsoft::WRL::ComPtr<ID3D11DepthStencilState> CreateDSS(D3D11_DEPTH_STENCIL_DESC* dssDesc) abstract;
	// Create RasterizerState Graphic Resource..
	virtual Microsoft::WRL::ComPtr<ID3D11RasterizerState> CreateRS(D3D11_RASTERIZER_DESC* rsDesc) abstract;
	// Create BlendState Graphic Resource..
	virtual Microsoft::WRL::ComPtr<ID3D11BlendState> CreateBS(D3D11_BLEND_DESC* bsDesc) abstract;
	// Create SamplerState Graphic Resource..
	virtual Microsoft::WRL::ComPtr<ID3D11SamplerState> CreateSS(D3D11_SAMPLER_DESC* ssDesc) abstract;
	
	// Create RenderTarget ViewPort Graphic Resource..
	virtual D3D11_VIEWPORT* CreateViewPort(float topX, float topY, float width, float height, float width_ratio = 1.0f, float height_ratio = 1.0f) abstract;

public:
	// Create MainRenderTarget(BackBuffer) Graphic Resource..
	virtual BasicRenderTarget* CreateMainRenderTarget(UINT width, UINT height) abstract;
	
	// Create BasicRenderTarget(RTV & SRV) Graphic Resource..
	virtual BasicRenderTarget* CreateBasicRenderTarget(ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv) abstract;
	virtual BasicRenderTarget* CreateBasicRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) abstract;
	
	// Create ComputeRenderTarget(RTV & UAV) Graphic Resource..
	virtual ComputeRenderTarget* CreateComputeRenderTarget(ID3D11RenderTargetView** rtv, ID3D11UnorderedAccessView** uav) abstract;
	virtual ComputeRenderTarget* CreateComputeRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;

	// Create Output IndexBuffer Graphic Resource..
	virtual Indexbuffer* CreateIndexBuffer(ParserData::Mesh* mesh) abstract;
	// Create Output VertexBuffer Graphic Resource..
	virtual Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mesh) abstract;
	// Create Output TextureBuffer Graphic Resource..
	virtual TextureBuffer* CreateTextureBuffer(std::string path) abstract;

public:
	virtual IShaderManager* GetShaderManager() abstract;
	virtual IGraphicResourceManager* GetResourceManager() abstract;
};