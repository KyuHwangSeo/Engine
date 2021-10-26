#include "ModelParser.h"

#include "ASEFile.h"
#include "CASEParser.h"

#include "BoneWeights.h"
#define FBXSDK_SHARED
#pragma comment(lib, "libfbxsdk")
#include <fbxsdk.h>
#include "FBXParser.h"

ModelParser* ModelParser::Create(Type type)
{
	ModelParser* newParser = nullptr;

	switch (type)
	{
	case ModelParser::Type::ASE:
		newParser = new CASEParser;
		newParser->Initialize();
		break;
	case ModelParser::Type::FBX:
		newParser = new FBXParser;
		newParser->Initialize();
		break;
	default:
		break;
	}

	return newParser;
}
