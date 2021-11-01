#pragma once
#include <typeinfo>
/// <summary>
/// Constant Buffer Struct
/// </summary>
/// 
/// - Shader ������ Constant Buffer�� 1:1 �����ϴ� Struct
/// 
/// - ������
///  1) ���� struct�� �̸��� ���� ������ Shader ������ Constant Buffer�� ��ġ�ؾ��Ѵ�
///  2) Struct Name�� ���� Constant Buffer ã������ struct Name ��ȯ �Լ� �ʿ�

///////////////////////////////////////////////////////////////////////////////////////////
// Forward Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct CBuffer
{
	std::string GetName()
	{
		std::string name = typeid(T).name();
		size_t spacePosition = name.find_first_of(" ");
		if (spacePosition != std::string::npos)
			return name.substr(spacePosition + 1, name.length());
		else
			return name;
	}
};

struct cbPerObject : public CBuffer<cbPerObject>
{
	DXMatrix4X4 gWorld;
	DXMatrix4X4 gWorldInvTranspose;
	DXMatrix4X4 gWorldViewProj;
	DXMatrix4X4 gTexTransform;
	DXMatrix4X4 gWorldView;
	DXMatrix4X4 gWorldInvTransposeView;
};

struct cbLights : public CBuffer<cbLights>
{
	DirectionalLight gDirLights[3];
	PointLight gPointLights[4];
	SpotLight gSpotLights[4];
};

struct cbCamera : public CBuffer<cbCamera>
{
	DXVector3 gEyePosW;
};

struct cbShadow : public CBuffer<cbShadow>
{
	DXMatrix4X4 gShadowTransform;
};

struct cbSkinned : public CBuffer<cbSkinned>
{
	DXMatrix4X4 gBoneTransforms[96];
};

struct cbID : public CBuffer<cbID>
{
	int gMatID = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////
// Deferred Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

struct cbLightList : public CBuffer<cbLightList>
{
	DirectionalLight gDirLights[3];
	PointLight gPointLights[16];
	SpotLight gSpotLights[16];

	UINT gPointLightCount;
	UINT gSpotLightCount;
};

struct cbMaterialList : public CBuffer<cbMaterialList>
{
	MaterialData gMaterials[20];
};

struct cbTexViewProj : public CBuffer<cbTexViewProj>
{
	DXMatrix4X4 gViewProjTex;
};

///////////////////////////////////////////////////////////////////////////////////////////
// SSAO Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

struct cbTexel : public CBuffer<cbTexel>
{
	float gTexelSize;
};

struct cbSsaoFrame : public CBuffer<cbSsaoFrame>
{
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

struct cbFullScreen : public CBuffer<cbFullScreen>
{
	DXMatrix4X4 gNowViewProj;
	DXMatrix4X4 gPrevViewProj;
	float gDeltaTime;
};

///////////////////////////////////////////////////////////////////////////////////////////
// UI Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

struct cbPerUI : public CBuffer<cbPerUI>
{
	DXMatrix4X4 gWorldViewProj;
};

struct cbAlpha : public CBuffer<cbAlpha>
{
	float alpha = 0.0f;
}; 
