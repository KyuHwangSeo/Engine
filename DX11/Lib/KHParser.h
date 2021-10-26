#pragma once

#define WIN32_LEAN_AND_MEAN

#include <string>
#include <vector>
#include <unordered_map>

#include "ModelParser.h"

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

#include "ImageParser.h"