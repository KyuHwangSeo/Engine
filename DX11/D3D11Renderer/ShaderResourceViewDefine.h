#pragma once
#include "BufferNameDefine.h"

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

struct gDiffuseMap : public BufferName<gDiffuseMap> {};
struct gNormalMap : public BufferName<gNormalMap> {};
struct gCubeMap : public BufferName<gCubeMap> {};
struct gShadowMap : public BufferName<gShadowMap> {};
struct gSSAOMap : public BufferName<gSSAOMap> {};

///////////////////////////////////////////////////////////////////////////////////////////
// SSAO ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

struct gNormalDepthMap : public BufferName<gNormalDepthMap> {};
struct gRandomVecMap : public BufferName<gRandomVecMap> {};
struct gInputMap : public BufferName<gInputMap> {};

///////////////////////////////////////////////////////////////////////////////////////////
// Deferred ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

struct AlbedoSRV : public BufferName<AlbedoSRV> {};
struct NormalSRV : public BufferName<NormalSRV> {};
struct PositionSRV : public BufferName<PositionSRV> {};
struct ShadowSRV : public BufferName<ShadowSRV> {};
struct SsaoSRV : public BufferName<SsaoSRV> {};

///////////////////////////////////////////////////////////////////////////////////////////
// Blur ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

struct gInput : public BufferName<gInput> {};

///////////////////////////////////////////////////////////////////////////////////////////
// MotionBlur ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

struct gDepthMap : public BufferName<gDepthMap> {};
struct gOrigin : public BufferName<gOrigin> {};
struct gVelocity : public BufferName<gVelocity> {};