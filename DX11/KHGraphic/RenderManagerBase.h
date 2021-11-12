#pragma once
#include <queue>
#include "EngineData.h"

///
/// 2021/11/12 18:02
/// SeoKyuHwang
///
/// RenderManger Interface Class
///
/// - 모든 RenderPass를 관리하는 Class
/// - 외부 Game Engine에서 요청한 RenderPass를 관리하는 Class
/// - 내부적으로 Render방식에 따른 정렬은 해준다.
/// 

interface IRenderManager
{
public:
	virtual void Initialize(int width, int height) abstract;
	virtual void Release() abstract;

	// Main Render Pass..
	virtual void Render(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
	// Shader Render Pass..
	virtual void ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
	// SSAO Render Pass..
	virtual void SSAORender() abstract;
	// UI Render Pass..
	virtual void UIRender(std::queue<MeshData*>* meshList, GlobalData* global) abstract;

	virtual void OnResize(int width, int hegiht) abstract;
};