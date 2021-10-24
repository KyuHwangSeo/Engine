#pragma once

struct PARSER_DLL ImageData
{
	int width;
	int height;
	float* imgColor;
};

class ImageParser
{
public:
	ImageParser() = default;
	~ImageParser() = default;

public:
	PARSER_DLL ImageData LoadImageData(const char* fileName, unsigned int channels);
	PARSER_DLL DXVector4 GetPixelColor(ImageData& img, int x, int y);
	PARSER_DLL DXVector4 GetPixelColor(ImageData& img, float x, float y);
};

