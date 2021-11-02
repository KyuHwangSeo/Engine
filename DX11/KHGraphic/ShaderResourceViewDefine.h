#pragma once
#include "ClassType.h"

/// <summary>
/// ShaderResourceView Resource Struct
/// </summary>
/// 
/// - Shader 내부의 ShaderResourceView Resource와 1:1 대응하는 Struct
/// - Shader에서 설정한 레지스터 번호와 버퍼가 일치하는 한개의 ShaderResourceView
/// - D3DCompiler에서 현재 Shader에서 ShaderResourceView을 선언해 두어도 사용하지 않으면
///   Register Slot에 올리지 않는다..
/// 
/// - 주의점
///  1) 현재 struct의 이름과 변수 순서는 Shader 내부의 UnorderedAccessView와 일치해야한다 (Struct Name을 통해 UnorderedAccessView를 찾기 때문)

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