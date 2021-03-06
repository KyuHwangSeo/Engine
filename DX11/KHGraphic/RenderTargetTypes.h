#pragma once

/// <summary>
/// 여러가지 타입을 한개의 함수로 반환하기 위해 만든 Class
/// 해당 Class의 Type을 반영하여 원본 Class Operator를 통해 여러가지 타입으로 반환
/// </summary>

class GraphicResourceManager;
class BasicRenderTarget;
class ComputeRenderTarget;

class OriginalRenderTarget
{
public:
	GraphicResourceManager* pThis;
	eRenderTarget state;

public:
	operator BasicRenderTarget* ();
	operator ComputeRenderTarget* ();
};