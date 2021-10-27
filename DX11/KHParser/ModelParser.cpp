#include "ModelParser.h"

#include "ASEFile.h"
#include "ASEParser.h"

#include "BoneWeights.h"
#define FBXSDK_SHARED
#pragma comment(lib, "libfbxsdk")
#include <fbxsdk.h>
#include "FBXParser.h"

// Global Parser List
std::vector<ModelParser*> ModelParser::g_ParserList;

PARSER_DLL ModelParser* ModelParser::Create(Type type)
{
	ModelParser* newParser = nullptr;

	switch (type)
	{
	case ModelParser::Type::ASE:
		newParser = new ASEParser;
		newParser->Initialize();
		break;
	case ModelParser::Type::FBX:
		newParser = new FBXParser;
		newParser->Initialize();
		break;
	default:
		break;
	}

	ModelParser::g_ParserList.push_back(newParser);

	return newParser;
}

PARSER_DLL void ModelParser::Destroy()
{
	if (ModelParser::g_ParserList.empty()) return;

	for (ModelParser* parser : ModelParser::g_ParserList)
	{
		parser->Release();
		delete parser;
	}

	ModelParser::g_ParserList.clear();
}
