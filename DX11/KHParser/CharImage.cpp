#include "ImageParser.h"
#include "CharImage.h"

void CharImage::Initialize()
{

}

void CharImage::SetTextureRoute(std::string texRoute)
{
	m_TextureRoute = texRoute;
}

ParserData::ImageData CharImage::LoadImagePixel(const char* fileName, unsigned int channels)
{
	std::string filePath = m_TextureRoute + fileName;

	ParserData::ImageData imgData;
	imgData.imgColor = LoadImage_Char(filePath.c_str(), &imgData.width, &imgData.height, nullptr, channels);

	return imgData;
}

DirectX::SimpleMath::Vector4 CharImage::GetPixelColor(ParserData::ImageData& img, int x, int y)
{
	const unsigned char* p = static_cast<unsigned char*>(img.imgColor) + (4 * (y * img.width + x));

	return DirectX::SimpleMath::Vector4(p[0], p[1], p[2], p[3]);
}

DirectX::SimpleMath::Vector4 CharImage::GetPixelColor(ParserData::ImageData& img, float x, float y)
{
	const unsigned char* p = static_cast<unsigned char*>(img.imgColor) + (4 * ((int)y * img.width + (int)x));

	return DirectX::SimpleMath::Vector4(p[0], p[1], p[2], p[3]);
}
