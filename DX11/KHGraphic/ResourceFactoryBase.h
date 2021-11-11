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
	virtual void CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc, ID3D11Texture2D** tex2D) abstract;
	virtual void CreateRTV(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, ID3D11RenderTargetView** rtv) abstract;
	virtual void CreateSRV(ID3D11Texture2D* tex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, ID3D11ShaderResourceView** srv) abstract;
	virtual void CreateUAV(ID3D11Texture2D* tex2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc, ID3D11UnorderedAccessView** uav) abstract;
	virtual void CreateDSV(ID3D11Texture2D* tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, ID3D11DepthStencilView** dsv) abstract;

	virtual Microsoft::WRL::ComPtr<ID3D11DepthStencilState> CreateDSS(D3D11_DEPTH_STENCIL_DESC* dssDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11RasterizerState> CreateRS(D3D11_RASTERIZER_DESC* rsDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11BlendState> CreateBS(D3D11_BLEND_DESC* bsDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11SamplerState> CreateSS(D3D11_SAMPLER_DESC* ssDesc) abstract;
	
	virtual D3D11_VIEWPORT* CreateViewPort(float topX, float topY, float width, float height, float width_ratio = 1.0f, float height_ratio = 1.0f) abstract;

public:
	virtual BasicRenderTarget* CreateMainRenderTarget(UINT width, UINT height) abstract;
	
	virtual BasicRenderTarget* CreateBasicRenderTarget(ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv) abstract;
	virtual BasicRenderTarget* CreateBasicRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) abstract;
	
	virtual ComputeRenderTarget* CreateComputeRenderTarget(ID3D11RenderTargetView** rtv, ID3D11UnorderedAccessView** uav) abstract;
	virtual ComputeRenderTarget* CreateComputeRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;

	virtual Indexbuffer* CreateIndexBuffer(ParserData::Mesh* mesh) abstract;
	virtual Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mesh) abstract;
	virtual TextureBuffer* CreateTextureBuffer(std::string path) abstract;

public:
	virtual IShaderManager* GetShaderManager() abstract;
	virtual IGraphicResourceManager* GetResourceManager() abstract;
};