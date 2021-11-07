#pragma once
#include "ResourceFactoryBase.h"

class D3D11Graphic;
class ShaderManager;
class GraphicResourceManager;
class GraphicResourceFactory : public IGraphicResourceFactory
{
public:
	GraphicResourceFactory(D3D11Graphic* graphic);
	~GraphicResourceFactory();

public:
	void Initialize() override;
	void Release() override;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateBackBuffer(UINT width, UINT height) override;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> CreateBackBufferRTV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D) override;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc) override;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> CreateRTV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc) override;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateSRV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) override;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> CreateUAV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) override;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> CreateDSV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc) override;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> CreateDSS(D3D11_DEPTH_STENCIL_DESC* dssDesc) override;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> CreateRS(D3D11_RASTERIZER_DESC* rsDesc) override;
	Microsoft::WRL::ComPtr<ID3D11BlendState> CreateBS(D3D11_BLEND_DESC* bsDesc) override;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> CreateSS(D3D11_SAMPLER_DESC* ssDesc) override;

	D3D11_VIEWPORT* CreateViewPort(float width, float height, float width_ratio = 1.0f, float height_ratio = 1.0f) override;

	Indexbuffer* CreateIndexBuffer(ParserData::Mesh* mesh) override;
	Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mesh) override;
	TextureBuffer* CreateTextureBuffer(std::string path) override;

public:
	IShaderManager* GetShaderManager();
	IGraphicResourceManager* GetResourceManager();

private:
	void CreateDepthStencilState();
	void CreateRasterizerState();
	void CreateSamplerState();
	void CreateBlendState();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

	ShaderManager* m_ShaderManager;
	GraphicResourceManager* m_ResourceManager;
};

