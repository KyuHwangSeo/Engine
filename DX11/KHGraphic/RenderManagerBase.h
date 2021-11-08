#pragma once
#include <queue>
#include "EngineData.h"

interface IRenderManager
{
public:
	virtual void Initialize(int width, int height) abstract;
	virtual void Render(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
};