#pragma once
#include <queue>
#include "EngineData.h"

///
/// 2021/11/12 18:02
/// SeoKyuHwang
///
/// RenderManger Interface Class
///
/// - ��� RenderPass�� �����ϴ� Class
/// - �ܺ� Game Engine���� ��û�� RenderPass�� �����ϴ� Class
/// - ���������� Render��Ŀ� ���� ������ ���ش�.
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