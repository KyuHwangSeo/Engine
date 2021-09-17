#include "D3DDefine.h"
#include "IRenderer.h"
#include "Object.h"
#include "Shader.h"
#include "DebugViewPort.h"

DebugViewPort::DebugViewPort(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context)
	:IRenderer(device, context)
{
}

DebugViewPort::~DebugViewPort()
{

}

void DebugViewPort::OnResize(int width, int height)
{
	// DebugDeffered ViewPort ¼³Á¤
	m_DebugViewport[0].TopLeftX = 0;
	m_DebugViewport[0].TopLeftY = static_cast<float>((height / 4) * 3);
	m_DebugViewport[0].Width = static_cast<float>(width / 4);
	m_DebugViewport[0].Height = static_cast<float>(height / 4);
	m_DebugViewport[0].MinDepth = 0.0f;
	m_DebugViewport[0].MaxDepth = 1.0f;

	m_DebugViewport[1].TopLeftX = static_cast<float>(width / 4);
	m_DebugViewport[1].TopLeftY = static_cast<float>((height / 4) * 3);
	m_DebugViewport[1].Width = static_cast<float>(width / 4);
	m_DebugViewport[1].Height = static_cast<float>(height / 4);
	m_DebugViewport[1].MinDepth = 0.0f;
	m_DebugViewport[1].MaxDepth = 1.0f;

	m_DebugViewport[2].TopLeftX = static_cast<float>((width / 4) * 2);
	m_DebugViewport[2].TopLeftY = static_cast<float>((height / 4) * 3);
	m_DebugViewport[2].Width = static_cast<float>(width / 4);
	m_DebugViewport[2].Height = static_cast<float>(height / 4);
	m_DebugViewport[2].MinDepth = 0.0f;
	m_DebugViewport[2].MaxDepth = 1.0f;

	m_DebugViewport[3].TopLeftX = static_cast<float>((width / 4) * 3);
	m_DebugViewport[3].TopLeftY = static_cast<float>((height / 4) * 3);
	m_DebugViewport[3].Width = static_cast<float>(width / 4);
	m_DebugViewport[3].Height = static_cast<float>(height / 4);
	m_DebugViewport[3].MinDepth = 0.0f;
	m_DebugViewport[3].MaxDepth = 1.0f;

	m_DebugViewport[4].TopLeftX = static_cast<float>((width / 4) * 3);
	m_DebugViewport[4].TopLeftY = static_cast<float>((height / 4) * 2);
	m_DebugViewport[4].Width = static_cast<float>(width / 4);
	m_DebugViewport[4].Height = static_cast<float>(height / 4);
	m_DebugViewport[4].MinDepth = 0.0f;
	m_DebugViewport[4].MaxDepth = 1.0f;

	m_DebugViewport[5].TopLeftX = static_cast<float>((width / 4) * 3);
	m_DebugViewport[5].TopLeftY = static_cast<float>((height / 4));
	m_DebugViewport[5].Width = static_cast<float>(width / 4);
	m_DebugViewport[5].Height = static_cast<float>(height / 4);
	m_DebugViewport[5].MinDepth = 0.0f;
	m_DebugViewport[5].MaxDepth = 1.0f;

	m_DebugViewport[6].TopLeftX = static_cast<float>((width / 4) * 3);
	m_DebugViewport[6].TopLeftY = 0;
	m_DebugViewport[6].Width = static_cast<float>(width / 4);
	m_DebugViewport[6].Height = static_cast<float>(height / 4);
	m_DebugViewport[6].MinDepth = 0.0f;
	m_DebugViewport[6].MaxDepth = 1.0f;
}

void DebugViewPort::SetViewPort(int count, int viewportNum)
{
	m_DeviceContext->RSSetViewports(count, &m_DebugViewport[viewportNum]);
}
