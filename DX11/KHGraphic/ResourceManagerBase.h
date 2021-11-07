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

interface IGraphicResourceManager
{
public:
	virtual void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain) abstract;
	virtual void OnResize(int width, int height) abstract;

public:
	virtual Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GetDepthStencilState(eDSState state) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11RasterizerState> GetRasterizerState(eRState state) abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11BlendState> GetBlendState(eBState state) abstract;
};