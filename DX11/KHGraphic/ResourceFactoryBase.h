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
/// - GraphicResource�� �������ִ� Factory Class
/// - ���������� Resource Manager���� Resource ����

class BasicRenderTarget;
class ComputeRenderTarget;
class ViewPort;

interface IGraphicResourceFactory
{
public:
	virtual void Initialize() abstract;
	virtual void Release() abstract;

public:
	virtual Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateBackBuffer(UINT width, UINT height) abstract;

	virtual Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11RenderTargetView> CreateRTV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateSRV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> CreateUAV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11DepthStencilView> CreateDSV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc) abstract;

	virtual Microsoft::WRL::ComPtr<ID3D11DepthStencilState> CreateDSS(D3D11_DEPTH_STENCIL_DESC* dssDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11RasterizerState> CreateRS(D3D11_RASTERIZER_DESC* rsDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11BlendState> CreateBS(D3D11_BLEND_DESC* bsDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11SamplerState> CreateSS(D3D11_SAMPLER_DESC* ssDesc) abstract;
	
public:
	virtual BasicRenderTarget* CreateMainRenderTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv) abstract;
	virtual BasicRenderTarget* CreateBasicRenderTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv) abstract;
	virtual ComputeRenderTarget* CreateComputeRenderTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv, Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav) abstract;

	virtual ViewPort* CreateViewPort(float width, float height, float width_ratio = 1.0f, float height_ratio = 1.0f) abstract;

	virtual Indexbuffer* CreateIndexBuffer(ParserData::Mesh* mesh) abstract;
	virtual Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mesh) abstract;
	virtual TextureBuffer* CreateTextureBuffer(std::string path) abstract;
};