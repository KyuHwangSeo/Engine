#include "D3DDefine.h"
#include "D3DEngine.h"
#include "IRenderer.h"
#include "RenderTarget.h"
#include "Object.h"
#include "Shader.h"
#include "Forward.h"
#include "ResourceManager.h"

Forward::Forward(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context, ComPtr<IDXGISwapChain> swapchain)
	:IRenderer(device, context), m_SwapChain(swapchain)
{
	m_ForwardRT = new RenderTarget(device);
	m_AllHorzBlurRT = new RenderTarget(device);
	m_AllVertBlurRT = new RenderTarget(device);

	m_QuadBuffer = D3DEngine::GetInstance()->GetResourceManager()->GetVertexBuffer("Quad");

	m_FinalShader = D3DEngine::GetInstance()->GetResourceManager()->GetShader("FinalShader");
}

Forward::~Forward()
{
	m_SwapChain.Reset();
}

void Forward::BeginRender(ID3D11DepthStencilView* dsv)
{
	// RenderTarget 설정..
	m_DeviceContext->OMSetRenderTargets(6, &m_ForwardRTV, dsv);

	// RenderTarget 초기화..
	m_DeviceContext->ClearRenderTargetView(m_ForwardRTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
}

void Forward::OnResize(int width, int height)
{
	m_Width = width;
	m_Height = height;

	m_ForwardRT->Reset();

	// Swap Chain, Render Target View Resize 및 재생성..
	HR(m_SwapChain->ResizeBuffers(1, (UINT)width, (UINT)height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	/// 스왑 체인 백 버퍼중 하나에 대한 포인터를 가져옵니다..
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;

	HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));

	/// 백 버퍼 포인터를 사용하여 RenderTargetView를 만듭니다.
	m_ForwardRT->CreateRTV(backBuffer.Get(), nullptr);
	m_ForwardRT->CreateSRV(backBuffer.Get(), nullptr);
	m_ForwardRTV = m_ForwardRT->GetRTV();

	/// 더 이상 필요하지 않은 백버퍼 포인터 해제..
	RESET_COM(backBuffer);

	// Full Screen Blur 생성..
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> offscreenTex = nullptr;
	HR(m_Device->CreateTexture2D(&texDesc, 0, offscreenTex.GetAddressOf()));

	// Post 전용 RenderTarget 생성..
	m_AllHorzBlurRT->CreateRenderTarget(offscreenTex.Get(), nullptr, nullptr, nullptr);

	// View saves a reference to the texture so we can release our reference.
	RESET_COM(offscreenTex);

	D3D11_TEXTURE2D_DESC blurredTexDesc;
	blurredTexDesc.Width = width;
	blurredTexDesc.Height = height;
	blurredTexDesc.MipLevels = 1;
	blurredTexDesc.ArraySize = 1;
	blurredTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	blurredTexDesc.SampleDesc.Count = 1;
	blurredTexDesc.SampleDesc.Quality = 0;
	blurredTexDesc.Usage = D3D11_USAGE_DEFAULT;
	blurredTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	blurredTexDesc.CPUAccessFlags = 0;
	blurredTexDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> blurredTex = nullptr;
	HR(m_Device->CreateTexture2D(&blurredTexDesc, 0, &blurredTex));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;

	// Blur 전용 RenderTarget 생성..
	m_AllVertBlurRT->CreateSRV(blurredTex.Get(), &srvDesc);
	m_AllVertBlurRT->CreateUAV(blurredTex.Get(), &uavDesc);

	// Views save a reference to the texture so we can release our reference.
	RESET_COM(blurredTex);

	/// 렌더링할 ViewPort 변환 설정..
	m_ForwardViewport.TopLeftX = 0;
	m_ForwardViewport.TopLeftY = 0;
	m_ForwardViewport.Width = static_cast<float>(width);
	m_ForwardViewport.Height = static_cast<float>(height);
	m_ForwardViewport.MinDepth = 0.0f;
	m_ForwardViewport.MaxDepth = 1.0f;
}

void Forward::FinalRender(ID3D11ShaderResourceView* srv)
{
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX identity = XMMatrixIdentity();

	m_DeviceContext->IASetVertexBuffers(0, 1, m_QuadBuffer->VB.GetAddressOf(), &m_QuadBuffer->Stride, &m_QuadBuffer->Offset);
	m_DeviceContext->IASetIndexBuffer(m_QuadBuffer->IB.Get(), DXGI_FORMAT_R32_UINT, 0);

	m_ObjectData.gWorldViewProj = identity;
	m_FinalShader->SetVertexConstantBuffer(m_ObjectData);

	// Vertex Shader Update
	m_FinalShader->VSUpdate();

	m_FinalShader->SetDiffuseMap(srv);

	// Pixel Shader Update
	m_FinalShader->PSUpdate();

	m_DeviceContext->DrawIndexed(m_QuadBuffer->IndexCount, 0, 0);

	Shader::ResetVSResources();
	Shader::ResetPSResources();
}
