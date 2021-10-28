#pragma once
#pragma warning(disable : 4251)

#include "ParserDLL.h"
#include "../ShareData/ParserData.h"

/// Image Parser InterFace Class
/// 
/// - �ܺο��� ���������� ����� �Լ����� ��Ƴ� InterFace Class
/// - Parser Type�� ���� DLL ���ο��� �Ҵ� �� ������ ��ȯ ����
/// - �ش� Parser Class�� �ƴ� InterFace Class�� ���� �Լ� ȣ�� ���

class ImageParser
{
public:
	// ��� �� Image Color Ÿ��
	enum Type
	{
		FLOAT,
		CHAR
	};

protected:
	float* LoadImage_Float(const char* filename, int* x, int* y, int* comp, int req_comp);
	unsigned char* LoadImage_Char(const char* filename, int* x, int* y, int* comp, int req_comp);

public:
	// Create �Լ��� ���� ImageParser ����..
	static PARSER_DLL ImageParser* Create(Type type);

public:
	// �⺻���� �ʱ�ȭ �Լ�..
	virtual PARSER_DLL void Initialize() abstract;

	// �⺻ Texture ��� ���� �Լ�..
	virtual PARSER_DLL void SetTextureRoute(std::string texRoute) abstract;

	// �ش� Image Pixel Color ���� �Լ�..
	virtual PARSER_DLL ParserData::ImageData LoadImagePixel(const char* fileName, unsigned int channels) abstract;

	// �ش� ��ġ Pixel Color ���� �Լ�..
	virtual PARSER_DLL DirectX::SimpleMath::Vector4 GetPixelColor(ParserData::ImageData& img, int x, int y) abstract;
	virtual PARSER_DLL DirectX::SimpleMath::Vector4 GetPixelColor(ParserData::ImageData& img, float x, float y) abstract;
};