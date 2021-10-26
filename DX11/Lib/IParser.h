#pragma once

enum class ParserType
{
	ASE,
	FBX
};

class IParser
{
public:
	IParser() = default;
	virtual ~IParser() = default;

public:
	static PARSER_DLL IParser* Create(ParserType type);

public:
	virtual PARSER_DLL void Initialize() abstract;
	virtual PARSER_DLL void SetTextureRoute(std::string texRoute) abstract;
	virtual PARSER_DLL void Release() abstract;

	virtual PARSER_DLL ParserData::Model* LoadModel(std::string fileName) { return nullptr; }
	virtual PARSER_DLL ParserData::Model* LoadModel(std::string fileName, bool scaling, bool onlyAni = false) { return nullptr; }
};

