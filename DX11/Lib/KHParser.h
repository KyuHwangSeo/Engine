#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

#ifdef _WIN64
	#ifdef _DEBUG
		#pragma comment(lib, "KHParser_x64d")
	#else
		#pragma comment(lib, "KHParser_x64")
	#endif
#else
	#ifdef _DEBUG
		#pragma comment(lib, "KHParser_x86d")
	#else
		#pragma comment(lib, "KHParser_x86")
	#endif
#endif

#include "KHMath.h"

#include "AnimationData.h"
#include "CParsingDataClass.h"
#include "ASEFile.h"
#include "CASEParser.h"

#define FBXSDK_SHARED
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk")

using namespace fbxsdk;

#include <windows.h>
#include <cassert>
#include "BoneWeights.h"
#include "FBXParser.h"