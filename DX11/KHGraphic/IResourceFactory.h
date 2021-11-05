#pragma once
#include <wrl.h>
#include <d3d11.h>

interface IGraphicResourceFactory
{
public:
	virtual Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateBackBuffer(UINT width, UINT height, DXGI_FORMAT format, UINT swapchainflags) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc) abstract;

	virtual Microsoft::WRL::ComPtr<ID3D11RenderTargetView> CreateRTV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, int count = 1) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateSRV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> CreateUAV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11DepthStencilView> CreateDSV(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11DepthStencilState> CreateDSS(D3D11_DEPTH_STENCIL_DESC* dssDesc) abstract;
};