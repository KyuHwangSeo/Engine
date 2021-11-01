#pragma once
#include "BufferNameDefine.h"

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