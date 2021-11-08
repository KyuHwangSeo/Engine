#pragma once

///
/// 2021/11/08 2:22
/// SeoKyuHwang
///
/// RenderBase Class
///
/// - ���� Rendering Class���� �ش� Rendering�� �´� GraphicResource ������ ����
///   ���� Manager & DeviceContext�� �������� �ΰ� ���

interface IShaderManager;
interface IGraphicResourceManager;
interface IGraphicResourceFactory;

class RenderBase
{
public:
	RenderBase() = default;
	virtual ~RenderBase() = default;

public:
	virtual void Initialize(int width, int height) abstract;
	virtual void OnResize(int width, int height) abstract;

public:
	static void Initialize(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, IGraphicResourceFactory* factory, IGraphicResourceManager* resourceManager, IShaderManager* shaderManager);
	static void Reset();

protected:
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> g_Context;
	static IGraphicResourceFactory* g_Factory;
	static IGraphicResourceManager* g_Resource;
	static IShaderManager* g_Shader;
};