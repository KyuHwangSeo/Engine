#pragma once
#include "BufferNameDefine.h"

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

struct gShadowSam : public BufferName<gShadowSam> {};
struct samWrapMinLinear : public BufferName<samWrapMinLinear> {};
struct samWrapAnisotropic : public BufferName<samWrapAnisotropic> {};
struct samClampMinLinear : public BufferName<samClampMinLinear> {};