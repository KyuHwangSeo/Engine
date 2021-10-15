#include "MathDLL.h"
#include <math.h>
#include "SimpleMath.h"
#include "DXMath.h"

////////////////////////////////////////////////////////////////////////////////////////////
// Vector2 Struct
////////////////////////////////////////////////////////////////////////////////////////////

DXVector2::DXVector2()
	: x(0), y(0)
{

}

DXVector2::DXVector2(const DirectX::XMFLOAT2& xmf2)
	: x(xmf2.x), y(xmf2.y)
{

}

DXVector2::DXVector2(const DirectX::SimpleMath::Vector2& v2)
	: x(v2.x), y(v2.y)
{

}

DXVector2::DXVector2(float _x, float _y)
	: x(_x), y(_y)
{

}

bool DXVector2::operator==(const DXVector2& v2) const noexcept
{
	if (x == v2.x && y == v2.y)
		return true;
	else
		return false;
}

bool DXVector2::operator!=(const DXVector2& v2) const noexcept
{
	if (x == v2.x && y == v2.y)
		return false;
	else
		return true;
}

DXVector2& DXVector2::operator*=(const DXVector2& v2) noexcept
{
	x *= v2.x;
	y *= v2.y;

	return *this;
}

DXVector2& DXVector2::operator*=(const float f) noexcept
{
	x *= f;
	y *= f;

	return *this;
}

DXVector2& DXVector2::operator+=(const DXVector2& v2) noexcept
{
	x += v2.x;
	y += v2.y;

	return *this;
}

DXVector2& DXVector2::operator=(const DirectX::SimpleMath::Vector2& v2) noexcept
{
	x = v2.x;
	y = v2.y;

	return *this;
}

DXVector2& DXVector2::operator=(const DirectX::XMFLOAT2& xmf2) noexcept
{
	x = xmf2.x;
	y = xmf2.y;

	return *this;
}

DXVector2 DXVector2::operator*(const DXVector2& v2) noexcept
{
	return DXVector2(x * v2.x, y * v2.y);
}

DXVector2 DXVector2::operator*(const float f) noexcept
{
	return DXVector2(x * f, y * f);
}

DXVector2 DXVector2::operator+(const DXVector2& v2) noexcept
{
	return DXVector2(x + v2.x, y + v2.y);
}

DXVector2 DXVector2::operator-(const DXVector2& v2) noexcept
{
	return DXVector2(x - v2.x, y - v2.y);
}

DirectX::XMFLOAT2 DXVector2::ConvertXMFLOAT2()
{
	return DirectX::SimpleMath::Vector2(x, y);
}

DirectX::SimpleMath::Vector2 DXVector2::ConvertVector2()
{
	return DirectX::XMVECTOR{ x, y };
}

void DXVector2::Rotate(float angle) noexcept
{
	DXVector2 temp;
	temp.x = x * cosf(angle) - y * sinf(angle);
	temp.y = x * sinf(angle) + y * cosf(angle);
	
	x = temp.x;
	y = temp.y;
}

DXVector2 DXVector2::Zero()
{
	return DXVector2(0.0f, 0.0f);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Vector3 Struct
////////////////////////////////////////////////////////////////////////////////////////////

DXVector3::DXVector3()
{
	x = 0; y = 0; z = 0;
}

DXVector3::DXVector3(const DXVector4& v4)
	: x(v4.x), y(v4.y), z(v4.z)
{

}

DXVector3::DXVector3(const DirectX::XMFLOAT3& xmf3)
	: x(xmf3.x), y(xmf3.y), z(xmf3.z)
{

}

DXVector3::DXVector3(const DirectX::XMFLOAT4& xmf4)
	: x(xmf4.x), y(xmf4.y), z(xmf4.z)
{

}

DXVector3::DXVector3(const DirectX::XMVECTOR& xmv)
	: x(xmv.m128_f32[0]), y(xmv.m128_f32[1]), z(xmv.m128_f32[2])
{

}

DXVector3::DXVector3(const DirectX::SimpleMath::Vector3& v3)
	: x(v3.x), y(v3.y), z(v3.z)
{

}

DXVector3::DXVector3(const DirectX::SimpleMath::Vector4& v4)
	: x(v4.x), y(v4.y), z(v4.z)
{

}

DXVector3::DXVector3(float _x, float _y, float _z)
	: x(_x), y(_y), z(_z)
{

}

DXVector3::DXVector3(float _x, float _y)
	: x(_x), y(_y), z(0.0f)
{

}

bool DXVector3::operator!=(const DXVector3& v3) const noexcept
{
	if (x == v3.x && y == v3.y && z == v3.z)
	{
		return false;
	}

	return true;
}

bool DXVector3::operator==(const DXVector3& v3) const noexcept
{
	if (x == v3.x && y == v3.y && z == v3.z)
	{
		return true;
	}

	return false;
}

DXVector3& DXVector3::operator=(const DirectX::XMVECTOR& xmv) noexcept
{
	x = xmv.m128_f32[0];
	y = xmv.m128_f32[1];
	z = xmv.m128_f32[2];

	return *this;
}

DXVector3& DXVector3::operator=(const DirectX::XMFLOAT3& xmf3) noexcept
{
	x = xmf3.x;
	y = xmf3.y;
	z = xmf3.z;

	return *this;
}

DXVector3& DXVector3::operator=(const DirectX::SimpleMath::Vector3& v3) noexcept
{
	x = v3.x;
	y = v3.y;
	z = v3.z;

	return *this;
}

DXVector3& DXVector3::operator=(const DirectX::SimpleMath::Vector4& v4) noexcept
{
	x = v4.x;
	y = v4.y;
	z = v4.z;

	return *this;
}

DXVector3& DXVector3::operator+=(const DXVector3& v3) noexcept
{
	x += v3.x;
	y += v3.y;
	z += v3.z;

	return *this;
}

DXVector3& DXVector3::operator*=(const DXVector3& v3) noexcept
{
	x *= v3.x;
	y *= v3.y;
	z *= v3.z;

	return *this;
}

DXVector3& DXVector3::operator*=(const float f) noexcept
{
	x *= f;
	y *= f;
	z *= f;

	return *this;
}

DXVector3 DXVector3::operator+(const DXVector3& v3) noexcept
{
	return DXVector3(x + v3.x, y + v3.y, z + v3.z);
}

DXVector3 DXVector3::operator-(const DXVector3& v3) noexcept
{
	return DXVector3(x - v3.x, y - v3.y, z - v3.z);
}

DXVector3 DXVector3::operator*(const DXVector3& v3) noexcept
{
	return DXVector3(x * v3.x, y * v3.y, z * v3.z);
}


DXVector3 DXVector3::operator*(const float f) noexcept
{
	return DXVector3(x * f, y * f, z * f);
}

DXVector3 DXVector3::Normalize() noexcept
{
	float length = sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));

	x /= length;
	y /= length;
	z /= length;

	return *this;
}

DXVector3 DXVector3::Clamp(const DXVector3& v3min, const DXVector3& v3max) noexcept
{
	*this = KH_MATH::DXVector3Clamp(*this, v3min, v3max);

	return *this;
}

DXVector3 DXVector3::Cross(const DXVector3& v3) noexcept
{
	return DXVector3(y * v3.z - z * v3.y, z * v3.x - x * v3.z, x * v3.y - y * v3.x);
}

float DXVector3::Dot(const DXVector3& v3) noexcept
{
	return (x * v3.x) + (y * v3.y) + (z * v3.z);
}

DirectX::SimpleMath::Vector4 DXVector3::ConvertVector4()
{
	return DirectX::SimpleMath::Vector4(x, y, z, 0.0f);
}

DirectX::XMVECTOR DXVector3::ConvertXMVECTOR()
{
	return DirectX::XMVECTOR{ x, y, z, 0.0f };
}

DirectX::XMFLOAT4 DXVector3::ConvertXMFLOAT4()
{
	return DirectX::XMFLOAT4(x, y, z, 0.0f);
}

DirectX::XMFLOAT3 DXVector3::ConvertXMFLOAT3()
{
	return DirectX::XMFLOAT3(x, y, z);
}

DXVector3 DXVector3::Zero()
{
	return DXVector3(0.0f, 0.0f, 0.0f);
}

DXVector3 DXVector3::One()
{
	return DXVector3(1.0f, 1.0f, 1.0f);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Vector4 Struct
////////////////////////////////////////////////////////////////////////////////////////////

DXVector4::DXVector4()
{
	x = 0; y = 0; z = 0; w = 0;
}

DXVector4::DXVector4(float _x, float _y, float _z, float _w)
	:x(_x), y(_y), z(_z), w(_w)
{

}

DXVector4::DXVector4(const DXVector3& v3)
	: x(v3.x), y(v3.y), z(v3.z), w(0.0f)
{

}

DXVector4::DXVector4(const DirectX::XMFLOAT3& xmf3)
	: x(xmf3.x), y(xmf3.y), z(xmf3.z), w(0.0f)
{

}

DXVector4::DXVector4(const DirectX::XMFLOAT4& xmf4)
	: x(xmf4.x), y(xmf4.y), z(xmf4.z), w(xmf4.w)
{

}

DXVector4::DXVector4(const DirectX::XMVECTOR& xmv)
	: x(xmv.m128_f32[0]), y(xmv.m128_f32[1]), z(xmv.m128_f32[2]), w(xmv.m128_f32[3])
{

}

DXVector4::DXVector4(const DirectX::SimpleMath::Vector3& v3)
	: x(v3.x), y(v3.y), z(v3.z), w(0.0f)
{

}

DXVector4::DXVector4(const DirectX::SimpleMath::Vector4& v4)
	: x(v4.x), y(v4.y), z(v4.z), w(v4.w)
{

}

bool DXVector4::operator!=(const DXVector4& v4) const 
{
	if (x == v4.x && y == v4.y && z == v4.z && w == v4.w)
	{
		return false;
	}

	return true;
}

bool DXVector4::operator==(const DXVector4& v4) const 
{
	if (x == v4.x && y == v4.y && z == v4.z && w == v4.w)
	{
		return true;
	}

	return false;
}

DXVector4& DXVector4::operator=(const DXVector3& v3) 
{
	x = v3.x;
	y = v3.y;
	z = v3.z;

	return *this;
}

DXVector4& DXVector4::operator=(const DirectX::XMVECTOR& xmv)
{
	x = xmv.m128_f32[0];
	y = xmv.m128_f32[1];
	z = xmv.m128_f32[2];
	w = xmv.m128_f32[3];

	return *this;
}

DXVector4& DXVector4::operator=(const DirectX::XMFLOAT4& xmf4)
{
	x = xmf4.x;
	y = xmf4.y;
	z = xmf4.z;
	w = xmf4.w;

	return *this;
}

DXVector4& DXVector4::operator*=(const float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;

	return *this;
}

DXVector4 DXVector4::operator+(const DXVector4& v4)
{
	return DXVector4(x + v4.x, y + v4.y, z + v4.z, w + v4.w);
}

DXVector4 DXVector4::operator+(const DXVector3& v3)
{
	return DXVector4(x + v3.x, y + v3.y, z + v3.z, w);
}

DXVector4 DXVector4::operator-(const DXVector4& v4)
{
	return DXVector4(x - v4.x, y - v4.y, z - v4.z, w - v4.w);
}

DXVector4 DXVector4::operator-(const DXVector3& v3)
{
	return DXVector4(x - v3.x, y - v3.y, z - v3.z, w);
}

DXVector4 DXVector4::operator*(const DXVector4& v4)
{
	return DXVector4(x * v4.x, y * v4.y, z * v4.z, w * v4.w);
}

DXVector4 DXVector4::Normalize3() noexcept
{
	float length = sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));

	x /= length;
	y /= length;
	z /= length;
	w = 0;

	return *this;
}

DXVector4 DXVector4::Normalize() noexcept
{
	float length = sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2) + powf(w, 2));

	x /= length;
	y /= length;
	z /= length;
	w /= length;

	return *this;
}

DXVector4 DXVector4::Clamp(const DXVector4& v4min, const DXVector4& v4max) noexcept
{
	*this = KH_MATH::DXVector4Clamp(*this, v4min, v4max);

	return *this;
}

DXVector4 DXVector4::Cross(const DXVector4& v4) noexcept
{
	return DXVector4(y * v4.z - z * v4.y, z * v4.x - x * v4.z, x * v4.y - y * v4.x, 0.0f);
}

float DXVector4::Dot(const DXVector4& v4) noexcept
{
	return (x * v4.x) + (y * v4.y) + (z * v4.z);
}

DirectX::XMVECTOR DXVector4::ConvertXMVECTOR()
{
	return DirectX::XMVECTOR{ x, y, z, w };
}

DirectX::XMFLOAT4 DXVector4::ConvertXMFLOAT4()
{
	return DirectX::XMFLOAT4(x, y, z, w);
}

DirectX::XMFLOAT3 DXVector4::ConvertXMFLOAT3()
{
	return DirectX::XMFLOAT3(x, y, z);
}

DXVector4 DXVector4::Zero()
{
	return DXVector4(0.0f, 0.0f, 0.0f, 0.0f);
}

DXVector4 DXVector4::One()
{
	return DXVector4(1.0f, 1.0f, 1.0f, 1.0f);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Matrix4X4 Struct
////////////////////////////////////////////////////////////////////////////////////////////

DXMatrix4X4::DXMatrix4X4()
	:_11(1.0f), _12(0.0f), _13(0.0f), _14(0.0f),
	_21(0.0f), _22(1.0f), _23(0.0f), _24(0.0f),
	_31(0.0f), _32(0.0f), _33(1.0f), _34(0.0f),
	_41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f)
{

}

DXMatrix4X4::DXMatrix4X4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
	:_11(m11), _12(m12), _13(m13), _14(m14),
	_21(m21), _22(m22), _23(m23), _24(m24),
	_31(m31), _32(m32), _33(m33), _34(m34),
	_41(m41), _42(m42), _43(m43), _44(m44)
{

}

DXMatrix4X4::DXMatrix4X4(const DXVector4& v1, const DXVector4& v2, const DXVector4& v3, const DXVector4& v4)
	:_11(v1.x), _12(v1.y), _13(v1.z), _14(v1.w),
	_21(v2.x), _22(v2.y), _23(v2.z), _24(v2.w),
	_31(v3.x), _32(v3.y), _33(v3.z), _34(v3.w),
	_41(v4.x), _42(v4.y), _43(v4.z), _44(v4.w)
{

}

DXMatrix4X4::DXMatrix4X4(const DXVector4& v1, const DXVector4& v2, const DXVector4& v3)
	:_11(v1.x), _12(v1.y), _13(v1.z), _14(v1.w),
	_21(v2.x), _22(v2.y), _23(v2.z), _24(v2.w),
	_31(v3.x), _32(v3.y), _33(v3.z), _34(v3.w),
	_41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f)
{

}

DXMatrix4X4::DXMatrix4X4(const DXVector3& v1, const DXVector3& v2, const DXVector3& v3)
	:_11(v1.x), _12(v1.y), _13(v1.z), _14(0.0f),
	_21(v2.x), _22(v2.y), _23(v2.z), _24(0.0f),
	_31(v3.x), _32(v3.y), _33(v3.z), _34(0.0f),
	_41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f)
{

}


DXMatrix4X4::DXMatrix4X4(const DirectX::XMFLOAT4X4& xmf)
	:_11(xmf._11), _12(xmf._12), _13(xmf._13), _14(xmf._14),
	_21(xmf._21), _22(xmf._22), _23(xmf._23), _24(xmf._24),
	_31(xmf._31), _32(xmf._32), _33(xmf._33), _34(xmf._34),
	_41(xmf._41), _42(xmf._42), _43(xmf._43), _44(xmf._44)
{

}

DXMatrix4X4::DXMatrix4X4(const DirectX::XMMATRIX& xmm)
	:_11(xmm.r[0].m128_f32[0]), _12(xmm.r[0].m128_f32[1]), _13(xmm.r[0].m128_f32[2]), _14(xmm.r[0].m128_f32[3]),
	_21(xmm.r[1].m128_f32[0]), _22(xmm.r[1].m128_f32[1]), _23(xmm.r[1].m128_f32[2]), _24(xmm.r[1].m128_f32[3]),
	_31(xmm.r[2].m128_f32[0]), _32(xmm.r[2].m128_f32[1]), _33(xmm.r[2].m128_f32[2]), _34(xmm.r[2].m128_f32[3]),
	_41(xmm.r[3].m128_f32[0]), _42(xmm.r[3].m128_f32[1]), _43(xmm.r[3].m128_f32[2]), _44(xmm.r[3].m128_f32[3])
{

}

DXMatrix4X4::DXMatrix4X4(const DirectX::SimpleMath::Matrix& mx)
	:_11(mx._11), _12(mx._12), _13(mx._13), _14(mx._14),
	_21(mx._21), _22(mx._22), _23(mx._23), _24(mx._24),
	_31(mx._31), _32(mx._32), _33(mx._33), _34(mx._34),
	_41(mx._41), _42(mx._42), _43(mx._43), _44(mx._44)
{

}

bool DXMatrix4X4::operator==(const DXMatrix4X4& dxm) const 
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m[i][j] != dxm.m[i][j]) return false;
		}
	}

	return true;
}

DXMatrix4X4& DXMatrix4X4::operator=(const DirectX::XMFLOAT4X4& xmf)
{
	_11 = xmf._11;	_12 = xmf._12;	_13 = xmf._13;	_14 = xmf._14;
	_21 = xmf._21;	_22 = xmf._22;	_23 = xmf._23;	_24 = xmf._24;
	_31 = xmf._31;	_32 = xmf._32;	_33 = xmf._33;	_34 = xmf._34;
	_41 = xmf._41;	_42 = xmf._42;	_43 = xmf._43;	_44 = xmf._44;

	return *this;
}

DXMatrix4X4& DXMatrix4X4::operator=(const DirectX::XMMATRIX& xmm)
{
	_11 = xmm.r[0].m128_f32[0]; _12 = xmm.r[0].m128_f32[1]; _13 = xmm.r[0].m128_f32[2]; _14 = xmm.r[0].m128_f32[3];
	_21 = xmm.r[1].m128_f32[0]; _22 = xmm.r[1].m128_f32[1]; _23 = xmm.r[1].m128_f32[2]; _24 = xmm.r[1].m128_f32[3];
	_31 = xmm.r[2].m128_f32[0]; _32 = xmm.r[2].m128_f32[1]; _33 = xmm.r[2].m128_f32[2]; _34 = xmm.r[2].m128_f32[3];
	_41 = xmm.r[3].m128_f32[0]; _42 = xmm.r[3].m128_f32[1]; _43 = xmm.r[3].m128_f32[2]; _44 = xmm.r[3].m128_f32[3];

	return *this;
}

DXMatrix4X4& DXMatrix4X4::operator=(const DirectX::SimpleMath::Matrix& mx)
{
	_11 = mx._11;	_12 = mx._12;	_13 = mx._13;	_14 = mx._14;
	_21 = mx._21;	_22 = mx._22;	_23 = mx._23;	_24 = mx._24;
	_31 = mx._31;	_32 = mx._32;	_33 = mx._33;	_34 = mx._34;
	_41 = mx._41;	_42 = mx._42;	_43 = mx._43;	_44 = mx._44;

	return *this;
}

DXMatrix4X4& DXMatrix4X4::operator+=(const DXMatrix4X4& dxm)
{
	_11 = _11 + dxm._11;	_12 = _12 + dxm._12;	_13 = _13 + dxm._13;	_14 = _14 + dxm._14;
	_21 = _21 + dxm._21;	_22 = _22 + dxm._22;	_23 = _23 + dxm._23;	_24 = _24 + dxm._24;
	_31 = _31 + dxm._31;	_32 = _32 + dxm._32;	_33 = _33 + dxm._33;	_34 = _34 + dxm._34;
	_41 = _41 + dxm._41;	_42 = _42 + dxm._42;	_43 = _43 + dxm._43;	_44 = _44 + dxm._44;

	return *this;
}

DXMatrix4X4& DXMatrix4X4::operator-=(const DXMatrix4X4& dxm)
{
	_11 = _11 - dxm._11;	_12 = _12 - dxm._12;	_13 = _13 - dxm._13;	_14 = _14 - dxm._14;
	_21 = _21 - dxm._21;	_22 = _22 - dxm._22;	_23 = _23 - dxm._23;	_24 = _24 - dxm._24;
	_31 = _31 - dxm._31;	_32 = _32 - dxm._32;	_33 = _33 - dxm._33;	_34 = _34 - dxm._34;
	_41 = _41 - dxm._41;	_42 = _42 - dxm._42;	_43 = _43 - dxm._43;	_44 = _44 - dxm._44;

	return *this;
}

DXMatrix4X4& DXMatrix4X4::operator*=(const DXMatrix4X4& dxm)
{
	DXMatrix4X4 result;

	result._11 = _11 * dxm._11 + _12 * dxm._21 + _13 * dxm._31 + _14 * dxm._41;
	result._12 = _11 * dxm._12 + _12 * dxm._22 + _13 * dxm._32 + _14 * dxm._42;
	result._13 = _11 * dxm._13 + _12 * dxm._23 + _13 * dxm._33 + _14 * dxm._43;
	result._14 = _11 * dxm._14 + _12 * dxm._24 + _13 * dxm._34 + _14 * dxm._44;

	result._21 = _21 * dxm._11 + _22 * dxm._21 + _23 * dxm._31 + _24 * dxm._41;
	result._22 = _21 * dxm._12 + _22 * dxm._22 + _23 * dxm._32 + _24 * dxm._42;
	result._23 = _21 * dxm._13 + _22 * dxm._23 + _23 * dxm._33 + _24 * dxm._43;
	result._24 = _21 * dxm._14 + _22 * dxm._24 + _23 * dxm._34 + _24 * dxm._44;

	result._31 = _31 * dxm._11 + _32 * dxm._21 + _33 * dxm._31 + _34 * dxm._41;
	result._32 = _31 * dxm._12 + _32 * dxm._22 + _33 * dxm._32 + _34 * dxm._42;
	result._33 = _31 * dxm._13 + _32 * dxm._23 + _33 * dxm._33 + _34 * dxm._43;
	result._34 = _31 * dxm._14 + _32 * dxm._24 + _33 * dxm._34 + _34 * dxm._44;

	result._41 = _41 * dxm._11 + _42 * dxm._21 + _43 * dxm._31 + _44 * dxm._41;
	result._42 = _41 * dxm._12 + _42 * dxm._22 + _43 * dxm._32 + _44 * dxm._42;
	result._43 = _41 * dxm._13 + _42 * dxm._23 + _43 * dxm._33 + _44 * dxm._43;
	result._44 = _41 * dxm._14 + _42 * dxm._24 + _43 * dxm._34 + _44 * dxm._44;

	*this = result;

	return *this;
}

DXMatrix4X4 DXMatrix4X4::operator+(const DXMatrix4X4& dxm)
{
	return DXMatrix4X4(_11 + dxm._11, _12 + dxm._12, _13 + dxm._13, _14 + dxm._14,
		_21 + dxm._21, _22 + dxm._22, _23 + dxm._23, _24 + dxm._24,
		_31 + dxm._31, _32 + dxm._32, _33 + dxm._33, _34 + dxm._34,
		_41 + dxm._41, _42 + dxm._42, _43 + dxm._43, _44 + dxm._44);
}

DXMatrix4X4 DXMatrix4X4::operator-(const DXMatrix4X4& dxm)
{
	return DXMatrix4X4(_11 - dxm._11, _12 - dxm._12, _13 - dxm._13, _14 - dxm._14,
		_21 - dxm._21, _22 - dxm._22, _23 - dxm._23, _24 - dxm._24,
		_31 - dxm._31, _32 - dxm._32, _33 - dxm._33, _34 - dxm._34,
		_41 - dxm._41, _42 - dxm._42, _43 - dxm._43, _44 - dxm._44);
}

DXMatrix4X4 DXMatrix4X4::operator*(const DXMatrix4X4& dxm)
{
	DXMatrix4X4 result;

	result._11 = _11 * dxm._11 + _12 * dxm._21 + _13 * dxm._31 + _14 * dxm._41;
	result._12 = _11 * dxm._12 + _12 * dxm._22 + _13 * dxm._32 + _14 * dxm._42;
	result._13 = _11 * dxm._13 + _12 * dxm._23 + _13 * dxm._33 + _14 * dxm._43;
	result._14 = _11 * dxm._14 + _12 * dxm._24 + _13 * dxm._34 + _14 * dxm._44;

	result._21 = _21 * dxm._11 + _22 * dxm._21 + _23 * dxm._31 + _24 * dxm._41;
	result._22 = _21 * dxm._12 + _22 * dxm._22 + _23 * dxm._32 + _24 * dxm._42;
	result._23 = _21 * dxm._13 + _22 * dxm._23 + _23 * dxm._33 + _24 * dxm._43;
	result._24 = _21 * dxm._14 + _22 * dxm._24 + _23 * dxm._34 + _24 * dxm._44;

	result._31 = _31 * dxm._11 + _32 * dxm._21 + _33 * dxm._31 + _34 * dxm._41;
	result._32 = _31 * dxm._12 + _32 * dxm._22 + _33 * dxm._32 + _34 * dxm._42;
	result._33 = _31 * dxm._13 + _32 * dxm._23 + _33 * dxm._33 + _34 * dxm._43;
	result._34 = _31 * dxm._14 + _32 * dxm._24 + _33 * dxm._34 + _34 * dxm._44;

	result._41 = _41 * dxm._11 + _42 * dxm._21 + _43 * dxm._31 + _44 * dxm._41;
	result._42 = _41 * dxm._12 + _42 * dxm._22 + _43 * dxm._32 + _44 * dxm._42;
	result._43 = _41 * dxm._13 + _42 * dxm._23 + _43 * dxm._33 + _44 * dxm._43;
	result._44 = _41 * dxm._14 + _42 * dxm._24 + _43 * dxm._34 + _44 * dxm._44;

	return result;
}

DXMatrix4X4 DXMatrix4X4::operator*(const DirectX::XMFLOAT4X4& xmf)
{
	DXMatrix4X4 temp = xmf;
	return *this * temp;
}

DXMatrix4X4 DXMatrix4X4::operator*(const DirectX::XMMATRIX& xmm)
{
	DXMatrix4X4 temp = xmm;
	return *this * temp;
}

DXMatrix4X4 DXMatrix4X4::operator*(const DirectX::SimpleMath::Matrix& mx)
{
	DXMatrix4X4 temp = mx;
	return *this * temp;
}

DXMatrix4X4 DXMatrix4X4::Inverse()
{
	DirectX::XMMATRIX M = *this;
	DirectX::XMVECTOR det;
	DXMatrix4X4 result = XMMatrixInverse(&det, M);

	return result;
}

DXVector4 DXMatrix4X4::GetRow(int row)
{
	return DXVector4(m[row][0], m[row][1], m[row][2], m[row][3]);
}

DirectX::SimpleMath::Matrix DXMatrix4X4::ConvertMatrix()
{
	return DirectX::SimpleMath::Matrix(_11, _12, _13, _14,
		_21, _22, _23, _24,
		_31, _32, _33, _34,
		_41, _42, _43, _44);
}

DirectX::XMFLOAT4X4 DXMatrix4X4::ConvertXMFLOAT4X4()
{
	return DirectX::XMFLOAT4X4(_11, _12, _13, _14,
		_21, _22, _23, _24,
		_31, _32, _33, _34,
		_41, _42, _43, _44);
}

DirectX::XMMATRIX DXMatrix4X4::ConvertXMMATRIX()
{
	return DirectX::XMMATRIX(_11, _12, _13, _14,
		_21, _22, _23, _24,
		_31, _32, _33, _34,
		_41, _42, _43, _44);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Quaternion Struct
////////////////////////////////////////////////////////////////////////////////////////////

DXQuaternion::DXQuaternion()
	: DXVector4(0.0f, 0.0f, 0.0f, 1.0f)
{

}

DXQuaternion::DXQuaternion(const DXVector4& v4)
{
	x = v4.x;
	y = v4.y;
	z = v4.z;
	w = v4.w;
}

bool DXQuaternion::operator!=(const DXQuaternion& q) const
{
	DirectX::XMVECTOR q1 = KH_MATH::XMStoreFloat4(*this);
	DirectX::XMVECTOR q2 = KH_MATH::XMStoreFloat4(q);

	return DirectX::XMQuaternionEqual(q1, q2);
}

bool DXQuaternion::operator==(const DXQuaternion& q) const
{
	DirectX::XMVECTOR q1 = KH_MATH::XMStoreFloat4(*this);
	DirectX::XMVECTOR q2 = KH_MATH::XMStoreFloat4(q);

	return DirectX::XMQuaternionNotEqual(q1, q2);
}

DXQuaternion& DXQuaternion::operator=(const DirectX::XMVECTORF32& vf) noexcept
{
	x = vf.f[0];
	y = vf.f[1];
	z = vf.f[2];
	w = vf.f[3];

	return *this;
}

DXQuaternion& DXQuaternion::operator=(const DirectX::XMVECTOR& v) noexcept
{
	x = v.m128_f32[0];
	y = v.m128_f32[1];
	z = v.m128_f32[2];
	w = v.m128_f32[3];

	return *this;
}

DXQuaternion& DXQuaternion::operator+=(const DXQuaternion& q) noexcept
{
	DirectX::XMVECTOR q1 = KH_MATH::XMStoreFloat4(*this);
	DirectX::XMVECTOR q2 = KH_MATH::XMStoreFloat4(q);
	*this = DirectX::XMVectorAdd(q1, q2);

	return *this;
}

DXQuaternion& DXQuaternion::operator-=(const DXQuaternion& q) noexcept
{
	DirectX::XMVECTOR q1 = KH_MATH::XMStoreFloat4(*this);
	DirectX::XMVECTOR q2 = KH_MATH::XMStoreFloat4(q);
	*this = DirectX::XMVectorSubtract(q1, q2);

	return *this;
}

DXQuaternion& DXQuaternion::operator*=(const DXQuaternion& q) noexcept
{
	DirectX::XMVECTOR q1 = KH_MATH::XMStoreFloat4(*this);
	DirectX::XMVECTOR q2 = KH_MATH::XMStoreFloat4(q);
	*this = DirectX::XMQuaternionMultiply(q1, q2);

	return *this;
}

DXQuaternion& DXQuaternion::operator*=(float S) noexcept
{
	DirectX::XMVECTOR q1 = KH_MATH::XMStoreFloat4(*this);
	*this = DirectX::XMVectorScale(q1, S);

	return *this;
}

DXQuaternion& DXQuaternion::operator/=(const DXQuaternion& q) noexcept
{
	DirectX::XMVECTOR q1 = KH_MATH::XMStoreFloat4(*this);
	DirectX::XMVECTOR q2 = KH_MATH::XMStoreFloat4(q);
	q2 = DirectX::XMQuaternionInverse(q2);
	*this = DirectX::XMQuaternionMultiply(q1, q2);

	return *this;
}

DXQuaternion DXQuaternion::operator-() const noexcept
{
	DirectX::XMVECTOR q = KH_MATH::XMStoreFloat4(*this);

	return DirectX::XMVectorNegate(q);
}

float DXQuaternion::Length() const noexcept
{
	DirectX::XMVECTOR q = KH_MATH::XMStoreFloat4(*this);

	return DirectX::XMVectorGetX(DirectX::XMQuaternionLength(q));
}

float DXQuaternion::LengthSquared() const noexcept
{
	DirectX::XMVECTOR q = KH_MATH::XMStoreFloat4(*this);

	return DirectX::XMVectorGetX(DirectX::XMQuaternionLengthSq(q));
}

void DXQuaternion::Normalize() noexcept
{
	DirectX::XMVECTOR q = KH_MATH::XMStoreFloat4(*this);

	*this = DirectX::XMQuaternionNormalize(q);
}

void DXQuaternion::Normalize(DXQuaternion& q) const noexcept
{
	DirectX::XMVECTOR result = KH_MATH::XMStoreFloat4(*this);

	result = DirectX::XMQuaternionNormalize(q);
}

void DXQuaternion::Conjugate() noexcept
{
	DirectX::XMVECTOR q = KH_MATH::XMStoreFloat4(*this);

	*this = DirectX::XMQuaternionConjugate(q);
}

void DXQuaternion::Conjugate(DXQuaternion& q) const noexcept
{
	DirectX::XMVECTOR result = KH_MATH::XMStoreFloat4(*this);

	q = DirectX::XMQuaternionConjugate(result);
}

void DXQuaternion::Inverse(DXQuaternion& q) const noexcept
{
	DirectX::XMVECTOR result = KH_MATH::XMStoreFloat4(*this);

	q = DirectX::XMQuaternionInverse(result);
}

float DXQuaternion::Dot(const DXQuaternion& q) const noexcept
{
	DirectX::XMVECTOR q1 = KH_MATH::XMStoreFloat4(*this);
	DirectX::XMVECTOR q2 = KH_MATH::XMStoreFloat4(q);

	return DirectX::XMVectorGetX(DirectX::XMQuaternionDot(q1, q2));
}

DXQuaternion DXQuaternion::CreateFromAxisAngle(const DXVector3& axis, float angle) noexcept
{
	DirectX::XMVECTOR v3 = KH_MATH::XMStoreFloat3(axis);

	return DirectX::XMQuaternionRotationAxis(v3, angle);
}

DXQuaternion DXQuaternion::CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept
{
	return DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
}

DXQuaternion DXQuaternion::CreateFromRotationMatrix(const DXMatrix4X4& M) noexcept
{
	DirectX::XMMATRIX xm = KH_MATH::XMStoreFloat4X4(M);

	return DirectX::XMQuaternionRotationMatrix(xm);
}

void DXQuaternion::Lerp(const DXQuaternion& q1, const DXQuaternion& q2, float t, DXQuaternion& result) noexcept
{
	DirectX::XMVECTOR Q0 = KH_MATH::XMStoreFloat4(q1);
	DirectX::XMVECTOR Q1 = KH_MATH::XMStoreFloat4(q2);

	DirectX::XMVECTOR dot = DirectX::XMVector4Dot(Q0, Q1);

	DirectX::XMVECTOR R;
	if (DirectX::XMVector4GreaterOrEqual(dot, DirectX::XMVectorZero()))
	{
		R = DirectX::XMVectorLerp(Q0, Q1, t);
	}
	else
	{
		DirectX::XMVECTOR tv = DirectX::XMVectorReplicate(t);
		DirectX::XMVECTOR t1v = DirectX::XMVectorReplicate(1.f - t);
		DirectX::XMVECTOR X0 = DirectX::XMVectorMultiply(Q0, t1v);
		DirectX::XMVECTOR X1 = DirectX::XMVectorMultiply(Q1, tv);
		R = DirectX::XMVectorSubtract(X0, X1);
	}

	result = DirectX::XMQuaternionNormalize(R);
}

DXQuaternion DXQuaternion::Lerp(const DXQuaternion& q1, const DXQuaternion& q2, float t) noexcept
{
	DirectX::XMVECTOR Q0 = KH_MATH::XMStoreFloat4(q1);
	DirectX::XMVECTOR Q1 = KH_MATH::XMStoreFloat4(q2);

	DirectX::XMVECTOR dot = DirectX::XMVector4Dot(Q0, Q1);

	DirectX::XMVECTOR R;
	if (DirectX::XMVector4GreaterOrEqual(dot, DirectX::XMVectorZero()))
	{
		R = DirectX::XMVectorLerp(Q0, Q1, t);
	}
	else
	{
		DirectX::XMVECTOR tv = DirectX::XMVectorReplicate(t);
		DirectX::XMVECTOR t1v = DirectX::XMVectorReplicate(1.f - t);
		DirectX::XMVECTOR X0 = DirectX::XMVectorMultiply(Q0, t1v);
		DirectX::XMVECTOR X1 = DirectX::XMVectorMultiply(Q1, tv);
		R = DirectX::XMVectorSubtract(X0, X1);
	}

	return DirectX::XMQuaternionNormalize(R);
}

void DXQuaternion::Slerp(const DXQuaternion& q1, const DXQuaternion& q2, float t, DXQuaternion& result) noexcept
{
	DirectX::XMVECTOR Q0 = KH_MATH::XMStoreFloat4(q1);
	DirectX::XMVECTOR Q1 = KH_MATH::XMStoreFloat4(q2);

	result = DirectX::XMQuaternionSlerp(Q0, Q1, t);
}

DXQuaternion DXQuaternion::Slerp(const DXQuaternion& q1, const DXQuaternion& q2, float t) noexcept
{
	DirectX::XMVECTOR Q0 = KH_MATH::XMStoreFloat4(q1);
	DirectX::XMVECTOR Q1 = KH_MATH::XMStoreFloat4(q2);

	return DirectX::XMQuaternionSlerp(Q0, Q1, t);
}

void DXQuaternion::Concatenate(const DXQuaternion& q1, const DXQuaternion& q2, DXQuaternion& result) noexcept
{
	DirectX::XMVECTOR Q0 = KH_MATH::XMStoreFloat4(q1);
	DirectX::XMVECTOR Q1 = KH_MATH::XMStoreFloat4(q2);

	result = DirectX::XMQuaternionMultiply(Q0, Q1);
}

DXQuaternion DXQuaternion::Concatenate(const DXQuaternion& q1, const DXQuaternion& q2) noexcept
{
	DirectX::XMVECTOR Q0 = KH_MATH::XMStoreFloat4(q1);
	DirectX::XMVECTOR Q1 = KH_MATH::XMStoreFloat4(q2);

	return DirectX::XMQuaternionMultiply(Q0, Q1);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Math Expansion Function
////////////////////////////////////////////////////////////////////////////////////////////

MATH_DLL DirectX::XMVECTOR KH_MATH::XMStoreFloat3(const DXVector3& _v3)
{
	DirectX::XMVECTOR resultV;
	resultV.m128_f32[0] = _v3.x;
	resultV.m128_f32[1] = _v3.y;
	resultV.m128_f32[2] = _v3.z;

	return resultV;
}

MATH_DLL DirectX::XMFLOAT3 KH_MATH::XMLoadFloat3(const DXVector3& _v3)
{
	DirectX::XMFLOAT3 resultF;
	resultF.x = _v3.x;
	resultF.y = _v3.y;
	resultF.z = _v3.z;

	return resultF;
}

MATH_DLL DirectX::XMVECTOR KH_MATH::XMStoreFloat4(const DXVector4& _v4)
{
	DirectX::XMVECTOR resultV;

	resultV.m128_f32[0] = _v4.x;
	resultV.m128_f32[1] = _v4.y;
	resultV.m128_f32[2] = _v4.z;
	resultV.m128_f32[3] = _v4.w;

	return resultV;
}

MATH_DLL DirectX::XMFLOAT4 KH_MATH::XMLoadFloat4(const DXVector4& _v4)
{
	DirectX::XMFLOAT4 resultF;

	resultF.x = _v4.x;
	resultF.y = _v4.y;
	resultF.z = _v4.z;
	resultF.w = _v4.w;

	return resultF;
}

MATH_DLL DirectX::XMMATRIX KH_MATH::XMStoreFloat4X4(const DXMatrix4X4& _dxm)
{
	DirectX::XMMATRIX resultM;

	resultM.r[0] = _dxm(0);
	resultM.r[1] = _dxm(1);
	resultM.r[2] = _dxm(2);
	resultM.r[3] = _dxm(3);

	return resultM;
}

MATH_DLL DXVector3 KH_MATH::BezierCurveThree(DXVector3& _p1, DXVector3& _p2, DXVector3& _p3, float t)
{
	DirectX::XMVECTOR t1 = DirectX::XMVectorReplicate((1 - t) * (1 - t));
	DirectX::XMVECTOR t2 = DirectX::XMVectorReplicate(2 * t * (1 - t));
	DirectX::XMVECTOR t3 = DirectX::XMVectorReplicate(t * t);

	DirectX::XMVECTOR result = DirectX::XMVectorMultiply(_p1, t1);
	result = DirectX::XMVectorMultiplyAdd(_p2, t2, result);
	result = DirectX::XMVectorMultiplyAdd(_p3, t3, result);

	return result;
}

MATH_DLL DXVector3 KH_MATH::BezierCurveFour(DXVector3& _p1, DXVector3& _p2, DXVector3& _p3, DXVector3& _p4, float t)
{
	DirectX::XMVECTOR t1 = DirectX::XMVectorReplicate((1 - t) * (1 - t) * (1 - t));
	DirectX::XMVECTOR t2 = DirectX::XMVectorReplicate(3 * t * (1 - t) * (1 - t));
	DirectX::XMVECTOR t3 = DirectX::XMVectorReplicate(3 * t * t * (1 - t));
	DirectX::XMVECTOR t4 = DirectX::XMVectorReplicate(t * t * t);

	DirectX::XMVECTOR result = DirectX::XMVectorMultiply(_p1, t1);
	result = DirectX::XMVectorMultiplyAdd(_p2, t2, result);
	result = DirectX::XMVectorMultiplyAdd(_p3, t3, result);
	result = DirectX::XMVectorMultiplyAdd(_p4, t4, result);

	return result;
}

MATH_DLL DXVector3 KH_MATH::GetMiddlePoint(DXVector3& _p1, DXVector3& _p2)
{
	DXVector3 resultF = { _p1.x * 2 - _p2.x,
						 _p1.y * 2 - _p2.y,
						 _p1.z * 2 - _p2.z };
	return resultF;
}

MATH_DLL DXMatrix4X4 KH_MATH::RotationAxis(DXVector3& _axis, float _angle)
{
	DXMatrix4X4 _result;

	float c = cosf(_angle);
	float s = sinf(_angle);
	float x = _axis.x;
	float y = _axis.y;
	float z = _axis.z;

	_result._11 = c + (1 - c) * powf(x, 2.0f);
	_result._12 = (1 - c) * x * y + s * z;
	_result._13 = (1 - c) * x * z - s * y;
	_result._14 = 0;

	_result._21 = (1 - c) * x * y - s * z;
	_result._22 = c + (1 - c) * powf(y, 2.0f);
	_result._23 = (1 - c) * y * z + s * x;
	_result._24 = 0;

	_result._31 = (1 - c) * x * z + s * y;
	_result._32 = (1 - c) * y * z - s * x;
	_result._33 = c + (1 - c) * powf(z, 2.0f);
	_result._34 = 0;

	_result._41 = 0;
	_result._42 = 0;
	_result._43 = 0;
	_result._44 = 1;

	return _result;
}

MATH_DLL float KH_MATH::GetDistance(DXVector3& _p1, DXVector3& _p2)
{
	return sqrtf(powf(_p1.x - _p2.x, 2) +
		powf(_p1.y - _p2.y, 2) +
		powf(_p1.z - _p2.z, 2));
}

MATH_DLL float KH_MATH::GetDistance(DXVector2& _p1, DXVector2& _p2)
{
	return sqrtf(powf(_p1.x - _p2.x, 2) +
		powf(_p1.y - _p2.y, 2));
}

MATH_DLL bool KH_MATH::CompareXMFLOAT3(DirectX::XMFLOAT3& _xmf1, DirectX::XMFLOAT3& _xmf2)
{
	if (_xmf1.x == _xmf2.x && _xmf1.y == _xmf2.y && _xmf1.z == _xmf2.z)
	{
		return true;
	}

	return false;
}

MATH_DLL DXMatrix4X4 KH_MATH::CreateScale(DXVector3 _v3)
{
	DXMatrix4X4 M;

	M._11 = _v3.x;
	M._22 = _v3.y;
	M._33 = _v3.z;

	return M;
}

MATH_DLL DXMatrix4X4 KH_MATH::CreateScale(float _x, float _y, float _z)
{
	DXMatrix4X4 M;

	M._11 = _x;
	M._22 = _y;
	M._33 = _z;

	return M;
}

MATH_DLL DXMatrix4X4 KH_MATH::CreateRotationEuler(DXVector3 _v3)
{
	const auto yaw = _v3.y * KH_MATH::Pi / 180.0f;
	const auto pitch = _v3.x * KH_MATH::Pi / 180.0f;
	const auto roll = _v3.z * KH_MATH::Pi / 180.0f;

	DirectX::SimpleMath::Quaternion q = DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

	return DirectX::XMMatrixRotationQuaternion(q);
}

MATH_DLL DXMatrix4X4 KH_MATH::CreateRotationEuler(float _x, float _y, float _z)
{
	const auto yaw = _y * KH_MATH::Pi / 180.0f;
	const auto pitch = _x * KH_MATH::Pi / 180.0f;
	const auto roll = _z * KH_MATH::Pi / 180.0f;

	DirectX::SimpleMath::Quaternion q = DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

	return DirectX::XMMatrixRotationQuaternion(q);
}

MATH_DLL DXMatrix4X4 KH_MATH::CreateTranslation(DXVector3 _v3)
{
	DXMatrix4X4 M;

	M._41 = _v3.x;
	M._42 = _v3.y;
	M._43 = _v3.z;

	return M;
}

MATH_DLL DXVector3 KH_MATH::DXVector3Max(const DXVector3& _v1, const DXVector3& _v2)
{
	return DXVector3((_v1.x > _v2.x) ? _v1.x : _v2.x,
					 (_v1.y > _v2.y) ? _v1.y : _v2.y,
					 (_v1.z > _v2.z) ? _v1.z : _v2.z);
}

MATH_DLL DXVector3 KH_MATH::DXVector3Min(const DXVector3& _v1, const DXVector3& _v2)
{
	return DXVector3((_v1.x < _v2.x) ? _v1.x : _v2.x,
					 (_v1.y < _v2.y) ? _v1.y : _v2.y,
					 (_v1.z < _v2.z) ? _v1.z : _v2.z);
}

MATH_DLL DXVector3 KH_MATH::DXVector3Clamp(const DXVector3& _v3, const DXVector3& _v3min, const DXVector3& _v3max)
{
	DXVector3 Result;

	Result = KH_MATH::DXVector3Max(_v3min, _v3);
	Result = KH_MATH::DXVector3Min(_v3max, Result);

	return Result;
}

MATH_DLL DXMatrix4X4 KH_MATH::CreateTranslation(float _x, float _y, float _z)
{
	DXMatrix4X4 M;

	M._41 = _x;
	M._42 = _y;
	M._43 = _z;

	return M;
}

MATH_DLL DXVector3 KH_MATH::DXVector3Dot(DXVector3& _v1, DXVector3& _v2)
{
	float fValue = (_v1.x * _v2.x) + (_v1.y * _v2.y) + (_v1.z * _v2.z);
	return DXVector3(fValue, fValue, fValue);
}

MATH_DLL DXVector3 KH_MATH::DXVector3Abs(DXVector3& _v3)
{
	return DXVector3(fabsf(_v3.x), fabsf(_v3.y), fabsf(_v3.z));
}

MATH_DLL DXVector3 KH_MATH::DXVector3Sqrt(DXVector3& _v3)
{
	return DXVector3(sqrtf(_v3.x), sqrtf(_v3.y), sqrtf(_v3.z));
}

MATH_DLL DXVector3 KH_MATH::DXVector3Length(DXVector3& _v3)
{
	DXVector3 Vdot = DXVector3Dot(_v3, _v3);

	return DXVector3Sqrt(Vdot);
}

MATH_DLL DXVector3 KH_MATH::DXVector3SplatOne(DXVector3& _v3)
{
	return DXVector3(1.0f, 1.0f, 1.0f);
}

MATH_DLL DXVector3 KH_MATH::DXVector3Replicate(float _value)
{
	return DXVector3(_value, _value, _value);
}

MATH_DLL bool KH_MATH::DXVector3Less(DXVector3& _v1, DXVector3& _v2)
{
	return (((_v1.x < _v2.x) && (_v1.y < _v2.y) && (_v1.z < _v1.z)) != 0);
}

MATH_DLL DXVector4 KH_MATH::DXVector4Max(const DXVector4& _v1, const DXVector4& _v2)
{
	return DXVector4((_v1.x > _v2.x) ? _v1.x : _v2.x,
		(_v1.y > _v2.y) ? _v1.y : _v2.y,
		(_v1.z > _v2.z) ? _v1.z : _v2.z,
		(_v1.w > _v2.w) ? _v1.w : _v2.w);
}

MATH_DLL DXVector4 KH_MATH::DXVector4Min(const DXVector4& _v1, const DXVector4& _v2)
{
	return DXVector4((_v1.x < _v2.x) ? _v1.x : _v2.x,
		(_v1.y < _v2.y) ? _v1.y : _v2.y,
		(_v1.z < _v2.z) ? _v1.z : _v2.z,
		(_v1.w < _v2.w) ? _v1.w : _v2.w);
}

MATH_DLL DXVector4 KH_MATH::DXVector4Clamp(const DXVector4& _v4, const DXVector4& _v4min, const DXVector4& _v4max)
{
	DXVector4 Result;

	Result = KH_MATH::DXVector4Max(_v4min, _v4);
	Result = KH_MATH::DXVector4Min(_v4max, Result);

	return Result;
}

MATH_DLL DXVector4 KH_MATH::DXVector4Abs(DXVector4& _v4)
{
	return DXVector4(fabsf(_v4.x), fabsf(_v4.y), fabsf(_v4.z), fabsf(_v4.w));
}

MATH_DLL DXVector4 KH_MATH::DXVector4Sqrt(DXVector4& _v4)
{
	return DXVector4(sqrtf(_v4.x), sqrtf(_v4.y), sqrtf(_v4.z), sqrtf(_v4.w));
}

MATH_DLL DXVector4 KH_MATH::DXVector4SplatOne(DXVector4& _v4)
{
	return DXVector4(1.0f, 1.0f, 1.0f, 1.0f);
}

MATH_DLL DXVector4 KH_MATH::DXVector4Replicate(float _value)
{
	return DXVector4(_value, _value, _value, _value);
}

MATH_DLL bool KH_MATH::DXVector4Less(DXVector4& _v1, DXVector4& _v2)
{
	return (((_v1.x < _v2.x) && (_v1.y < _v2.y) && (_v1.z < _v1.z) && (_v1.w < _v1.w)) != 0);
}