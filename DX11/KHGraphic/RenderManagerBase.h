#pragma once
#include <queue>
#include "EngineData.h"

interface IRenderManager
{
public:
	virtual void Initialize() abstract;
	virtual void Render(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
};