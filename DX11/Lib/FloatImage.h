#pragma once
// Pixel By Color 0 ~ 1
class FloatImage : public ImageParser
{
public:
	FloatImage() = default;
	~FloatImage() = default;

public:
	void Initialize();
	void SetTextureRoute(std::string texRoute);
	ParserData::ImageData LoadImagePixel(const char* fileName, unsigned int channels);

	DirectX::SimpleMath::Vector4 GetPixelColor(ParserData::ImageData& img, int x, int y);
	DirectX::SimpleMath::Vector4 GetPixelColor(ParserData::ImageData& img, float x, float y);
};

