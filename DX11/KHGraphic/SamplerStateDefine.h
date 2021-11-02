#pragma once
#include "ClassType.h"

/// <summary>
/// SamplerState Resource Struct
/// </summary>
/// 
/// - Shader ������ SamplerState Resource�� 1:1 �����ϴ� Struct
/// - Shader���� ������ �������� ��ȣ�� ���۰� ��ġ�ϴ� �Ѱ��� SamplerState
/// - D3DCompiler���� ���� Shader���� SamplerState�� ������ �ξ ������� ������
///   Register Slot�� �ø��� �ʴ´�..
/// 
/// - ������
///  1) ���� struct�� �̸��� ���� ������ Shader ������ SamplerState�� ��ġ�ؾ��Ѵ� (Struct Name�� ���� SamplerState�� ã�� ����)

///////////////////////////////////////////////////////////////////////////////////////////
// Global SamplerState Resource
///////////////////////////////////////////////////////////////////////////////////////////

struct gShadowSam : public ClassType<gShadowSam> {};
struct samWrapMinLinear : public ClassType<samWrapMinLinear> {};
struct samWrapAnisotropic : public ClassType<samWrapAnisotropic> {};
struct samClampMinLinear : public ClassType<samClampMinLinear> {};
struct samWrapLinerPoint : public ClassType<samWrapLinerPoint> {};
struct samBorderLinerPoint : public ClassType<samBorderLinerPoint> {};
struct samClampMinLinearPoint : public ClassType<samClampMinLinearPoint> {};
struct samMirrorMinLinear : public ClassType<samMirrorMinLinear> {};