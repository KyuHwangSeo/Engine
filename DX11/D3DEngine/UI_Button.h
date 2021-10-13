#pragma once
///
/// 2021/08/23 19:31
/// SeoKyuHwang
///
/// UI_Button Class
///
/// - 2D Image를 이용한 버튼 UI Class

class UI_Button : public UI_Image
{
public:
	UI_Button();
	~UI_Button();

public:
	void Start();
	void Update(float dTime);

	DLL_DECLSPEC void SetOnMouseTexture(ComPtr<ID3D11ShaderResourceView> tex) { m_OnMouseTex = tex; };
	DLL_DECLSPEC void SetOnClickTexture(ComPtr<ID3D11ShaderResourceView> tex) { m_OnClickTex = tex; }

	DLL_DECLSPEC bool GetIsOnClick() { return m_IsOnClick; }
	DLL_DECLSPEC bool GetIsOnMouse() { return m_IsOnMouse; }

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

