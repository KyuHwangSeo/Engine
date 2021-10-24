#include "ParserDLL.h"
#include "KHMath.h"
#include "ImageParser.h"

#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb_image.h"

ImageData ImageParser::LoadImageData(const char* fileName, unsigned int channels)
{
	ImageData imgData;
	imgData.imgColor = stbi_loadf(fileName, &imgData.width, &imgData.height, nullptr, channels);
	
	return imgData;
}

PARSER_DLL DXVector4 ImageParser::GetPixelColor(ImageData& img, int x, int y)
{
	const float* p = img.imgColor + (4 * (y * img.width + x));

	return DXVector4(p[0], p[1], p[2], p[3]);
}

PARSER_DLL DXVector4 ImageParser::GetPixelColor(ImageData& img, float x, float y)
{
	const float* p = img.imgColor + (4 * ((int)y * img.width + (int)x));

	return DXVector4(p[0], p[1], p[2], p[3]);
}
