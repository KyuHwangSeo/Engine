#pragma once
/// <summary>
/// DXTK를 이용한 텍스트 출력
/// 
/// 완성형밖에 안된다는데..
/// DirectWrite버전도 제공해야겠다.
/// 
/// 2021.03.04 LeHideOffice
/// </summary>
#include "SpriteFont.h"
#include "SpriteBatch.h"

class DXTKFont
{
public:
	DXTKFont();
	~DXTKFont();

	void Create(ID3D11Device* pDevice);

	void DrawTextColorE(int x, int y, float scale, XMFLOAT4 color, TCHAR* text, ...);
	void DrawTextColorD(int x, int y, float scale, XMFLOAT4 color, TCHAR* text);

private:
	SpriteBatch* m_pSpriteBatch;
	SpriteFont* m_pSpriteFont;
};