#pragma once
/// <summary>
/// Constant Buffer Struct
/// </summary>
/// 
/// - Shader 내부의 Constant Buffer와 1:1 대응하는 Struct
/// 
/// - 주의점
///  1) 현재 struct의 이름과 변수 순서는 Shader 내부의 Constant Buffer와 일치해야한다
///  2) Struct Name을 통해 Constant Buffer 찾기위해 struct Name 반환 함수 필요

///////////////////////////////////////////////////////////////////////////////////////////
// Forward Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

struct cbPerObject
{
	std::string GetName() { return "cbPerObject"; }
	
	DXMatrix4X4 gWorld;
	DXMatrix4X4 gWorldInvTranspose;
	DXMatrix4X4 gWorldViewProj;
	DXMatrix4X4 gTexTransform;
	DXMatrix4X4 gWorldView;
	DXMatrix4X4 gWorldInvTransposeView;
};

struct cbLights
{
	std::string GetName() { return "cbLights"; }
	
	DirectionalLight gDirLights[3];
	PointLight gPointLights[4];
	SpotLight gSpotLights[4];
};

struct cbMaterial
{
	std::string GetName() { return "cbMaterial"; }
	
	MaterialData gMaterial;
};

struct cbCamera
{
	std::string GetName() { return "cbCamera"; }
	
	DXVector3 gEyePosW;
};

struct cbShadow
{
	std::string GetName() { return "cbShadow"; }
	
	DXMatrix4X4 gShadowTransform;
};

struct cbSkinned
{
	std::string GetName() { return "cbSkinned"; }
	
	DXMatrix4X4 gBoneTransforms[96];
};

struct cbID
{
	std::string GetName() { return "cbID"; }
	
	int gMatID = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////
// Deferred Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

struct cbLightList
{
	std::string GetName() { return "cbLightList"; }

	DirectionalLight gDirLights[3];
	PointLight gPointLights[16];
	SpotLight gSpotLights[16];

	UINT gPointLightCount;
	UINT gSpotLightCount;
};

struct cbMaterialList
{
	std::string GetName() { return "cbMaterialList"; }
	MaterialData gMaterials[20];
};

struct cbTexViewProj
{
	std::string GetName() { return "cbTexViewProj"; }
	DXMatrix4X4 gViewProjTex;
};

///////////////////////////////////////////////////////////////////////////////////////////
// SSAO Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

struct cbTexel
{
	std::string GetName() { return "cbTexel"; }
	float gTexelSize;
};

struct cbSsaoFrame
{
	std::string GetName() { return "cbSsaoFrame"; }
	DXMatrix4X4 gViewToTexSpace;
	DXVector4   gOffsetVectors[14];
	DXVector4   gFrustumCorners[4];

	float    gOcclusionRadius = 0.5f;
	float    gOcclusionFadeStart = 0.2f;
	float    gOcclusionFadeEnd = 2.0f;
	float    gSurfaceEpsilon = 0.05f;
};

///////////////////////////////////////////////////////////////////////////////////////////
// Full Screen Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

struct cbFullScreen
{
	std::string GetName() { return "cbFullScreen"; }
	DXMatrix4X4 gNowViewProj;
	DXMatrix4X4 gPrevViewProj;
	float gDeltaTime;
};

///////////////////////////////////////////////////////////////////////////////////////////
// UI Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

struct cbPerUI
{
	std::string GetName() { return "cbPerUI"; }
	DXMatrix4X4 gWorldViewProj;
};

struct cbAlpha
{
	std::string GetName() { return "cbAlpha"; }
	float alpha = 0.0f;
};