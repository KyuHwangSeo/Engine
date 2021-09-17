#include "D3DDefine.h"
#include "D3DEngine.h"
#include "IRenderer.h"
#include "RenderTarget.h"
#include "Object.h"
#include "Shader.h"
#include "Component.h"
#include "Transform.h"
#include "Light.h"
#include "Camera.h"
#include "Deferred.h"
#include "ResourceManager.h"

Deferred::Deferred(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context)
	:IRenderer(device, context)
{
	m_AlbedoRT = new RenderTarget(device);
	m_NormalRT = new RenderTarget(device);
	m_PositionRT = new RenderTarget(device);
	m_LightRT = new RenderTarget(device);
	m_ShadowRT = new RenderTarget(device);
	m_NormalDepthRT = new RenderTarget(device);
	m_DepthRT = new RenderTarget(device);

	m_DeferredRTV.resize(7);
	m_DeferredSRV.resize(7);

	m_QuadBuffer = ResourceManager::GetInstance()->GetVertexBuffer("Quad");

	m_LightShader = ResourceManager::GetInstance()->GetShader("LightShader");
}

Deferred::~Deferred()
{

}

void Deferred::BeginRender(ID3D11DepthStencilView* dsv)
{
	// RenderTarget 설정..
	m_DeviceContext->OMSetRenderTargets(7, &m_DeferredRTV[0], dsv);

	// RenderTarget 초기화..
	m_DeviceContext->ClearRenderTargetView(m_DeferredRTV[0], reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	m_DeviceContext->ClearRenderTargetView(m_DeferredRTV[1], reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	m_DeviceContext->ClearRenderTargetView(m_DeferredRTV[2], reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	m_DeviceContext->ClearRenderTargetView(m_DeferredRTV[3], reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	m_DeviceContext->ClearRenderTargetView(m_DeferredRTV[4], reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	m_DeviceContext->ClearRenderTargetView(m_DeferredRTV[5], reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	m_DeviceContext->ClearRenderTargetView(m_DeferredRTV[6], reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
}

void Deferred::LightRender(ID3D11ShaderResourceView* ssaoSRV)
{
	D3DEngine::GetInstance()->SetSolid();

	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_DeviceContext->IASetVertexBuffers(0, 1, m_QuadBuffer->VB.GetAddressOf(), &m_QuadBuffer->Stride, &m_QuadBuffer->Offset);
	m_DeviceContext->IASetIndexBuffer(m_QuadBuffer->IB.Get(), DXGI_FORMAT_R32_UINT, 0);
	
	// Vertex Shader Update
	m_LightShader->VSUpdate();

	m_LightShader->SetSRV("AlbedoSRV", m_AlbedoRT->GetSRV());
	m_LightShader->SetSRV("NormalSRV", m_NormalRT->GetSRV());
	m_LightShader->SetSRV("PositionSRV", m_PositionRT->GetSRV());
	m_LightShader->SetSRV("LightSRV", m_LightRT->GetSRV());
	m_LightShader->SetSRV("ShadowSRV", m_ShadowRT->GetSRV());
	m_LightShader->SetSRV("DepthSRV", m_DepthRT->GetSRV());
	m_LightShader->SetSRV("SsaoSRV", ssaoSRV);

	// Pixel Shader Update
	m_LightShader->PSUpdate();

	m_DeviceContext->DrawIndexed(m_QuadBuffer->IndexCount, 0, 0);

	Shader::ResetVSResources();
	Shader::ResetPSResources();
}

void Deferred::OnResize(int width, int height)
{
	m_Width = width;
	m_Height = height;

	D3D11_TEXTURE2D_DESC textureDescPosNorm;
	textureDescPosNorm.Width = width;
	textureDescPosNorm.Height = height;
	textureDescPosNorm.MipLevels = 1;
	textureDescPosNorm.ArraySize = 1;
	textureDescPosNorm.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDescPosNorm.SampleDesc.Count = 1;
	textureDescPosNorm.SampleDesc.Quality = 0;
	textureDescPosNorm.Usage = D3D11_USAGE_DEFAULT;
	textureDescPosNorm.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDescPosNorm.CPUAccessFlags = 0;
	textureDescPosNorm.MiscFlags = 0;

	D3D11_TEXTURE2D_DESC textureDescDiffuse;
	textureDescDiffuse.Width = width;
	textureDescDiffuse.Height = height;
	textureDescDiffuse.MipLevels = 1;
	textureDescDiffuse.ArraySize = 1;
	textureDescDiffuse.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDescDiffuse.SampleDesc.Count = 1;
	textureDescDiffuse.SampleDesc.Quality = 0;
	textureDescDiffuse.Usage = D3D11_USAGE_DEFAULT;
	textureDescDiffuse.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDescDiffuse.CPUAccessFlags = 0;
	textureDescDiffuse.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> renderTargetTextureArray[7];
	m_Device->CreateTexture2D(&textureDescDiffuse, 0, &renderTargetTextureArray[0]);
	m_Device->CreateTexture2D(&textureDescPosNorm, 0, &renderTargetTextureArray[1]);
	m_Device->CreateTexture2D(&textureDescPosNorm, 0, &renderTargetTextureArray[2]);
	m_Device->CreateTexture2D(&textureDescPosNorm, 0, &renderTargetTextureArray[3]);
	m_Device->CreateTexture2D(&textureDescPosNorm, 0, &renderTargetTextureArray[4]);
	m_Device->CreateTexture2D(&textureDescPosNorm, 0, &renderTargetTextureArray[5]);
	m_Device->CreateTexture2D(&textureDescPosNorm, 0, &renderTargetTextureArray[6]);

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = textureDescDiffuse.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	//Create the render target arrays
	m_AlbedoRT->CreateRTV(renderTargetTextureArray[0].Get(), &rtvDesc);

	rtvDesc.Format = textureDescPosNorm.Format;
	m_NormalRT->CreateRTV(renderTargetTextureArray[1].Get(), &rtvDesc);
	m_PositionRT->CreateRTV(renderTargetTextureArray[2].Get(), &rtvDesc);
	m_LightRT->CreateRTV(renderTargetTextureArray[3].Get(), &rtvDesc);
	m_ShadowRT->CreateRTV(renderTargetTextureArray[4].Get(), &rtvDesc);
	m_NormalDepthRT->CreateRTV(renderTargetTextureArray[5].Get(), &rtvDesc);
	m_DepthRT->CreateRTV(renderTargetTextureArray[6].Get(), &rtvDesc);

	// RenderTarget List 설정..
	m_DeferredRTV[0] = m_AlbedoRT->GetRTV();
	m_DeferredRTV[1] = m_NormalRT->GetRTV();
	m_DeferredRTV[2] = m_PositionRT->GetRTV();
	m_DeferredRTV[3] = m_LightRT->GetRTV();
	m_DeferredRTV[4] = m_ShadowRT->GetRTV();
	m_DeferredRTV[5] = m_NormalDepthRT->GetRTV();
	m_DeferredRTV[6] = m_DepthRT->GetRTV();

	//Creste SRV desc
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDescDiffuse.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	m_AlbedoRT->CreateSRV(renderTargetTextureArray[0].Get(), &srvDesc);

	srvDesc.Format = textureDescPosNorm.Format;
	m_NormalRT->CreateSRV(renderTargetTextureArray[1].Get(), &srvDesc);
	m_PositionRT->CreateSRV(renderTargetTextureArray[2].Get(), &srvDesc);
	m_LightRT->CreateSRV(renderTargetTextureArray[3].Get(), &srvDesc);
	m_ShadowRT->CreateSRV(renderTargetTextureArray[4].Get(), &srvDesc);
	m_NormalDepthRT->CreateSRV(renderTargetTextureArray[5].Get(), &srvDesc);
	m_DepthRT->CreateSRV(renderTargetTextureArray[6].Get(), &srvDesc);

	// RenderTarget List 설정..
	m_DeferredSRV[0] = m_AlbedoRT->GetSRV();
	m_DeferredSRV[1] = m_NormalRT->GetSRV();
	m_DeferredSRV[2] = m_PositionRT->GetSRV();
	m_DeferredSRV[3] = m_LightRT->GetSRV();
	m_DeferredSRV[4] = m_ShadowRT->GetSRV();
	m_DeferredSRV[5] = m_NormalDepthRT->GetSRV();
	m_DeferredSRV[6] = m_DepthRT->GetSRV();

	RESET_COM(renderTargetTextureArray[0]);
	RESET_COM(renderTargetTextureArray[1]);
	RESET_COM(renderTargetTextureArray[2]);
	RESET_COM(renderTargetTextureArray[3]);
	RESET_COM(renderTargetTextureArray[4]);
	RESET_COM(renderTargetTextureArray[5]);
	RESET_COM(renderTargetTextureArray[6]);
}