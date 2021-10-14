#include "D3DDefine.h"
#include "D3DEngine.h"
#include "IRenderer.h"
#include "RenderTarget.h"
#include "Object.h"
#include "Shader.h"
#include "PostProcessing.h"
#include "ResourceManager.h"

PostProcessing::PostProcessing(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context)
	:IRenderer(device, context)
{
	HorzBlurShader = D3DEngine::GetInstance()->GetResourceManager()->GetShader("HorizonBlurShader");
	VertBlurShader = D3DEngine::GetInstance()->GetResourceManager()->GetShader("VerticalBlurShader");
}

PostProcessing::~PostProcessing()
{

}

void PostProcessing::OnResize(int width, int height)
{
	m_Width = width;
	m_Height = height;
}

void PostProcessing::Render(RenderTarget* vertRT, RenderTarget* horzRT)
{
	D3DEngine::GetInstance()->SetSolid();

	// HORIZONTAL blur pass.
	HorzBlurShader->SetComputeSRV("gInput", vertRT->GetSRV());
	HorzBlurShader->SetComputeUAV("gOutput", horzRT->GetUAV());
	HorzBlurShader->CSUpdate();

	// How many groups do we need to dispatch to cover a row of pixels, where each
	// group covers 256 pixels (the 256 is defined in the CS).
	UINT numGroupsX = (UINT)ceilf(m_Width / 256.0f);
	m_DeviceContext->Dispatch(numGroupsX, m_Height, 1);

	// Unbind the input texture from the CS for good housekeeping.
	// Unbind output from compute shader (we are going to use this output as an input in the next pass, 
	// and a resource cannot be both an output and input at the same time.
	Shader::ResetCSResources();

	// VERTICAL blur pass.
	VertBlurShader->SetComputeSRV("gInput", horzRT->GetSRV());
	VertBlurShader->SetComputeUAV("gOutput", vertRT->GetUAV());
	VertBlurShader->CSUpdate();

	// How many groups do we need to dispatch to cover a column of pixels, where each
	// group covers 256 pixels  (the 256 is defined in the CS).
	UINT numGroupsY = (UINT)ceilf(m_Height / 256.0f);
	m_DeviceContext->Dispatch(m_Width, numGroupsY, 1);

	// Unbind the input texture from the CS for good housekeeping.
	// Unbind output from compute shader (we are going to use this output as an input in the next pass, 
	// and a resource cannot be both an output and input at the same time.
	Shader::ResetCSResources();

	// Disable compute shader.
	m_DeviceContext->CSSetShader(0, 0, 0);
}
