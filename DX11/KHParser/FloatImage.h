#pragma once
// Pixel Color Range 0 ~ 1
class FloatImage : public ImageParser
{
public:
	FloatImage() = default;
	~FloatImage() = default;

private:
	std::string m_TextureRoute;

public:
	void Initialize() override;
	void SetTextureRoute(std::string texRoute) override;
	ParserData::ImageData LoadImagePixel(const char* fileName, unsigned int channels) override;

	DirectX::SimpleMath::Vector4 GetPixelColor(ParserData::ImageData& img, int x, int y) override;
	DirectX::SimpleMath::Vector4 GetPixelColor(ParserData::ImageData& img, float x, float y) override;
};

