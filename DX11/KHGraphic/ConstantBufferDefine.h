#pragma once
#include "SimpleMath.h"
#include "LightHelper.h"
#include "ClassType.h"

#define CONSTANT_BUFFER(ClassName) CREATE_CLASS(ClassName)

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
CONSTANT_BUFFER(cbPerObject)
{
	DirectX::SimpleMath::Matrix gWorld;
	//DirectX::SimpleMath::Matrix gWorldInvTranspose;
	//DirectX::SimpleMath::Matrix gWorldViewProj;
	DirectX::SimpleMath::Matrix gTexTransform;
	//DirectX::SimpleMath::Matrix gWorldView;
	//DirectX::SimpleMath::Matrix gWorldInvTransposeView;
};

CONSTANT_BUFFER(cbLights)
{
	DirectionalLight gDirLights[3];
	PointLight gPointLights[4];
	SpotLight gSpotLights[4];
};

CONSTANT_BUFFER(cbCamera)
{
	DirectX::SimpleMath::Vector3 gEyePosW;
};

CONSTANT_BUFFER(cbShadow)
{
	DirectX::SimpleMath::Matrix gShadowTransform;
};

CONSTANT_BUFFER(cbSkinned)
{
	DirectX::SimpleMath::Matrix gBoneTransforms[96];
};

CONSTANT_BUFFER(cbID)
{
	int gMatID = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////
// Deferred Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

CONSTANT_BUFFER(cbLightList)
{
	DirectionalLight gDirLights[3];
	PointLight gPointLights[16];
	SpotLight gSpotLights[16];

	UINT gPointLightCount;
	UINT gSpotLightCount;
};

CONSTANT_BUFFER(cbMaterialList)
{
	MaterialData gMaterials[20];
};

CONSTANT_BUFFER(cbTexViewProj)
{
	DirectX::SimpleMath::Matrix gViewProjTex;
};

///////////////////////////////////////////////////////////////////////////////////////////
// SSAO Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

CONSTANT_BUFFER(cbTexel)
{
	float gTexelSize;
};

CONSTANT_BUFFER(cbSsaoFrame)
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

CONSTANT_BUFFER(cbFullScreen)
{
	DirectX::SimpleMath::Matrix gNowViewProj;
	DirectX::SimpleMath::Matrix gPrevViewProj;
	float gDeltaTime;
};

///////////////////////////////////////////////////////////////////////////////////////////
// UI Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

CONSTANT_BUFFER(cbPerUI)
{
	DirectX::SimpleMath::Matrix gWorldViewProj;
};

CONSTANT_BUFFER(cbAlpha)
{
	float alpha = 0.0f;
};