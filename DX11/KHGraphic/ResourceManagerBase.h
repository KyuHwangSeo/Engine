#pragma once

interface IGraphicResourceManager
{
public:
	virtual void OnResize(int width, int height) abstract;
};