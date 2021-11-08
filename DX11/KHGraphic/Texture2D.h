#pragma once

class Texture2D
{
public:
	virtual ID3D11Texture2D* GetTexture2D() abstract;
	virtual ID3D11Texture2D** GetAddressTexture2D() abstract;
	virtual D3D11_TEXTURE2D_DESC GetTextureDesc() abstract;
};