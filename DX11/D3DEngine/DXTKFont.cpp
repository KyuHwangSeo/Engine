#include "D3DEngineDLL.h"

#include "DXTKFont.h"
#include <tchar.h>

DXTKFont::DXTKFont()
	: m_pSpriteBatch(nullptr), m_pSpriteFont(m_pSpriteFont)
{

}

DXTKFont::~DXTKFont()
{
	SAFE_DELETE(m_pSpriteFont);
	SAFE_DELETE(m_pSpriteBatch);
}

void DXTKFont::Create(ID3D11Device* pDevice)
{
	ID3D11DeviceContext* pDC = nullptr;
	pDevice->GetImmediateContext(&pDC);

	m_pSpriteBatch = new SpriteBatch(pDC);

	TCHAR* _fileName = (TCHAR*)L"../Resource/Font/gulim9k.spritefont";

	m_pSpriteFont = new SpriteFont(pDevice, _fileName);
	m_pSpriteFont->SetLineSpacing(14.0f);
	//m_pSpriteFont->SetDefaultCharacter(' ');

	RELEASE_COM(pDC);
}

void DXTKFont::DrawTextColorE(int x, int y, float scale, XMFLOAT4 color, TCHAR* text, ...)
{
	TCHAR _buffer[1024] = L"";
	va_list vl;
	va_start(vl, text);
	_vstprintf(_buffer, 1024, text, vl);
	va_end(vl);

	m_pSpriteBatch->Begin();
	m_pSpriteFont->DrawString(m_pSpriteBatch, _buffer, XMFLOAT2((float)x, (float)y), SimpleMath::Vector4(color), 0.0f, XMFLOAT2(), scale);
	m_pSpriteBatch->End();
}

void DXTKFont::DrawTextColorD(int x, int y, float scale, XMFLOAT4 color, TCHAR* text)
{
	// SpriteBatch에 여러 렌더 스테이트를 지정할 수 있다.
	// 문제는, 그냥 Begin만 하면 뎁스스탠실버퍼 옵션이 D3D11_DEPTH_WRITE_MASK_ZERO가 되는 듯. DSS를 다루지 않는 포반에는 문제가 될 수 있다.
	// 아래처럼 여기에 옵션을 ALL로 넣어줘서 ZERO가 되는 것을 막을 수도 있고, 다른 쪽 오브젝트를 그릴 때 렌더스테이트를 지정 해 줄 수도 있다.
	// DX12에서 렌더스테이트등을 그렇게 가져가는 것도 이해는 간다. 별로 맘에 안들었겠지..
	///m_pSpriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, nullptr, nullptr, m_DepthStencilState/*, m_RasterizerState*/);
	m_pSpriteBatch->Begin();
	m_pSpriteFont->DrawString(m_pSpriteBatch, text, XMFLOAT2((float)x, (float)y), SimpleMath::Vector4(color), 0.0f, XMFLOAT2(), scale);
	m_pSpriteBatch->End();
}