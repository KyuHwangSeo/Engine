#pragma once
#include <queue>
#include "EngineData.h"

interface IRenderManager
{
public:
	virtual void Initialize(int width, int height) abstract;
	virtual void Release() abstract;

	virtual void Render(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
	virtual void ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
	virtual void SSAORender() abstract;
	virtual void UIRender(std::queue<MeshData*>* meshList, GlobalData* global) abstract;

	virtual void OnResize(int width, int hegiht) abstract;
};