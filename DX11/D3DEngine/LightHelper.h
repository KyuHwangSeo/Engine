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
using namespace DirectX;

// Note: Make sure structure alignment agrees with HLSL structure padding rules. 
//   Elements are packed into 4D vectors with the restriction that an element
//   cannot straddle a 4D vector boundary.

struct DLL_DECLSPEC DirectionalLight
{
	DirectionalLight() = default;

	DXVector4 Ambient;
	DXVector4 Diffuse;
	DXVector4 Specular;
	DXVector3 Direction;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct DLL_DECLSPEC PointLight
{
	PointLight() = default;
	
	DXVector4 Ambient;
	DXVector4 Diffuse;
	DXVector4 Specular;

	// Packed into 4D vector: (Position, Range)
	DXVector3 Position;
	float Range;

	// Packed into 4D vector: (A0, A1, A2, Pad)
	DXVector3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct DLL_DECLSPEC SpotLight
{
	SpotLight() = default;
	
	DXVector4 Ambient;
	DXVector4 Diffuse;
	DXVector4 Specular;

	// Packed into 4D vector: (Position, Range)
	DXVector3 Position;
	float Range;

	// Packed into 4D vector: (Direction, Spot)
	DXVector3 Direction;
	float Spot;

	// Packed into 4D vector: (Att, Pad)
	DXVector3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct DLL_DECLSPEC MaterialData
{
	MaterialData() = default;

	DXVector4 Ambient;
	DXVector4 Diffuse;
	DXVector4 Specular; // w = SpecPower
	DXVector4 Reflect;
};

struct DLL_DECLSPEC LightData
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