#pragma once
#include "ParserDLL.h"
#include "../ShareData/ParserData.h"

class ModelParser
{
public:
	ModelParser() = default;
	virtual ~ModelParser() = default;

public:
	enum class Type
	{
		ASE,
		FBX
	};

public:
	static PARSER_DLL ModelParser* Create(Type type);

public:
	virtual PARSER_DLL void Initialize() abstract;
	virtual PARSER_DLL void SetTextureRoute(std::string texRoute) abstract;
	virtual PARSER_DLL void Release() abstract;

	virtual PARSER_DLL ParserData::Model* LoadModel(std::string fileName) { return nullptr; }
	virtual PARSER_DLL ParserData::Model* LoadModel(std::string fileName, bool scaling, bool onlyAni = false) { return nullptr; }
};

