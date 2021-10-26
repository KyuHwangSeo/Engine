#include "ImageParser.h"

#include "FloatImage.h"
#include "CharImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float* ImageParser::LoadImage_Float(const char* filename, int* x, int* y, int* comp, int req_comp)
{
	return stbi_loadf(filename, x, y, comp, req_comp);
}

unsigned char* ImageParser::LoadImage_Char(const char* filename, int* x, int* y, int* comp, int req_comp)
{
	return stbi_load(filename, x, y, comp, req_comp);
}

PARSER_DLL ImageParser* ImageParser::Create(Type type)
{
	ImageParser* newParser = nullptr;

	switch (type)
	{
	case ImageParser::Type::FLOAT:
		newParser = new FloatImage();
		newParser->Initialize();
		break;
	case ImageParser::Type::CHAR:
		newParser = new CharImage();
		newParser->Initialize();
		break;
	default:
		break;
	}

	return newParser;
}
