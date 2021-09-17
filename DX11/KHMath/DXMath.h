#pragma once
///////////////////////////////////////////////////////////////////////////////////
// Math Struct and Function
///////////////////////////////////////////////////////////////////////////////////

/// DLL Export 전용 struct 재구성
/// 간단히 재구성하려다가 변환하는 과정이 너무 귀찮아서 변환 연산자도 추가
/// 기본적으로 필요한 내적, 외적, 정규화만 구현
/// 변환 연산자 및 추가 함수 구성 (XMLoad, XMStore로 변환 안해도 된다)

struct DXVector4;
struct DXMatrix4X4;

struct KH_API DXVector2
{
	float x, y;

	DXVector2();
	DXVector2(const DXVector2& v2);
	DXVector2(const DirectX::XMFLOAT2& xmf2);
	DXVector2(const DirectX::SimpleMath::Vector2& v2);
	DXVector2(float _x, float _y);

	bool operator==(const DXVector2& v2);
	bool operator!=(const DXVector2& v2);

	DXVector2& operator=(const DXVector2& v2);
	DXVector2& operator=(const DirectX::XMFLOAT2& xmf2);
	DXVector2& operator=(const DirectX::SimpleMath::Vector2& v2);
	DXVector2& operator+=(const  DXVector2& v2);
	DXVector2& operator*=(const  DXVector2& v2);
	DXVector2& operator*=(const float f);
	DXVector2 operator+(const DXVector2& v2);
	DXVector2 operator-(const DXVector2& v2);
	DXVector2 operator*(const DXVector2& v2);
	DXVector2 operator*(const float f);

	// 변환 연산자 (Conversion Operator)
	operator DirectX::SimpleMath::Vector2() { return ConvertVector2(); }
	operator DirectX::XMFLOAT2() { return ConvertXMFLOAT2(); }

	// 변환 함수
	DirectX::SimpleMath::Vector2 ConvertVector2();
	DirectX::XMFLOAT2 ConvertXMFLOAT2();

	void Rotate(float angle);

	static DXVector2 Zero();
};

struct KH_API DXVector3
{
	float x, y, z;

	DXVector3();
	DXVector3(const DXVector4& v4);
	DXVector3(const DirectX::XMFLOAT3& xmf3);
	DXVector3(const DirectX::XMFLOAT4& xmf4);
	DXVector3(const DirectX::XMVECTOR& xmv);
	DXVector3(const DirectX::SimpleMath::Vector3& v3);
	DXVector3(const DirectX::SimpleMath::Vector4& v4);
	DXVector3(float _x, float _y, float _z);
	DXVector3(float _x, float _y);
	~DXVector3();

	bool operator==(const DXVector3& v3);
	bool operator!=(const DXVector3& v3);

	DXVector3& operator=(const DXVector3& v3);
	DXVector3& operator=(const DirectX::XMVECTOR& xmv);
	DXVector3& operator=(const DirectX::XMFLOAT3& xmf3);
	DXVector3& operator=(const DirectX::SimpleMath::Vector3& v3);
	DXVector3& operator=(const DirectX::SimpleMath::Vector4& v4);
	DXVector3& operator+=(const  DXVector3& v3);
	DXVector3& operator*=(const  DXVector3& v3);
	DXVector3& operator*=(const float f);
	DXVector3 operator+(const DXVector3& v3);
	DXVector3 operator-(const DXVector3& v3);
	DXVector3 operator*(const DXVector3& v3);
	DXVector3 operator*(const float f);

	// 변환 연산자 (Conversion Operator)
	operator DirectX::SimpleMath::Vector4() { return ConvertVector4(); }
	operator DirectX::XMVECTOR()			{ return ConvertXMVECTOR(); }
	operator DirectX::XMFLOAT4()			{ return ConvertXMFLOAT4(); }
	operator DirectX::XMFLOAT3()			{ return ConvertXMFLOAT3(); }

	
	DXVector3	Normalize();					// 정규화 함수
	DXVector3	Cross(const DXVector3& v3);		// 외적 함수
	float		Dot(const DXVector3& v3);		// 내적 함수
	bool		Equal(const DXVector3& v3);

	// 변환 함수
	DirectX::SimpleMath::Vector4 ConvertVector4();
	DirectX::XMVECTOR ConvertXMVECTOR();
	DirectX::XMFLOAT4 ConvertXMFLOAT4();
	DirectX::XMFLOAT3 ConvertXMFLOAT3();

	static DXVector3 Zero();
	static DXVector3 One();
};

struct KH_API DXVector4
{
	float x, y, z, w;

	DXVector4();
	DXVector4(const DXVector3& v3);
	DXVector4(const DirectX::XMFLOAT3& xmf3);
	DXVector4(const DirectX::XMFLOAT4& xmf4);
	DXVector4(const DirectX::XMVECTOR& xmv);
	DXVector4(const DirectX::SimpleMath::Vector3& v3);
	DXVector4(const DirectX::SimpleMath::Vector4& v4);
	DXVector4(float _x, float _y, float _z, float _w);
	~DXVector4();

	bool operator==(const DXVector4& v4);
	bool operator!=(const DXVector4& v4);

	DXVector4& operator=(const DirectX::XMVECTOR& xmv);
	DXVector4& operator=(const DirectX::XMFLOAT4& xmf4);
	DXVector4& operator=(const DXVector4& v4);
	DXVector4& operator=(const DXVector3& v3);
	DXVector4& operator*=(const float f);
	DXVector4 operator+(const DXVector4& v4);
	DXVector4 operator+(const DXVector3& v3);
	DXVector4 operator-(const DXVector4& v4);
	DXVector4 operator-(const DXVector3& v3);
	DXVector4 operator*(const DXVector4& v4);

	// 변환 연산자 (Conversion Operator)
	operator DirectX::XMVECTOR() { return ConvertXMVECTOR(); }
	operator DirectX::XMFLOAT4() { return ConvertXMFLOAT4(); }
	operator DirectX::XMFLOAT3() { return ConvertXMFLOAT3(); }

	DXVector4	Normalize3();					// 정규화 함수(Vector3)
	DXVector4	Normalize();					// 정규화 함수(Vector4)
	DXVector4	Cross(const DXVector4& v4);		// 외적 함수
	float		Dot(const DXVector4& v4);		// 내적 함수
	bool		Equal(const DXVector4& v4);

	// 변환 함수
	DirectX::XMVECTOR ConvertXMVECTOR();
	DirectX::XMFLOAT4 ConvertXMFLOAT4();
	DirectX::XMFLOAT3 ConvertXMFLOAT3();

	static DXVector4 Zero();
	static DXVector4 One();
};

struct KH_API DXMatrix4X4
{
	union 
	{
		struct  
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};


	DXMatrix4X4();
	DXMatrix4X4(float m11, float m12, float m13, float m14,
				float m21, float m22, float m23, float m24,
				float m31, float m32, float m33, float m34,
				float m41, float m42, float m43, float m44);
	DXMatrix4X4(const DXVector4& v1, const DXVector4& v2, const DXVector4& v3, const DXVector4& v4);
	DXMatrix4X4(const DXVector4& v1, const DXVector4& v2, const DXVector4& v3);
	DXMatrix4X4(const DXVector3& v1, const DXVector3& v2, const DXVector3& v3);
	DXMatrix4X4(const DirectX::XMFLOAT4X4& xmf);
	DXMatrix4X4(const DirectX::XMMATRIX& xmm);
	DXMatrix4X4(const DirectX::SimpleMath::Matrix& mx);
	~DXMatrix4X4();

	bool operator==(const DXMatrix4X4& dxm);

	DXMatrix4X4& operator=(const DirectX::XMFLOAT4X4& xmf);
	DXMatrix4X4& operator=(const DirectX::XMMATRIX& xmm);
	DXMatrix4X4& operator=(const DirectX::SimpleMath::Matrix& mx);
	DXMatrix4X4& operator=(const DXMatrix4X4& dxm);
	DXMatrix4X4& operator+=(const DXMatrix4X4& dxm);
	DXMatrix4X4& operator-=(const DXMatrix4X4& dxm);
	DXMatrix4X4& operator*=(const DXMatrix4X4& dxm);
	DXMatrix4X4 operator+(const DXMatrix4X4& dxm);
	DXMatrix4X4 operator-(const DXMatrix4X4& dxm);
	DXMatrix4X4 operator*(const DXMatrix4X4& dxm);
	DXMatrix4X4 operator*(const DirectX::XMFLOAT4X4& xmf);
	DXMatrix4X4 operator*(const DirectX::XMMATRIX& xmm);
	DXMatrix4X4 operator*(const DirectX::SimpleMath::Matrix& mx);

	float       operator() (size_t Row, size_t Column) const	{ return m[Row][Column]; }
	float&		operator() (size_t Row, size_t Column)			{ return m[Row][Column]; }

	// 변환 연산자 (Conversion Operator)
	operator DirectX::SimpleMath::Matrix()	{ return ConvertXMFLOAT4X4(); }
	operator DirectX::XMFLOAT4X4()			{ return ConvertXMFLOAT4X4(); }
	operator DirectX::XMMATRIX()			{ return ConvertXMMATRIX(); }

	DXMatrix4X4 Inverse();
	DXVector4 GetRow(int row);

	// 변환 함수
	DirectX::SimpleMath::Matrix ConvertMatrix();
	DirectX::XMFLOAT4X4 ConvertXMFLOAT4X4();
	DirectX::XMMATRIX ConvertXMMATRIX();
};

namespace KH_MATH
{
	KH_API DirectX::XMVECTOR XMStoreFloat3(DirectX::XMFLOAT3 _xmf);
	KH_API DirectX::XMFLOAT3 XMLoadFloat3(DirectX::XMVECTOR _xmv);
	KH_API DXVector3 BezierCurveThree(DXVector3 _p1, DXVector3 _p2, DXVector3 _p3, float t);
	KH_API DXVector3 BezierCurveFour(DXVector3 _p1, DXVector3 _p2, DXVector3 _p3, DXVector3 _p4, float t);
	KH_API DXVector3 GetMiddlePoint(DXVector3 _p1, DXVector3 _p2);
	KH_API DXMatrix4X4 RotationAxis(DXVector3 _axis, float _angle);
	KH_API float GetDistance(DXVector3 _p1, DXVector3 _p2);
	KH_API float GetDistance(DXVector2 _p1, DXVector2 _p2);
	KH_API bool CompareXMFLOAT3(DirectX::XMFLOAT3 _xmf1, DirectX::XMFLOAT3 _xmf2);

	KH_API DXMatrix4X4 CreateScale(DXVector3 _v3);
	KH_API DXMatrix4X4 CreateScale(float _x, float _y, float _z);
	KH_API DXMatrix4X4 CreateRotationEuler(DXVector3 _v3);
	KH_API DXMatrix4X4 CreateRotationEuler(float _x, float _y, float _z);
	KH_API DXMatrix4X4 CreateTranslation(DXVector3 _v3);
	KH_API DXMatrix4X4 CreateTranslation(float _x, float _y, float _z);

	KH_API DXVector3 DXVector3Dot(DXVector3 _v1, DXVector3 _v2);
	KH_API DXVector3 DXVector3Abs(DXVector3 _v3);
	KH_API DXVector3 DXVector3Sqrt(DXVector3 _v3);
	KH_API DXVector3 DXVector3Length(DXVector3 _v3);
	KH_API DXVector3 DXVector3SplatOne(DXVector3 _v3);
	KH_API DXVector3 DXVector3Replicate(float _value);
	KH_API bool DXVector3Less(DXVector3 _v1, DXVector3 _v2);

	KH_API DXVector4 DXVector4Abs(DXVector4 _v4);
	KH_API DXVector4 DXVector4Sqrt(DXVector4 _v4);
	KH_API DXVector4 DXVector4SplatOne(DXVector4 _v4);
	KH_API DXVector4 DXVector4Replicate(float _value);
	KH_API bool DXVector4Less(DXVector4 _v1, DXVector4 _v2);

	const float Infinity = FLT_MAX;
	const float Pi = 3.1415926535f;
}