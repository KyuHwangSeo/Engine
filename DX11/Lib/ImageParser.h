#pragma once
#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb_image.h"
struct PARSER_DLL ImageData
{
	int width;
	int height;
	unsigned char* imgColor;
};

class ImageParser
{
public:
	ImageParser();
	~ImageParser();

public:
	PARSER_DLL ImageData LoadImage(const char* fileName, unsigned int channels);
};

