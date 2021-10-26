#include "ImageParser.h"
#include "FloatImage.h"

void FloatImage::Initialize()
{

}

void FloatImage::SetTextureRoute(std::string texRoute)
{
	m_TextureRoute = texRoute;
}

ParserData::ImageData FloatImage::LoadImagePixel(const char* fileName, unsigned int channels)
{
	std::string filePath = m_TextureRoute + fileName;

	ParserData::ImageData imgData;
	imgData.imgColor = LoadImage_Float(filePath.c_str(), &imgData.width, &imgData.height, nullptr, channels);

	return imgData;
}

DirectX::SimpleMath::Vector4 FloatImage::GetPixelColor(ParserData::ImageData& img, int x, int y)
{
	const float* p = static_cast<float*>(img.imgColor) + (4 * (y * img.width + x));

	return DirectX::SimpleMath::Vector4(p[0], p[1], p[2], p[3]);
}

DirectX::SimpleMath::Vector4 FloatImage::GetPixelColor(ParserData::ImageData& img, float x, float y)
{
	const float* p = static_cast<float*>(img.imgColor) + (4 * ((int)y * img.width + (int)x));

	return DirectX::SimpleMath::Vector4(p[0], p[1], p[2], p[3]);
}
