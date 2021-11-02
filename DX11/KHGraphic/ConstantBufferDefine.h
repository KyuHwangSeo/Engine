#pragma once
#include "SimpleMath.h"
#include "LightHelper.h"
#include "ClassType.h"

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

struct cbPerObject : public ClassType<cbPerObject>
{
	DirectX::SimpleMath::Matrix gWorld;
	DirectX::SimpleMath::Matrix gWorldInvTranspose;
	DirectX::SimpleMath::Matrix gWorldViewProj;
	DirectX::SimpleMath::Matrix gTexTransform;
	DirectX::SimpleMath::Matrix gWorldView;
	DirectX::SimpleMath::Matrix gWorldInvTransposeView;
};

struct cbLights : public ClassType<cbLights>
{
	DirectionalLight gDirLights[3];
	PointLight gPointLights[4];
	SpotLight gSpotLights[4];
};

struct cbCamera : public ClassType<cbCamera>
{
	DirectX::SimpleMath::Vector3 gEyePosW;
};

struct cbShadow : public ClassType<cbShadow>
{
	DirectX::SimpleMath::Matrix gShadowTransform;
};

struct cbSkinned : public ClassType<cbSkinned>
{
	DirectX::SimpleMath::Matrix gBoneTransforms[96];
};

struct cbID : public ClassType<cbID>
{
	int gMatID = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////
// Deferred Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

struct cbLightList : public ClassType<cbLightList>
{
	DirectionalLight gDirLights[3];
	PointLight gPointLights[16];
	SpotLight gSpotLights[16];

	UINT gPointLightCount;
	UINT gSpotLightCount;
};

struct cbMaterialList : public ClassType<cbMaterialList>
{
	MaterialData gMaterials[20];
};

struct cbTexViewProj : public ClassType<cbTexViewProj>
{
	DirectX::SimpleMath::Matrix gViewProjTex;
};

///////////////////////////////////////////////////////////////////////////////////////////
// SSAO Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

struct cbTexel : public ClassType<cbTexel>
{
	float gTexelSize;
};

struct cbSsaoFrame : public ClassType<cbSsaoFrame>
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

struct cbFullScreen : public ClassType<cbFullScreen>
{
	DirectX::SimpleMath::Matrix gNowViewProj;
	DirectX::SimpleMath::Matrix gPrevViewProj;
	float gDeltaTime;
};

///////////////////////////////////////////////////////////////////////////////////////////
// UI Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

struct cbPerUI : public ClassType<cbPerUI>
{
	DirectX::SimpleMath::Matrix gWorldViewProj;
};

struct cbAlpha : public ClassType<cbAlpha>
{
	float alpha = 0.0f;
};