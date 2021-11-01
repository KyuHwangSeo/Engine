#pragma once
#include "../ShareData/SimpleMath.h"
#include "../ShareData/LightHelper.h"
#include "BufferNameDefine.h"

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