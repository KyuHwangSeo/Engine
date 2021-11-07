#include "DirectDefine.h"
#include "ViewPort.h"

ViewPort::ViewPort(float width, float height, float width_ratio, float height_ratio)
	:m_Width_Ratio(width_ratio), m_Height_Ratio(height_ratio)
{
	m_ViewPort = new D3D11_VIEWPORT();
	m_ViewPort->Width = width * width_ratio;
	m_ViewPort->Height = height * height_ratio;
	m_ViewPort->TopLeftX = 0.0f;
	m_ViewPort->TopLeftY = 0.0f;
	m_ViewPort->MinDepth = 0.0f;
	m_ViewPort->MaxDepth = 1.0f;
}

ViewPort::~ViewPort()
{
	SAFE_DELETE(m_ViewPort);
}

void ViewPort::OnResize(int width, int height)
{
	m_ViewPort->Width = width * m_Width_Ratio;
	m_ViewPort->Height = height * m_Height_Ratio;
}

D3D11_VIEWPORT* ViewPort::GetViewPort()
{
	return m_ViewPort;
}
