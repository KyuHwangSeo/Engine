#pragma once
///
/// 2021/08/23 19:30
/// SeoKyuHwang
///
/// UI_Image Class
///
/// - UI Base Class
/// - 간단한 2D Image 위치, 사이즈 조절 기능

class UI_Image : public Component
{
public:
	UI_Image() = default;
	virtual ~UI_Image() = default;

public:
	void Start();

	void SetSize(float width, float height);
	void ChangeSize(float width, float height);

	void SetPosition(float width, float height);
	void MovePosition(float width, float height);

	void Release();

protected:
	Transform* m_Transform;

	DXVector2 m_OriginPoint;
	DXVector2 m_Size;
};

