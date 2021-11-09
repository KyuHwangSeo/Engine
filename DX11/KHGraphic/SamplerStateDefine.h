#pragma once
#include "ClassType.h"

#define SAMPLER_STATE(ClassName) CREATE_EMPTY_CLASS(ClassName)

/// <summary>
/// SamplerState Resource Struct
/// </summary>
/// 
/// - Shader 내부의 SamplerState Resource와 1:1 대응하는 Struct
/// - Shader에서 설정한 레지스터 번호와 버퍼가 일치하는 한개의 SamplerState
/// - D3DCompiler에서 현재 Shader에서 SamplerState을 선언해 두어도 사용하지 않으면
///   Register Slot에 올리지 않는다..
/// 
/// - 주의점
///  1) 현재 struct의 이름과 변수 순서는 Shader 내부의 SamplerState와 일치해야한다 (Struct Name을 통해 SamplerState를 찾기 때문)

///////////////////////////////////////////////////////////////////////////////////////////
// Global SamplerState Resource
///////////////////////////////////////////////////////////////////////////////////////////

SAMPLER_STATE(gShadowSam)
SAMPLER_STATE(samWrapMinLinear)
SAMPLER_STATE(samWrapAnisotropic)
SAMPLER_STATE(samClampMinLinear)
SAMPLER_STATE(samWrapLinerPoint)
SAMPLER_STATE(samBorderLinerPoint)
SAMPLER_STATE(samClampMinLinearPoint)
SAMPLER_STATE(samMirrorMinLinear)
