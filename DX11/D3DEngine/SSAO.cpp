#include "D3DDefine.h"
#include "D3DEngine.h"
#include "IRenderer.h"
#include "RenderTarget.h"
#include "Object.h"
#include "Shader.h"
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "SSAO.h"
#include "ResourceManager.h"


SSAO::SSAO(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context)
	:IRenderer(device, context)
{
	m_RandomVectorRT = new RenderTarget(device);
	m_AmbientRT = new RenderTarget(device);
	m_BlurRT = new RenderTarget(device);

	m_MainCamera = &Camera::g_MainCamera;

	BuildOffsetVectors();
	BuildRandomVectorTexture();

	m_TextureSpace = DXMatrix4X4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	m_QuadBuffer = ResourceManager::GetInstance()->GetVertexBuffer("SsaoQuad");

	m_SSAOShader = ResourceManager::GetInstance()->GetShader("SSAOShader");
	m_HorzBlurShader = ResourceManager::GetInstance()->GetShader("SSAOHorizonBlurShader");
	m_VertBlurShader = ResourceManager::GetInstance()->GetShader("SSAOVerticalBlurShader");
}

SSAO::~SSAO()
{

}

void SSAO::SsaoRender(ID3D11ShaderResourceView* normalDepthSRV1, ID3D11ShaderResourceView* normalDepthSRV2)
{
	// Bind the ambient map as the render target.  Observe that this pass does not bind 
	// a depth/stencil buffer--it does not need it, and without one, no depth test is
	// performed, which is what we want.
	D3DEngine::GetInstance()->SetSolid();

	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_DeviceContext->IASetVertexBuffers(0, 1, m_QuadBuffer->VB.GetAddressOf(), &m_QuadBuffer->Stride, &m_QuadBuffer->Offset);
	m_DeviceContext->IASetIndexBuffer(m_QuadBuffer->IB.Get(), DXGI_FORMAT_R32_UINT, 0);

	m_SsaoFrame.gViewToTexSpace = Camera::g_MainCamera->Proj() * m_TextureSpace;
	m_SSAOShader->SetVertexConstantBuffer(m_SsaoFrame);
	m_SSAOShader->VSUpdate();

	m_SSAOShader->SetPixelConstantBuffer(m_SsaoFrame);
	m_SSAOShader->SetSRV("gNormalDepthMap", normalDepthSRV1);
	m_SSAOShader->SetSRV("DepthSRV", normalDepthSRV2);
	m_SSAOShader->SetSRV("gRandomVecMap", m_RandomVectorRT->GetSRV());
	m_SSAOShader->PSUpdate();

	m_DeviceContext->DrawIndexed(m_QuadBuffer->IndexCount, 0, 0);

	Shader::ResetVSResources();
	Shader::ResetPSResources();
}

void SSAO::BlurRender(ID3D11ShaderResourceView* normalDepthSRV1, ID3D11ShaderResourceView* normalDepthSRV2, int blurCount)
{
	for (int i = 0; i < blurCount; i++)
	{
		// Horizon Blur..
		m_RenderTarget = m_BlurRT->GetRTV();
		m_DeviceContext->OMSetRenderTargets(1, &m_RenderTarget, 0);
		m_DeviceContext->ClearRenderTargetView(m_RenderTarget, reinterpret_cast<const float*>(&Colors::Red));

		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_DeviceContext->IASetVertexBuffers(0, 1, m_QuadBuffer->VB.GetAddressOf(), &m_QuadBuffer->Stride, &m_QuadBuffer->Offset);
		m_DeviceContext->IASetIndexBuffer(m_QuadBuffer->IB.Get(), DXGI_FORMAT_R32_UINT, 0);

		m_HorzBlurShader->VSUpdate();

		m_HorzBlurShader->SetPixelConstantBuffer(m_TexelWidth);
		m_HorzBlurShader->SetSRV("gNormalDepthMap", normalDepthSRV1);
		m_HorzBlurShader->SetSRV("DepthSRV", normalDepthSRV2);
		m_HorzBlurShader->SetSRV("gInputMap", m_AmbientRT->GetSRV());
		m_HorzBlurShader->PSUpdate();

		m_DeviceContext->DrawIndexed(m_QuadBuffer->IndexCount, 0, 0);

		Shader::ResetVSResources();
		Shader::ResetPSResources();

		// Vertical Blur..
		m_RenderTarget = m_AmbientRT->GetRTV();
		m_DeviceContext->OMSetRenderTargets(1, &m_RenderTarget, 0);
		m_DeviceContext->ClearRenderTargetView(m_RenderTarget, reinterpret_cast<const float*>(&Colors::Red));

		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_DeviceContext->IASetVertexBuffers(0, 1, m_QuadBuffer->VB.GetAddressOf(), &m_QuadBuffer->Stride, &m_QuadBuffer->Offset);
		m_DeviceContext->IASetIndexBuffer(m_QuadBuffer->IB.Get(), DXGI_FORMAT_R32_UINT, 0);

		m_VertBlurShader->VSUpdate();

		m_VertBlurShader->SetPixelConstantBuffer(m_TexelHeight);
		m_VertBlurShader->SetSRV("gNormalDepthMap", normalDepthSRV1);
		m_VertBlurShader->SetSRV("DepthSRV", normalDepthSRV2);
		m_VertBlurShader->SetSRV("gInputMap", m_BlurRT->GetSRV());

		m_VertBlurShader->PSUpdate();

		m_DeviceContext->DrawIndexed(m_QuadBuffer->IndexCount, 0, 0);

		Shader::ResetVSResources();
		Shader::ResetPSResources();
	}
}

void SSAO::OnResize(int width, int height)
{
	m_Width = width;
	m_Height = height;

	BuildFrustumFarCorners();

	// We render to ambient map at half the resolution.
	m_AmbientMapViewport.TopLeftX = 0.0f;
	m_AmbientMapViewport.TopLeftY = 0.0f;
	m_AmbientMapViewport.Width = width / 2.0f;
	m_AmbientMapViewport.Height = height / 2.0f;
	m_AmbientMapViewport.MinDepth = 0.0f;
	m_AmbientMapViewport.MaxDepth = 1.0f;

	// Blur Constant Buffer Data..
	m_TexelWidth.gTexelSize = 1.0f / m_AmbientMapViewport.Width;
	m_TexelHeight.gTexelSize = 1.0f / m_AmbientMapViewport.Height;

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = m_Width / 2;
	texDesc.Height = m_Height / 2; 
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R16_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> ambientTex0 = nullptr;
	HR(m_Device->CreateTexture2D(&texDesc, nullptr, &ambientTex0));
	m_AmbientRT->CreateSRV(ambientTex0.Get(), nullptr);
	m_AmbientRT->CreateRTV(ambientTex0.Get(), nullptr);

	ComPtr<ID3D11Texture2D> ambientTex1 = nullptr;
	HR(m_Device->CreateTexture2D(&texDesc, nullptr, &ambientTex1));
	m_BlurRT->CreateSRV(ambientTex1.Get(), nullptr);
	m_BlurRT->CreateRTV(ambientTex1.Get(), nullptr);

	// view saves a reference.
	RESET_COM(ambientTex0);
	RESET_COM(ambientTex1);
}

void SSAO::BuildFrustumFarCorners()
{
	if (Camera::g_MainCamera == nullptr) return;

	float aspect = (float)m_Width / (float)m_Height;
	float farZ = (*m_MainCamera)->GetFarZ();
	float fovy = (*m_MainCamera)->GetFovY();

	float halfHeight = farZ * tanf(0.5f * fovy);
	float halfWidth = aspect * halfHeight;

	m_FrustumFarCorner[0] = DXVector4(-halfWidth, -halfHeight, farZ, 0.0f);
	m_FrustumFarCorner[1] = DXVector4(-halfWidth, +halfHeight, farZ, 0.0f);
	m_FrustumFarCorner[2] = DXVector4(+halfWidth, +halfHeight, farZ, 0.0f);
	m_FrustumFarCorner[3] = DXVector4(+halfWidth, -halfHeight, farZ, 0.0f);

	// FrustumCorner Constant Buffer Data »ðÀÔ..
	for (int i = 0; i < 4; i++)
	{
		m_SsaoFrame.gFrustumCorners[i] = m_FrustumFarCorner[i];
	}
}

void SSAO::BuildOffsetVectors()
{
	// Start with 14 uniformly distributed vectors.  We choose the 8 corners of the cube
   // and the 6 center points along each cube face.  We always alternate the points on 
   // opposites sides of the cubes.  This way we still get the vectors spread out even
   // if we choose to use less than 14 samples.

   // 8 cube corners
	m_Offsets[0] = DXVector4(+1.0f, +1.0f, +1.0f, 0.0f);
	m_Offsets[1] = DXVector4(-1.0f, -1.0f, -1.0f, 0.0f);

	m_Offsets[2] = DXVector4(-1.0f, +1.0f, +1.0f, 0.0f);
	m_Offsets[3] = DXVector4(+1.0f, -1.0f, -1.0f, 0.0f);

	m_Offsets[4] = DXVector4(+1.0f, +1.0f, -1.0f, 0.0f);
	m_Offsets[5] = DXVector4(-1.0f, -1.0f, +1.0f, 0.0f);

	m_Offsets[6] = DXVector4(-1.0f, +1.0f, -1.0f, 0.0f);
	m_Offsets[7] = DXVector4(+1.0f, -1.0f, +1.0f, 0.0f);

	// 6 centers of cube faces
	m_Offsets[8] = DXVector4(-1.0f, 0.0f, 0.0f, 0.0f);
	m_Offsets[9] = DXVector4(+1.0f, 0.0f, 0.0f, 0.0f);

	m_Offsets[10] = DXVector4(0.0f, -1.0f, 0.0f, 0.0f);
	m_Offsets[11] = DXVector4(0.0f, +1.0f, 0.0f, 0.0f);

	m_Offsets[12] = DXVector4(0.0f, 0.0f, -1.0f, 0.0f);
	m_Offsets[13] = DXVector4(0.0f, 0.0f, +1.0f, 0.0f);

	for (int i = 0; i < 14; ++i)
	{
		// Create random lengths in [0.25, 1.0].
		float s = MathHelper::RandF(0.25f, 1.0f);

		m_Offsets[i] = s * XMVector4Normalize(m_Offsets[i]);

		// OffsetVector Constant Buffer Data »ðÀÔ..
		m_SsaoFrame.gOffsetVectors[i] = m_Offsets[i];
	}
}

void SSAO::BuildRandomVectorTexture()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = 256;
	texDesc.Height = 256;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.SysMemPitch = 256 * sizeof(XMCOLOR);

	XMCOLOR color[256 * 256];
	for (int i = 0; i < 256; ++i)
	{
		for (int j = 0; j < 256; ++j)
		{
			XMFLOAT3 v(MathHelper::RandF(), MathHelper::RandF(), MathHelper::RandF());

			color[i * 256 + j] = XMCOLOR(v.x, v.y, v.z, 0.0f);
		}
	}

	initData.pSysMem = color;

	ID3D11Texture2D* tex = nullptr;
	HR(m_Device->CreateTexture2D(&texDesc, &initData, &tex));
	m_RandomVectorRT->CreateSRV(tex, nullptr);

	// view saves a reference.
	RELEASE_COM(tex);
}
