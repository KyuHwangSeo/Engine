#include "DLLDefine.h"
#include <math.h>
#include "SimpleMath.h"
using namespace DirectX;
using namespace SimpleMath;

#include "DXMath.h"

//////////////////////////////////////////////////////////////////////////
// DXVector2
//////////////////////////////////////////////////////////////////////////

DXVector2::DXVector2()
	: x(0), y(0)
{

}

DXVector2::DXVector2(const DXVector2& v2)
	: x(v2.x), y(v2.y)
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

bool DXVector2::operator==(const DXVector2& v2)
{
	if (x == v2.x && y == v2.y)
		return true;
	else
		return false;
}

bool DXVector2::operator!=(const DXVector2& v2)
{
	if (x == v2.x && y == v2.y)
		return false;
	else
		return true;
}

DXVector2& DXVector2::operator*=(const DXVector2& v2)
{
	x *= v2.x;
	y *= v2.y;

	return *this;
}

DXVector2& DXVector2::operator*=(const float f)
{
	x *= f;
	y *= f;

	return *this;
}

DXVector2& DXVector2::operator+=(const DXVector2& v2)
{
	x += v2.x;
	y += v2.y;

	return *this;
}

DXVector2& DXVector2::operator=(const DirectX::SimpleMath::Vector2& v2)
{
	x = v2.x;
	y = v2.y;

	return *this;
}

DXVector2& DXVector2::operator=(const DirectX::XMFLOAT2& xmf2)
{
	x = xmf2.x;
	y = xmf2.y;

	return *this;
}

DXVector2& DXVector2::operator=(const DXVector2& v2)
{
	x = v2.x;
	y = v2.y;

	return *this;
}

DXVector2 DXVector2::operator*(const DXVector2& v2)
{
	return DXVector2(x * v2.x, y * v2.y);
}

DXVector2 DXVector2::operator*(const float f)
{
	return DXVector2(x * f, y * f);
}

DXVector2 DXVector2::operator+(const DXVector2& v2)
{
	return DXVector2(x + v2.x, y + v2.y);
}

DXVector2 DXVector2::operator-(const DXVector2& v2)
{
	return DXVector2(x - v2.x, y - v2.y);
}

DirectX::XMFLOAT2 DXVector2::ConvertXMFLOAT2()
{
	return Vector2(x, y);
}

DirectX::SimpleMath::Vector2 DXVector2::ConvertVector2()
{
	return XMVECTOR{ x, y };

}

void DXVector2::Rotate(float angle)
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

//////////////////////////////////////////////////////////////////////////
// DXVector3
//////////////////////////////////////////////////////////////////////////

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

DXVector3::~DXVector3()
{

}

bool DXVector3::operator!=(const DXVector3& v3)
{
	if (x == v3.x && y == v3.y && z == v3.z)
	{
		return false;
	}

	return true;
}

bool DXVector3::operator==(const DXVector3& v3)
{
	if (x == v3.x && y == v3.y && z == v3.z)
	{
		return true;
	}

	return false;
}

DXVector3& DXVector3::operator=(const DirectX::XMVECTOR& xmv)
{
	x = xmv.m128_f32[0];
	y = xmv.m128_f32[1];
	z = xmv.m128_f32[2];

	return *this;
}

DXVector3& DXVector3::operator=(const DirectX::XMFLOAT3& xmf3)
{
	x = xmf3.x;
	y = xmf3.y;
	z = xmf3.z;

	return *this;
}

DXVector3& DXVector3::operator=(const DXVector3& v3)
{
	x = v3.x;
	y = v3.y;
	z = v3.z;

	return *this;
}

DXVector3& DXVector3::operator=(const DirectX::SimpleMath::Vector3& v3)
{
	x = v3.x;
	y = v3.y;
	z = v3.z;

	return *this;
}

DXVector3& DXVector3::operator=(const SimpleMath::Vector4& v4)
{
	x = v4.x;
	y = v4.y;
	z = v4.z;

	return *this;
}

DXVector3& DXVector3::operator+=(const DXVector3& v3)
{
	x += v3.x;
	y += v3.y;
	z += v3.z;

	return *this;
}

DXVector3& DXVector3::operator*=(const DXVector3& v3)
{
	x *= v3.x;
	y *= v3.y;
	z *= v3.z;

	return *this;
}

DXVector3& DXVector3::operator*=(const float f)
{
	x *= f;
	y *= f;
	z *= f;

	return *this;
}

DXVector3 DXVector3::operator+(const DXVector3& v3)
{
	return DXVector3(x + v3.x, y + v3.y, z + v3.z);
}

DXVector3 DXVector3::operator-(const DXVector3& v3)
{
	return DXVector3(x - v3.x, y - v3.y, z - v3.z);
}

DXVector3 DXVector3::operator*(const DXVector3& v3)
{
	return DXVector3(x * v3.x, y * v3.y, z * v3.z);
}


DXVector3 DXVector3::operator*(const float f)
{
	return DXVector3(x * f, y * f, z * f);
}

DXVector3 DXVector3::Normalize()
{
	float length = sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));

	x /= length;
	y /= length;
	z /= length;

	return *this;
}

DXVector3 DXVector3::Cross(const DXVector3& v3)
{
	return DXVector3(y * v3.z - z * v3.y, z * v3.x - x * v3.z, x * v3.y - y * v3.x);
}

float DXVector3::Dot(const DXVector3& v3)
{
	return (x * v3.x) + (y * v3.y) + (z * v3.z);
}

bool DXVector3::Equal(const DXVector3& v3)
{
	if (*this == v3)
	{
		return true;
	}

	return false;
}

DirectX::SimpleMath::Vector4 DXVector3::ConvertVector4()
{
	return Vector4(x, y, z, 0.0f);
}

DirectX::XMVECTOR DXVector3::ConvertXMVECTOR()
{
	return XMVECTOR{ x, y, z, 0.0f };
}

DirectX::XMFLOAT4 DXVector3::ConvertXMFLOAT4()
{
	return XMFLOAT4(x, y, z, 0.0f);
}

DirectX::XMFLOAT3 DXVector3::ConvertXMFLOAT3()
{
	return XMFLOAT3(x, y, z);
}

DXVector3 DXVector3::Zero()
{
	return DXVector3(0.0f, 0.0f, 0.0f);
}

DXVector3 DXVector3::One()
{
	return DXVector3(1.0f, 1.0f, 1.0f);
}

//////////////////////////////////////////////////////////////////////////
// DXVector4
//////////////////////////////////////////////////////////////////////////

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

DXVector4::~DXVector4()
{

}

bool DXVector4::operator!=(const DXVector4& v4)
{
	if (x == v4.x && y == v4.y && z == v4.z && w == v4.w)
	{
		return false;
	}

	return true;
}

bool DXVector4::operator==(const DXVector4& v4)
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

DXVector4& DXVector4::operator=(const DXVector4& v4)
{
	x = v4.x;
	y = v4.y;
	z = v4.z;
	w = v4.w;

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

DXVector4 DXVector4::Normalize3()
{
	float length = sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));

	x /= length;
	y /= length;
	z /= length;
	w = 0;

	return *this;
}

DXVector4 DXVector4::Normalize()
{
	float length = sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2) + powf(w, 2));

	x /= length;
	y /= length;
	z /= length;
	w /= length;

	return *this;
}

DXVector4 DXVector4::Cross(const DXVector4& v4)
{
	return DXVector4(y * v4.z - z * v4.y, z * v4.x - x * v4.z, x * v4.y - y * v4.x, 0.0f);
}

float DXVector4::Dot(const DXVector4& v4)
{
	return (x * v4.x) + (y * v4.y) + (z * v4.z);
}

bool DXVector4::Equal(const DXVector4& v4)
{
	if (*this == v4)
	{
		return true;
	}

	return false;
}

DirectX::XMVECTOR DXVector4::ConvertXMVECTOR()
{
	return XMVECTOR{ x, y, z, w };
}

DirectX::XMFLOAT4 DXVector4::ConvertXMFLOAT4()
{
	return XMFLOAT4(x, y, z, w);
}

DirectX::XMFLOAT3 DXVector4::ConvertXMFLOAT3()
{
	return XMFLOAT3(x, y, z);
}

DXVector4 DXVector4::Zero()
{
	return DXVector4(0.0f, 0.0f, 0.0f, 0.0f);
}

DXVector4 DXVector4::One()
{
	return DXVector4(1.0f, 1.0f, 1.0f, 1.0f);
}

//////////////////////////////////////////////////////////////////////////
// DXMatrix4X4
//////////////////////////////////////////////////////////////////////////

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

DXMatrix4X4::~DXMatrix4X4()
{

}

bool DXMatrix4X4::operator==(const DXMatrix4X4& dxm)
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

DXMatrix4X4& DXMatrix4X4::operator=(const DXMatrix4X4& dxm)
{
	_11 = dxm._11;	_12 = dxm._12;	_13 = dxm._13;	_14 = dxm._14;
	_21 = dxm._21;	_22 = dxm._22;	_23 = dxm._23;	_24 = dxm._24;
	_31 = dxm._31;	_32 = dxm._32;	_33 = dxm._33;	_34 = dxm._34;
	_41 = dxm._41;	_42 = dxm._42;	_43 = dxm._43;	_44 = dxm._44;

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
	XMMATRIX M = *this;
	XMVECTOR det;
	DXMatrix4X4 result = XMMatrixInverse(&det, M);

	return result;
}

DXVector4 DXMatrix4X4::GetRow(int row)
{
	return DXVector4(m[row][0], m[row][1], m[row][2], m[row][3]);
}

DirectX::SimpleMath::Matrix DXMatrix4X4::ConvertMatrix()
{
	return Matrix(_11, _12, _13, _14,
		_21, _22, _23, _24,
		_31, _32, _33, _34,
		_41, _42, _43, _44);
}

XMFLOAT4X4 DXMatrix4X4::ConvertXMFLOAT4X4()
{
	return XMFLOAT4X4(_11, _12, _13, _14,
		_21, _22, _23, _24,
		_31, _32, _33, _34,
		_41, _42, _43, _44);
}

XMMATRIX DXMatrix4X4::ConvertXMMATRIX()
{
	return XMMATRIX(_11, _12, _13, _14,
		_21, _22, _23, _24,
		_31, _32, _33, _34,
		_41, _42, _43, _44);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Math Expansion Function
////////////////////////////////////////////////////////////////////////////////////////////

KH_API XMVECTOR KH_MATH::XMStoreFloat3(XMFLOAT3 _xmf)
{
	XMVECTOR resultV;
	resultV.m128_f32[0] = _xmf.x;
	resultV.m128_f32[1] = _xmf.y;
	resultV.m128_f32[2] = _xmf.z;

	return resultV;
}

KH_API XMFLOAT3 KH_MATH::XMLoadFloat3(XMVECTOR _xmv)
{
	XMFLOAT3 resultF;
	resultF.x = _xmv.m128_f32[0];
	resultF.y = _xmv.m128_f32[1];
	resultF.z = _xmv.m128_f32[2];

	return resultF;
}

KH_API DXVector3 KH_MATH::BezierCurveThree(DXVector3 _p1, DXVector3 _p2, DXVector3 _p3, float t)
{
	XMVECTOR t1 = XMVectorReplicate((1 - t) * (1 - t));
	XMVECTOR t2 = XMVectorReplicate(2 * t * (1 - t));
	XMVECTOR t3 = XMVectorReplicate(t * t);

	XMVECTOR result = XMVectorMultiply(_p1, t1);
	result = XMVectorMultiplyAdd(_p2, t2, result);
	result = XMVectorMultiplyAdd(_p3, t3, result);

	return result;
}

KH_API DXVector3 KH_MATH::BezierCurveFour(DXVector3 _p1, DXVector3 _p2, DXVector3 _p3, DXVector3 _p4, float t)
{
	XMVECTOR t1 = XMVectorReplicate((1 - t) * (1 - t) * (1 - t));
	XMVECTOR t2 = XMVectorReplicate(3 * t * (1 - t) * (1 - t));
	XMVECTOR t3 = XMVectorReplicate(3 * t * t * (1 - t));
	XMVECTOR t4 = XMVectorReplicate(t * t * t);

	DXVector3 result = XMVectorMultiply(_p1, t1);
	result = XMVectorMultiplyAdd(_p2, t2, result);
	result = XMVectorMultiplyAdd(_p3, t3, result);
	result = XMVectorMultiplyAdd(_p4, t4, result);

	return result;
}

KH_API DXVector3 KH_MATH::GetMiddlePoint(DXVector3 _p1, DXVector3 _p2)
{
	DXVector3 resultF = { _p1.x * 2 - _p2.x,
						 _p1.y * 2 - _p2.y,
						 _p1.z * 2 - _p2.z };
	return resultF;
}

KH_API DXMatrix4X4 KH_MATH::RotationAxis(DXVector3 _axis, float _angle)
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

KH_API float KH_MATH::GetDistance(DXVector3 _p1, DXVector3 _p2)
{
	return sqrtf(powf(_p1.x - _p2.x, 2) +
		powf(_p1.y - _p2.y, 2) +
		powf(_p1.z - _p2.z, 2));
}

KH_API float KH_MATH::GetDistance(DXVector2 _p1, DXVector2 _p2)
{
	return sqrtf(powf(_p1.x - _p2.x, 2) +
		powf(_p1.y - _p2.y, 2));
}

KH_API bool KH_MATH::CompareXMFLOAT3(XMFLOAT3 _xmf1, XMFLOAT3 _xmf2)
{
	if (_xmf1.x == _xmf2.x && _xmf1.y == _xmf2.y && _xmf1.z == _xmf2.z)
	{
		return true;
	}

	return false;
}

KH_API DXMatrix4X4 KH_MATH::CreateScale(DXVector3 _v3)
{
	DXMatrix4X4 M;

	M._11 = _v3.x;
	M._22 = _v3.y;
	M._33 = _v3.z;

	return M;
}

KH_API DXMatrix4X4 KH_MATH::CreateScale(float _x, float _y, float _z)
{
	DXMatrix4X4 M;

	M._11 = _x;
	M._22 = _y;
	M._33 = _z;

	return M;
}

KH_API DXMatrix4X4 KH_MATH::CreateRotationEuler(DXVector3 _v3)
{
	const auto yaw = _v3.y * KH_MATH::Pi / 180.0f;
	const auto pitch = _v3.x * KH_MATH::Pi / 180.0f;
	const auto roll = _v3.z * KH_MATH::Pi / 180.0f;

	Quaternion q = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

	return XMMatrixRotationQuaternion(q);
}

KH_API DXMatrix4X4 KH_MATH::CreateRotationEuler(float _x, float _y, float _z)
{
	const auto yaw = _y * KH_MATH::Pi / 180.0f;
	const auto pitch = _x * KH_MATH::Pi / 180.0f;
	const auto roll = _z * KH_MATH::Pi / 180.0f;

	Quaternion q = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

	return XMMatrixRotationQuaternion(q);
}

KH_API DXMatrix4X4 KH_MATH::CreateTranslation(DXVector3 _v3)
{
	DXMatrix4X4 M;

	M._41 = _v3.x;
	M._42 = _v3.y;
	M._43 = _v3.z;

	return M;
}

KH_API DXMatrix4X4 KH_MATH::CreateTranslation(float _x, float _y, float _z)
{
	DXMatrix4X4 M;

	M._41 = _x;
	M._42 = _y;
	M._43 = _z;

	return M;
}

KH_API DXVector3 KH_MATH::DXVector3Dot(DXVector3 _v1, DXVector3 _v2)
{
	float fValue = (_v1.x * _v2.x) + (_v1.y * _v2.y) + (_v1.z * _v2.z);
	return DXVector3(fValue, fValue, fValue);
}

KH_API DXVector3 KH_MATH::DXVector3Abs(DXVector3 _v3)
{
	return DXVector3(fabsf(_v3.x), fabsf(_v3.y), fabsf(_v3.z));
}

KH_API DXVector3 KH_MATH::DXVector3Sqrt(DXVector3 _v3)
{
	return DXVector3(sqrtf(_v3.x), sqrtf(_v3.y), sqrtf(_v3.z));
}

KH_API DXVector3 KH_MATH::DXVector3Length(DXVector3 _v3)
{
	DXVector3 Vdot = DXVector3Dot(_v3, _v3);

	return DXVector3Sqrt(Vdot);
}

KH_API DXVector3 KH_MATH::DXVector3SplatOne(DXVector3 _v3)
{
	return DXVector3(1.0f, 1.0f, 1.0f);
}

KH_API DXVector3 KH_MATH::DXVector3Replicate(float _value)
{
	return DXVector3(_value, _value, _value);
}

KH_API bool KH_MATH::DXVector3Less(DXVector3 _v1, DXVector3 _v2)
{
	return (((_v1.x < _v2.x) && (_v1.y < _v2.y) && (_v1.z < _v1.z)) != 0);
}

KH_API DXVector4 KH_MATH::DXVector4Abs(DXVector4 _v4)
{
	return DXVector4(fabsf(_v4.x), fabsf(_v4.y), fabsf(_v4.z), fabsf(_v4.w));
}

KH_API DXVector4 KH_MATH::DXVector4Sqrt(DXVector4 _v4)
{
	return DXVector4(sqrtf(_v4.x), sqrtf(_v4.y), sqrtf(_v4.z), sqrtf(_v4.w));
}

KH_API DXVector4 KH_MATH::DXVector4SplatOne(DXVector4 _v4)
{
	return DXVector4(1.0f, 1.0f, 1.0f, 1.0f);
}

KH_API DXVector4 KH_MATH::DXVector4Replicate(float _value)
{
	return DXVector4(_value, _value, _value, _value);
}

KH_API bool KH_MATH::DXVector4Less(DXVector4 _v1, DXVector4 _v2)
{
	return (((_v1.x < _v2.x) && (_v1.y < _v2.y) && (_v1.z < _v1.z) && (_v1.w < _v1.w)) != 0);
}