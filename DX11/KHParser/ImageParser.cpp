#include "ParserDLL.h"
#include "ImageParser.h"

ImageParser::ImageParser()
{

}

ImageParser::~ImageParser()
{

}

ImageData ImageParser::LoadImage(const char* fileName, unsigned int channels)
{
	ImageData imgData;
	imgData.imgColor = stbi_load(fileName, &imgData.width, &imgData.height, nullptr, channels);
	
	return imgData;
}
