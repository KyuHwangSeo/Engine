#pragma once
#pragma warning(disable : 4251)

#include "ParserDLL.h"
#include "../ShareData/ParserData.h"

/// Image Parser InterFace Class
/// 
/// - 외부에서 실질적으로 사용할 함수들을 모아논 InterFace Class
/// - Parser Type을 통해 DLL 내부에서 할당 후 포인터 반환 형식
/// - 해당 Parser Class가 아닌 InterFace Class를 통해 함수 호출 방식

class ImageParser
{
public:
	// 사용 할 Image Color 타입
	enum Type
	{
		FLOAT,
		CHAR
	};

protected:
	float* LoadImage_Float(const char* filename, int* x, int* y, int* comp, int req_comp);
	unsigned char* LoadImage_Char(const char* filename, int* x, int* y, int* comp, int req_comp);

public:
	// Create 함수를 통해 ImageParser 생성..
	static PARSER_DLL ImageParser* Create(Type type);

public:
	// 기본적인 초기화 함수..
	virtual PARSER_DLL void Initialize() abstract;

	// 기본 Texture 경로 설정 함수..
	virtual PARSER_DLL void SetTextureRoute(std::string texRoute) abstract;

	// 해당 Image Pixel Color 추출 함수..
	virtual PARSER_DLL ParserData::ImageData LoadImagePixel(const char* fileName, unsigned int channels) abstract;

	// 해당 위치 Pixel Color 추출 함수..
	virtual PARSER_DLL DirectX::SimpleMath::Vector4 GetPixelColor(ParserData::ImageData& img, int x, int y) abstract;
	virtual PARSER_DLL DirectX::SimpleMath::Vector4 GetPixelColor(ParserData::ImageData& img, float x, float y) abstract;
};