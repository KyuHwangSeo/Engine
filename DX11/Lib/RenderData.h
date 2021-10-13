#pragma once
#include "DLLDefine.h"
#include "KHMath.h"

struct FrameRenderData
{
	DXMatrix4X4 view;
	DXMatrix4X4 proj;
};

struct ObjectRenderData
{
	// Object Matrix
	DXMatrix4X4 world;
	DXMatrix4X4 prev_world;

	// Material
	int material_id;

	// Texture
	void* diffuse_map;
	void* normal_map;
	void* cube_map;

	// Rasterizer
	int rasterizer_id;

	// Shader
	


};

