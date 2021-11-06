#pragma once
#include <windows.h>
#include <queue>
#include "EngineData.h"

interface IRenderManager
{
	virtual void Initialize() abstract;
	virtual void Render(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
};