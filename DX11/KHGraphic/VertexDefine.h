#pragma once
#include "SimpleMath.h"

struct NormalMapVertex
{
	DirectX::SimpleMath::Vector3 Pos;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Vector2 Tex;
	DirectX::SimpleMath::Vector3 Tangent;
};

struct TerrainVertex
{
	DirectX::SimpleMath::Vector3 Pos;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Vector2 Tex;
	DirectX::SimpleMath::Vector3 Tangent;
	DirectX::SimpleMath::Vector4 Mask1;
	DirectX::SimpleMath::Vector4 Mask2;
};