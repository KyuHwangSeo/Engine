#pragma once
#include "SimpleMath.h"
#include "LightHelper.h"
#include "ClassType.h"

#define CONSTANT_BUFFER(ClassName) CREATE_CLASS(ClassName)

/// <summary>
/// ConstantBuffer Resource Struct
/// </summary>
/// 
/// - Shader ������ ConstantBuffer Resource�� 1:1 �����ϴ� Struct
/// - Shader���� ������ �������� ��ȣ�� ���۰� ��ġ�ϴ� �Ѱ��� Constant Buffer
/// - D3DCompiler���� ���� Shader���� ConstantBuffer�� ������ �ξ ������� ������
///   Register Slot�� �ø��� �ʴ´�..
/// 
/// - ������
///  1) ���� struct�� �̸��� ���� ������ Shader ������ ConstantBuffer�� ��ġ�ؾ��Ѵ� (Struct Name�� ���� Constant Buffer�� ã�� ����)

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