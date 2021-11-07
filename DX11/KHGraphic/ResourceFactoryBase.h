#pragma once
#include <string>
#include <wrl.h>
#include <d3d11.h>
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

interface IGraphicResourceFactory
{
public:
	virtual void Initialize() abstract;

	virtual Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateBackBuffer(UINT width, UINT height) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11RenderTargetView> CreateBackBufferRTV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D) abstract;

	virtual Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11RenderTargetView> CreateRTV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateSRV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> CreateUAV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11DepthStencilView> CreateDSV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc) abstract;

	virtual Microsoft::WRL::ComPtr<ID3D11DepthStencilState> CreateDSS(D3D11_DEPTH_STENCIL_DESC* dssDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11RasterizerState> CreateRS(D3D11_RASTERIZER_DESC* rsDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11BlendState> CreateBS(D3D11_BLEND_DESC* bsDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11SamplerState> CreateSS(D3D11_SAMPLER_DESC* ssDesc) abstract;

	virtual Indexbuffer* CreateIndexBuffer(ParserData::Mesh* mesh) abstract;
	virtual Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mesh) abstract;
	virtual TextureBuffer* CreateTextureBuffer(std::string path) abstract;
};