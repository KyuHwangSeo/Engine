#pragma once
#include "ClassType.h"

/// <summary>
/// ShaderResourceView Resource Struct
/// </summary>
/// 
/// - Shader ������ ShaderResourceView Resource�� 1:1 �����ϴ� Struct
/// - Shader���� ������ �������� ��ȣ�� ���۰� ��ġ�ϴ� �Ѱ��� ShaderResourceView
/// - D3DCompiler���� ���� Shader���� ShaderResourceView�� ������ �ξ ������� ������
///   Register Slot�� �ø��� �ʴ´�..
/// 
/// - ������
///  1) ���� struct�� �̸��� ���� ������ Shader ������ UnorderedAccessView�� ��ġ�ؾ��Ѵ� (Struct Name�� ���� UnorderedAccessView�� ã�� ����)

///////////////////////////////////////////////////////////////////////////////////////////
// Global ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

struct gDiffuseMap : public ClassType<gDiffuseMap> {};
struct gNormalMap : public ClassType<gNormalMap> {};
struct gCubeMap : public ClassType<gCubeMap> {};
struct gShadowMap : public ClassType<gShadowMap> {};
struct gSSAOMap : public ClassType<gSSAOMap> {};

///////////////////////////////////////////////////////////////////////////////////////////
// SSAO ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

struct gNormalDepthMap : public ClassType<gNormalDepthMap> {};
struct gRandomVecMap : public ClassType<gRandomVecMap> {};
struct gInputMap : public ClassType<gInputMap> {};

///////////////////////////////////////////////////////////////////////////////////////////
// Deferred ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

struct AlbedoSRV : public ClassType<AlbedoSRV> {};
struct NormalSRV : public ClassType<NormalSRV> {};
struct PositionSRV : public ClassType<PositionSRV> {};
struct ShadowSRV : public ClassType<ShadowSRV> {};
struct SsaoSRV : public ClassType<SsaoSRV> {};

///////////////////////////////////////////////////////////////////////////////////////////
// Blur ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

struct gInput : public ClassType<gInput> {};

///////////////////////////////////////////////////////////////////////////////////////////
// MotionBlur ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

struct gDepthMap : public ClassType<gDepthMap> {};
struct gOrigin : public ClassType<gOrigin> {};
struct gVelocity : public ClassType<gVelocity> {};