#pragma once
/// <summary>
/// DXTK�� �̿��� �ؽ�Ʈ ���
/// </summary>
#include "SpriteFont.h"
#include "SpriteBatch.h"

class DXTKFont
{
public:
	DXTKFont();
	~DXTKFont();

	void Create(ID3D11Device* pDevice);

	void DrawTextColorE(int x, int y, float scale, DXVector4 color, TCHAR* text, ...);
	void DrawTextColorD(int x, int y, float scale, DXVector4 color, TCHAR* text);

private:
	SpriteBatch* m_pSpriteBatch;
	SpriteFont* m_pSpriteFont;
};