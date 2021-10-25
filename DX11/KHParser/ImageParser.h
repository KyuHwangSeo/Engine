#pragma once

struct ImageData
{
	int width;
	int height;
};

// Pixel By Color 0 ~ 1
struct PARSER_DLL ImageDataF : public ImageData
{
	float* imgColor;
};

// Pixel By Color 0 ~ 255
struct PARSER_DLL ImageDataI : public ImageData
{
	unsigned char* imgColor;
};

class ImageParser
{
public:
	ImageParser() = default;
	~ImageParser() = default;

private:
	std::string m_TextureRoute;

public:
	PARSER_DLL void Initialize(std::string texRoute);
	PARSER_DLL ImageDataF LoadImagePixelByFloat(const char* fileName, unsigned int channels);
	PARSER_DLL ImageDataI LoadImagePixelByInt(const char* fileName, unsigned int channels);

	PARSER_DLL DXVector4 GetPixelColorF(ImageDataF& img, int x, int y);
	PARSER_DLL DXVector4 GetPixelColorF(ImageDataF& img, float x, float y);
	PARSER_DLL DXVector4 GetPixelColorI(ImageDataI& img, int x, int y);
	PARSER_DLL DXVector4 GetPixelColorI(ImageDataI& img, float x, float y);
};