#include <string>

#include "ParserDLL.h"
#include "KHMath.h"

#include "ImageParser.h"

#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb_image.h"

PARSER_DLL void ImageParser::Initialize(std::string texRoute)
{
	m_TextureRoute = texRoute;
}

PARSER_DLL ImageDataF ImageParser::LoadImagePixelByFloat(const char* fileName, unsigned int channels)
{
	std::string filePath = m_TextureRoute + fileName;

	ImageDataF imgData;
	imgData.imgColor = stbi_loadf(filePath.c_str(), &imgData.width, &imgData.height, nullptr, channels);
	
	return imgData;
}

PARSER_DLL ImageDataI ImageParser::LoadImagePixelByInt(const char* fileName, unsigned int channels)
{
	std::string filePath = m_TextureRoute + fileName;

	ImageDataI imgData;
	imgData.imgColor = stbi_load(filePath.c_str(), &imgData.width, &imgData.height, nullptr, channels);

	return imgData;
}

PARSER_DLL DXVector4 ImageParser::GetPixelColorF(ImageDataF& img, int x, int y)
{
	const float* p = img.imgColor + (4 * (y * img.width + x));

	return DXVector4(p[0], p[1], p[2], p[3]);
}

PARSER_DLL DXVector4 ImageParser::GetPixelColorF(ImageDataF& img, float x, float y)
{
	const float* p = img.imgColor + (4 * ((int)y * img.width + (int)x));

	return DXVector4(p[0], p[1], p[2], p[3]);
}

PARSER_DLL DXVector4 ImageParser::GetPixelColorI(ImageDataI& img, int x, int y)
{
	const unsigned char* p = img.imgColor + (4 * (y * img.width + x));

	return DXVector4(p[0], p[1], p[2], p[3]);
}

PARSER_DLL DXVector4 ImageParser::GetPixelColorI(ImageDataI& img, float x, float y)
{
	const unsigned char* p = img.imgColor + (4 * ((int)y * img.width + (int)x));

	return DXVector4(p[0], p[1], p[2], p[3]);
}
