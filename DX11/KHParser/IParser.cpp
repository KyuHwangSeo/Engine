#include "ParserDLL.h"

#include <vector>
#include <string>
#include <windows.h>
#include <cassert>
using namespace std;

#include "CParsingDataClass.h"

#include "IParser.h"

#include "ASEFile.h"
#include "CASEParser.h"

#include "BoneWeights.h"
#define FBXSDK_SHARED
#pragma comment(lib, "libfbxsdk")
#include <fbxsdk.h>
#include "FBXParser.h"

IParser* IParser::Create(ParserType type)
{
	IParser* newParser = nullptr;

	switch (type)
	{
	case ParserType::ASE:
		newParser = new CASEParser;
		break;
	case ParserType::FBX:
		newParser = new FBXParser;
		break;
	default:
		break;
	}

	return newParser;
}
