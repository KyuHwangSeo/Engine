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
	// SpriteBatch�� ���� ���� ������Ʈ�� ������ �� �ִ�.
	// ������, �׳� Begin�� �ϸ� �������Ľǹ��� �ɼ��� D3D11_DEPTH_WRITE_MASK_ZERO�� �Ǵ� ��. DSS�� �ٷ��� �ʴ� ���ݿ��� ������ �� �� �ִ�.
	// �Ʒ�ó�� ���⿡ �ɼ��� ALL�� �־��༭ ZERO�� �Ǵ� ���� ���� ���� �ְ�, �ٸ� �� ������Ʈ�� �׸� �� ����������Ʈ�� ���� �� �� ���� �ִ�.
	// DX12���� ����������Ʈ���� �׷��� �������� �͵� ���ش� ����. ���� ���� �ȵ������..
	///m_pSpriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, nullptr, nullptr, m_DepthStencilState/*, m_RasterizerState*/);
	m_pSpriteBatch->Begin();
	m_pSpriteFont->DrawString(m_pSpriteBatch, text, XMFLOAT2((float)x, (float)y), SimpleMath::Vector4(color), 0.0f, XMFLOAT2(), scale);
	m_pSpriteBatch->End();
}