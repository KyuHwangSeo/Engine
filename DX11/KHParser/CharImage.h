#pragma once
// Pixel Color Range 0 ~ 255
class CharImage : public ImageParser
{
public:
	CharImage() = default;
	~CharImage() = default;

private:
	std::string m_TextureRoute;

public:
	void Initialize();
	void SetTextureRoute(std::string texRoute);
	ParserData::ImageData LoadImagePixel(const char* fileName, unsigned int channels);

	DirectX::SimpleMath::Vector4 GetPixelColor(ParserData::ImageData& img, int x, int y);
	DirectX::SimpleMath::Vector4 GetPixelColor(ParserData::ImageData& img, float x, float y);
};

