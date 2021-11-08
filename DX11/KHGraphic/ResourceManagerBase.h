#pragma once
#include "EnumDefine.h"

///
/// 2021/11/07 23:02
/// SeoKyuHwang
///
/// GraphicResourceManager Interface Class
///
/// - 葛电 GraphicResource甫 包府窍绰 Class
/// - GraphicResourceFactory俊辑 积己等 Resource 包府

class ViewPort;
class RenderTarget;
class DepthStencilView;

interface IGraphicResourceManager
{
public:
	virtual void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain) abstract;
	virtual void OnResize(int width, int height) abstract;

public:
	virtual RenderTarget* GetRenderTarget(eRenderTarget state) abstract;
	virtual RenderTarget* GetMainRenderTarget() abstract;

	virtual DepthStencilView* GetDepthStencilView(eDepthStencilView state) abstract;

	virtual ID3D11BlendState* GetBlendState(eBlendState state) abstract;
	virtual ID3D11RasterizerState* GetRasterizerState(eRasterizerState state) abstract;
	virtual ID3D11DepthStencilState* GetDepthStencilState(eDepthStencilState state) abstract;

	virtual D3D11_VIEWPORT* GetViewPort(eViewPort state) abstract;

};