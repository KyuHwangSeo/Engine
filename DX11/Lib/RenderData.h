#pragma once
#include "DLLDefine.h"
#include "KHMath.h"

struct RenderData
{
	// Object Matrix
	DXMatrix4X4 world;
	DXMatrix4X4 prev_world;

	// View & Proj
	DXMatrix4X4 camera_view;
	DXMatrix4X4 camera_proj;

	DXMatrix4X4 light_view;
	DXMatrix4X4 light_proj;

	// Buffer
	void* vertex_buffer;
	void* index_buffer;

	UINT vertex_buffer_size;
	UINT index_count;

	// Texture
	void* diffuse_map;
	void* normal_map;
	void* cube_map;

	// Shader
	UINT shader_id;

	// Material
	UINT material_id;

	// Rasterizer Type
	UINT rasterizer_type;

	// Topology Type
	UINT topology_type;
};

