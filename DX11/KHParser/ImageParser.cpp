#include "ImageParser.h"

#include "FloatImage.h"
#include "CharImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Global Parser List
std::vector<ImageParser*> ImageParser::g_ParserList;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// stb_image ���� extern �Լ��� ������ ������ �ֻ��� Ŭ������ �Լ� ����
/////////////////////////////////////////////////////////////////////////////////////////////////////////

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

	ImageParser::g_ParserList.push_back(newParser);

	return newParser;
}

PARSER_DLL void ImageParser::Destroy()
{
	if (ImageParser::g_ParserList.empty()) return;

	for (ImageParser* parser : ImageParser::g_ParserList)
	{
		delete parser;
	}

	ImageParser::g_ParserList.clear();
}