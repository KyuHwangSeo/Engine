#include "D3DDefine.h"
#include "D3DEngine.h"
#include "IRenderer.h"
#include "RenderTarget.h"
#include "Object.h"
#include "Shader.h"
#include "MotionBlur.h"
#include "ResourceManager.h"
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "GameTimer.h"

MotionBlur::MotionBlur(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context)
	:IRenderer(device, context) 
{
	m_MotionBlurRT = new RenderTarget(device);
	m_VelocityRT = new RenderTarget(device);

	m_QuadBuffer = D3DEngine::GetInstance()->GetResourceManager()->GetVertexBuffer("Quad");

	m_VelocityShader = D3DEngine::GetInstance()->GetResourceManager()->GetShader("VelocityShader");
	m_MotionBlurShader = D3DEngine::GetInstance()->GetResourceManager()->GetShader("MotionBlurShader");
}

MotionBlur::~MotionBlur()
{

}

void MotionBlur::VelocityRender(ID3D11ShaderResourceView* depthSRV)
{
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX identity = XMMatrixIdentity();

	m_DeviceContext->IASetVertexBuffers(0, 1, m_QuadBuffer->VB.GetAddressOf(), &m_QuadBuffer->Stride, &m_QuadBuffer->Offset);
	m_DeviceContext->IASetIndexBuffer(m_QuadBuffer->IB.Get(), DXGI_FORMAT_R32_UINT, 0);

	DXMatrix4X4 proj = Camera::g_MainCamera->Proj();
	DXMatrix4X4 nowView = Camera::g_MainCamera->View();
	DXMatrix4X4 prevView = Camera::g_MainCamera->PrevView();

	m_ObjectData.gWorldViewProj = identity;
	m_VelocityShader->SetVertexConstantBuffer(m_ObjectData);

	// Vertex Shader Update
	m_VelocityShader->VSUpdate();

	m_ScreenData.gNowViewProj = nowView * proj;
	m_ScreenData.gPrevViewProj = prevView * proj;
	m_ScreenData.gDeltaTime = D3DEngine::GetInstance()->GetTimer()->DeltaTime();
	m_VelocityShader->SetPixelConstantBuffer(m_ScreenData);

	m_VelocityShader->SetSRV("gDepthMap", depthSRV);

	// Pixel Shader Update
	m_VelocityShader->PSUpdate();

	m_DeviceContext->DrawIndexed(m_QuadBuffer->IndexCount, 0, 0);

	Shader::ResetVSResources();
	Shader::ResetPSResources();
}

void MotionBlur::MotionBlurRender(ID3D11ShaderResourceView* screenSRV)
{
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX identity = XMMatrixIdentity();

	m_DeviceContext->IASetVertexBuffers(0, 1, m_QuadBuffer->VB.GetAddressOf(), &m_QuadBuffer->Stride, &m_QuadBuffer->Offset);
	m_DeviceContext->IASetIndexBuffer(m_QuadBuffer->IB.Get(), DXGI_FORMAT_R32_UINT, 0);

	m_ObjectData.gWorldViewProj = identity;
	m_MotionBlurShader->SetVertexConstantBuffer(m_ObjectData);

	// Vertex Shader Update
	m_MotionBlurShader->VSUpdate();

	m_MotionBlurShader->SetSRV("gOrigin", screenSRV);
	m_MotionBlurShader->SetSRV("gVelocity", m_VelocityRT->GetSRV());

	// Pixel Shader Update
	m_MotionBlurShader->PSUpdate();

	m_DeviceContext->DrawIndexed(m_QuadBuffer->IndexCount, 0, 0);

	Shader::ResetVSResources();
	Shader::ResetPSResources();
}

void MotionBlur::OnResize(int width, int height)
{
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

	ComPtr<ID3D11Texture2D> renderTargetTexture[2];
	m_Device->CreateTexture2D(&textureDescDiffuse, 0, &renderTargetTexture[0]);
	m_Device->CreateTexture2D(&textureDescDiffuse, 0, &renderTargetTexture[1]);

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = textureDescDiffuse.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	m_MotionBlurRT->CreateRTV(renderTargetTexture[0].Get(), &rtvDesc);
	m_VelocityRT->CreateRTV(renderTargetTexture[1].Get(), &rtvDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDescDiffuse.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	m_MotionBlurRT->CreateSRV(renderTargetTexture[0].Get(), &srvDesc);
	m_VelocityRT->CreateSRV(renderTargetTexture[1].Get(), &srvDesc);

	RESET_COM(renderTargetTexture[0]);
	RESET_COM(renderTargetTexture[1]);
}
