#include "ImageParser.h"

#include "FloatImage.h"
#include "CharImage.h"

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