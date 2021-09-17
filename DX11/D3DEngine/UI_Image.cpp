#include "D3DDefine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "UI_Image.h"

void UI_Image::Start()
{
	m_Transform = m_GameObject->GetTransform();
}

void UI_Image::SetSize(float width, float height)
{
	m_Transform->SetScale(width, height, 1.0f);
}

void UI_Image::ChangeSize(float width, float height)
{
	m_Transform->ChangeScale(width, height, 1.0f);
}

void UI_Image::SetPosition(float width, float height)
{
	m_Transform->SetPosition(width, height, 1.0f);
}

void UI_Image::MovePosition(float width, float height)
{
	m_Transform->MoveWorld(width, height, 1.0f);
}

void UI_Image::Release()
{
	m_Transform = nullptr;
}
