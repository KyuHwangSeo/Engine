//***************************************************************************************
// LightHelper.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Helper classes for lighting.
//***************************************************************************************

#ifndef LIGHTHELPER_H
#define LIGHTHELPER_H

#include <Windows.h>

// LEHIDE
//#include <xnamath.h>
#include <DirectXMath.h>
#include "SimpleMath.h"

// Note: Make sure structure alignment agrees with HLSL structure padding rules. 
//   Elements are packed into 4D vectors with the restriction that an element
//   cannot straddle a 4D vector boundary.

struct DirectionalLight
{
	DirectionalLight() = default;

	DirectX::SimpleMath::Vector4 Ambient;
	DirectX::SimpleMath::Vector4 Diffuse;
	DirectX::SimpleMath::Vector4 Specular;
	DirectX::SimpleMath::Vector3 Direction;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct PointLight
{
	PointLight() = default;
	
	DirectX::SimpleMath::Vector4 Ambient;
	DirectX::SimpleMath::Vector4 Diffuse;
	DirectX::SimpleMath::Vector4 Specular;

	// Packed into 4D vector: (Position, Range)
	DirectX::SimpleMath::Vector3 Position;
	float Range;

	// Packed into 4D vector: (A0, A1, A2, Pad)
	DirectX::SimpleMath::Vector3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct SpotLight
{
	SpotLight() = default;
	
	DirectX::SimpleMath::Vector4 Ambient;
	DirectX::SimpleMath::Vector4 Diffuse;
	DirectX::SimpleMath::Vector4 Specular;

	// Packed into 4D vector: (Position, Range)
	DirectX::SimpleMath::Vector3 Position;
	float Range;

	// Packed into 4D vector: (Direction, Spot)
	DirectX::SimpleMath::Vector3 Direction;
	float Spot;

	// Packed into 4D vector: (Att, Pad)
	DirectX::SimpleMath::Vector3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct MaterialData
{
	MaterialData() = default;

	DirectX::SimpleMath::Vector4 Ambient;
	DirectX::SimpleMath::Vector4 Diffuse;
	DirectX::SimpleMath::Vector4 Specular; // w = SpecPower
	DirectX::SimpleMath::Vector4 Reflect;
};

struct LightData
{
	DirectionalLight DirLights[3];
	PointLight PointLights[4];
	SpotLight SpotLights[4];
};

static bool operator==(MaterialData& mat1, MaterialData& mat2)
{
	if (mat1.Ambient == mat2.Ambient && mat1.Diffuse == mat2.Diffuse && mat1.Specular == mat2.Specular && mat1.Reflect == mat2.Reflect)
		return true;
	else
		return false;
}

#endif // LIGHTHELPER_H