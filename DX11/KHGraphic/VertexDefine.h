#pragma once
#include "SimpleMath.h"

class BufferData
{
public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> VB;		// 버텍스버퍼
	Microsoft::WRL::ComPtr<ID3D11Buffer> IB;		// 인덱스버퍼

	UINT IndexCount = 0;			// 버텍스 개수
	UINT Stride = 0;				// 구조체 사이즈
	UINT Offset = 0;			// Offset
};

struct PosTexVertex
{
	DirectX::SimpleMath::Vector3 Pos;
	DirectX::SimpleMath::Vector2 Tex;
};

struct PosNormalTexVertex
{
	DirectX::SimpleMath::Vector3 Pos;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Vector2 Tex;
};

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