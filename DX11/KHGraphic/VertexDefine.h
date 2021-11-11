#pragma once
#include "SimpleMath.h"

class BufferData
{
public:
	BufferData() : VB(nullptr), IB(nullptr), IndexCount(0), Stride(0), Offset(0) {}
	~BufferData() { RESET_COM(VB); RESET_COM(IB); }

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> VB;		// Vertex Buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> IB;		// Index Buffer

	UINT IndexCount;		// Index Count
	UINT Stride;			// Vertex Struct Size
	UINT Offset;			// Offset
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

struct MeshVertex
{
	DirectX::SimpleMath::Vector3 Pos;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Vector2 Tex;
	DirectX::SimpleMath::Vector3 Tangent;
};

struct SkinVertex
{
	DirectX::SimpleMath::Vector3 Pos;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Vector2 Tex;
	DirectX::SimpleMath::Vector3 Tangent;

	UINT BoneIndex[4];
	float BoneWeight[4];
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