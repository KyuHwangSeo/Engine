#pragma once
#include "RenderManagerBase.h"

class RenderManager : public IRenderManager
{
public:
	RenderManager();
	~RenderManager();

public:
	void Initialize() override;
	void Render(std::queue<MeshData*>* meshList, GlobalData* global) override;

};

