#pragma once

enum class eDSState
{
	DEFALT = 0,
	NODEPTH
};

enum class eRState
{
	SOLID = 0,
	WIRE,
	NOCULL,
	DEPTH
};

enum class eBState : int
{
	BLEND_ONE = 0
};
