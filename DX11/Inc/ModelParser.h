#pragma once
#pragma warning(disable : 4251)

#include "ParserDLL.h"
#include "../ShareData/ParserData.h"

/// Model Parser InterFace Class
/// 
/// - �ܺο��� ���������� ����� �Լ����� ��Ƴ� InterFace Class
/// - Parser Type�� ���� DLL ���ο��� �Ҵ� �� ������ ��ȯ ����
/// - �ش� Parser Class�� �ƴ� InterFace Class�� ���� �Լ� ȣ�� ���

class ModelParser
{
public:
	ModelParser() = default;
	virtual ~ModelParser() = default;

public:
	// ��� �� Model Parser Ÿ��
	enum Type
	{
		ASE,
		FBX
	};

public:
	// Create �Լ��� ���� ModelParser ����..
	static PARSER_DLL ModelParser* Create(Type type);

public:
	// �⺻���� �ʱ�ȭ �Լ�..
	virtual PARSER_DLL void Initialize() abstract;

	// �⺻ Texture ��� ���� �Լ�..
	virtual PARSER_DLL void SetTextureRoute(std::string texRoute) abstract;

	// Model Data ���� �Լ�..
	virtual PARSER_DLL void Release() abstract;

	// Model Load �Լ�..
	virtual PARSER_DLL ParserData::Model* LoadModel(std::string fileName) { return nullptr; }
	virtual PARSER_DLL ParserData::Model* LoadModel(std::string fileName, bool scaling, bool onlyAni = false) { return nullptr; }
};

