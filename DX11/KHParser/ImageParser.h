#pragma once
#include "ParserDLL.h"
#include "../ShareData/ParserData.h"

class ImageParser
{
public:
	ImageParser() = default;
	virtual ~ImageParser() = default;

public:
	enum class Type
	{
		FLOAT,
		CHAR
	};

protected:
	std::string m_TextureRoute;

protected:
	float* LoadImage_Float(const char* filename, int* x, int* y, int* comp, int req_comp);
	unsigned char* LoadImage_Char(const char* filename, int* x, int* y, int* comp, int req_comp);

public:
	static PARSER_DLL ImageParser* Create(Type type);

public:
	virtual PARSER_DLL void Initialize() abstract;
	virtual PARSER_DLL void SetTextureRoute(std::string texRoute) abstract;
	virtual PARSER_DLL ParserData::ImageData LoadImagePixel(const char* fileName, unsigned int channels) abstract;

	virtual PARSER_DLL DirectX::SimpleMath::Vector4 GetPixelColor(ParserData::ImageData& img, int x, int y) abstract;
	virtual PARSER_DLL DirectX::SimpleMath::Vector4 GetPixelColor(ParserData::ImageData& img, float x, float y) abstract;
};