#pragma once
///
/// 2021/08/23 19:31
/// SeoKyuHwang
///
/// UI_Button Class
///
/// - 2D Image�� �̿��� ��ư UI Class

class UI_Button : public UI_Image
{
public:
	UI_Button();
	~UI_Button();

public:
	void Start();
	void Update(float dTime);

	KH_API void SetOnMouseTexture(ComPtr<ID3D11ShaderResourceView> tex) { m_OnMouseTex = tex; };
	KH_API void SetOnClickTexture(ComPtr<ID3D11ShaderResourceView> tex) { m_OnClickTex = tex; }

	KH_API bool GetIsOnClick() { return m_IsOnClick; }
	KH_API bool GetIsOnMouse() { return m_IsOnMouse; }

public:
	Material* m_Material;
	ComPtr<ID3D11ShaderResourceView> m_OnMouseTex;
	ComPtr<ID3D11ShaderResourceView> m_OnClickTex;
	ComPtr<ID3D11ShaderResourceView> m_OriginalTex;

	DXInput* m_Input;
	POINT* m_MousePos;

	bool m_IsOnClick;
	bool m_IsOnMouse;
};

