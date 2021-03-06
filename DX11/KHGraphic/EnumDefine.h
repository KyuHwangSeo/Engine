#pragma once

enum class eDepthStencilState
{
	DEFALT = 0,
	NODEPTH
};

enum class eRasterizerState
{
	SOLID = 0,
	WIRE,
	NOCULL,
	DEPTH
};

enum class eBlendState
{
	BLEND_ONE = 0,

};

enum class eViewPort
{
	SCREEN = 0,
	SHADOW,
	SSAO,
};

enum class eDepthStencilView
{
	DEFALT = 0,
	SHADOW
};

enum class eRenderTarget
{
	BACK_BUFFER,

};

enum class eBuffer
{
	SCREEN,
	SSAO
};

enum class eRenderType
{
	MESH,
	SKIN,
	TERRAIN
};