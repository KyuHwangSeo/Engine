#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "UI_Image.h"
#include "Shader.h"
#include "Material.h"
#include "Renderer.h"
#include "CanvasRenderer.h"
#include "UI_Button.h"

UI_Button::UI_Button()
	:UI_Image(), m_IsOnClick(false)
{
	m_Input = D3DEngine::GetInstance()->GetInput();
	m_MousePos = m_Input->GetMousePos();
}

UI_Button::~UI_Button()
{

}

void UI_Button::Start()
{
	UI_Image::Start();
	m_Material = m_GameObject->GetComponent<CanvasRenderer>()->GetMaterial();
	m_OriginalTex = m_Material->GetDiffuseMap();
}

void UI_Button::Update(float dTime)
{
	m_IsOnMouse = false;
	m_IsOnClick = false;

	DXVector3 pos = m_Transform->m_Pos;
	m_OriginPoint = DXVector2(pos.x, pos.y);

	DXVector3 scale = m_Transform->m_Scale;
	m_Size = DXVector2(scale.x, scale.y);

	if (m_MousePos->x > m_OriginPoint.x && m_MousePos->x < m_OriginPoint.x + m_Size.x &&
		m_MousePos->y > m_OriginPoint.y && m_MousePos->y < m_OriginPoint.y + m_Size.y)
	{
		m_IsOnMouse = true;
		if (m_Input->IsKeyUP(VK_LBUTTON))
		{
			m_IsOnClick = true;
		}
	}

	if (m_IsOnClick == false)
	{
		if (m_IsOnMouse)
			m_Material->SetDiffuseMap(m_OnMouseTex.Get());
		else
			m_Material->SetDiffuseMap(m_OriginalTex.Get());
	}
	else
	{
		m_Material->SetDiffuseMap(m_OnClickTex.Get());
	}
}
