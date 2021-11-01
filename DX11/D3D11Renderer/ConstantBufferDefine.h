#pragma once
#include "../ShareData/SimpleMath.h"
#include "../ShareData/LightHelper.h"
#include "BufferNameDefine.h"

/// <summary>
/// ConstantBuffer Resource Struct
/// </summary>
/// 
/// - Shader 내부의 ConstantBuffer Resource와 1:1 대응하는 Struct
/// - Shader에서 설정한 레지스터 번호와 버퍼가 일치하는 한개의 Constant Buffer
/// - D3DCompiler에서 현재 Shader에서 ConstantBuffer을 선언해 두어도 사용하지 않으면
///   Register Slot에 올리지 않는다..
/// 
/// - 주의점
///  1) 현재 struct의 이름과 변수 순서는 Shader 내부의 ConstantBuffer와 일치해야한다 (Struct Name을 통해 Constant Buffer를 찾기 때문)

///////////////////////////////////////////////////////////////////////////////////////////
// Forward Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

struct cbPerObject : public BufferName<cbPerObject>
{
	DirectX::SimpleMath::Matrix gWorld;
	DirectX::SimpleMath::Matrix gWorldInvTranspose;
	DirectX::SimpleMath::Matrix gWorldViewProj;
	DirectX::SimpleMath::Matrix gTexTransform;
	DirectX::SimpleMath::Matrix gWorldView;
	DirectX::SimpleMath::Matrix gWorldInvTransposeView;
};

struct cbLights : public BufferName<cbLights>
{
	DirectionalLight gDirLights[3];
	PointLight gPointLights[4];
	SpotLight gSpotLights[4];
};

struct cbCamera : public BufferName<cbCamera>
{
	DirectX::SimpleMath::Vector3 gEyePosW;
};

struct cbShadow : public BufferName<cbShadow>
{
	DirectX::SimpleMath::Matrix gShadowTransform;
};

struct cbSkinned : public BufferName<cbSkinned>
{
	DirectX::SimpleMath::Matrix gBoneTransforms[96];
};

struct cbID : public BufferName<cbID>
{
	int gMatID = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////
// Deferred Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

struct cbLightList : public BufferName<cbLightList>
{
	DirectionalLight gDirLights[3];
	PointLight gPointLights[16];
	SpotLight gSpotLights[16];

	UINT gPointLightCount;
	UINT gSpotLightCount;
};

struct cbMaterialList : public BufferName<cbMaterialList>
{
	MaterialData gMaterials[20];
};

struct cbTexViewProj : public BufferName<cbTexViewProj>
{
	DirectX::SimpleMath::Matrix gViewProjTex;
};

///////////////////////////////////////////////////////////////////////////////////////////
// SSAO Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

struct cbTexel : public BufferName<cbTexel>
{
	float gTexelSize;
};

struct cbSsaoFrame : public BufferName<cbSsaoFrame>
{
	DirectX::SimpleMath::Matrix  gViewToTexSpace;
	DirectX::SimpleMath::Vector4 gOffsetVectors[14];
	DirectX::SimpleMath::Vector4 gFrustumCorners[4];

	float    gOcclusionRadius = 0.5f;
	float    gOcclusionFadeStart = 0.2f;
	float    gOcclusionFadeEnd = 2.0f;
	float    gSurfaceEpsilon = 0.05f;
};

///////////////////////////////////////////////////////////////////////////////////////////
// Full Screen Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

struct cbFullScreen : public BufferName<cbFullScreen>
{
	DirectX::SimpleMath::Matrix gNowViewProj;
	DirectX::SimpleMath::Matrix gPrevViewProj;
	float gDeltaTime;
};

///////////////////////////////////////////////////////////////////////////////////////////
// UI Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

struct cbPerUI : public BufferName<cbPerUI>
{
	DirectX::SimpleMath::Matrix gWorldViewProj;
};

struct cbAlpha : public BufferName<cbAlpha>
{
	float alpha = 0.0f;
};