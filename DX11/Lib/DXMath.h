#pragma once
///////////////////////////////////////////////////////////////////////////////////
// Math Struct and Function
///////////////////////////////////////////////////////////////////////////////////

/// DLL Export 전용 struct 재구성
/// 간단히 재구성하려다가 변환하는 과정이 너무 귀찮아서 변환 연산자도 추가
/// 기본적으로 필요한 내적, 외적, 정규화만 구현
/// 변환 연산자 및 추가 함수 구성 (XMLoad, XMStore로 변환 안해도 된다)

struct DXVector4;
struct DXQuaternion;
struct DXMatrix4X4;

struct MATH_DLL DXVector2
{
	float x, y;

	DXVector2();
	DXVector2(const DirectX::XMFLOAT2& xmf2);
	DXVector2(const DirectX::SimpleMath::Vector2& v2);
	DXVector2(float _x, float _y);
	DXVector2(const DXVector2& v2) = default;
	DXVector2(DXVector2&& v3) = default;

	bool operator==(const DXVector2& v2) const noexcept;
	bool operator!=(const DXVector2& v2) const noexcept;

	DXVector2& operator=(const DXVector2& v2) = default;
	DXVector2& operator=(DXVector2&& v2) = default;

	DXVector2& operator=(const DirectX::XMFLOAT2& xmf2) noexcept;
	DXVector2& operator=(const DirectX::SimpleMath::Vector2& v2) noexcept;
	DXVector2& operator+=(const  DXVector2& v2) noexcept;
	DXVector2& operator*=(const  DXVector2& v2) noexcept;
	DXVector2& operator*=(const float f) noexcept;
	DXVector2 operator+(const DXVector2& v2) noexcept;
	DXVector2 operator-(const DXVector2& v2) noexcept;
	DXVector2 operator*(const DXVector2& v2) noexcept;
	DXVector2 operator*(const float f) noexcept;

	// 변환 연산자 (Conversion Operator)
	operator DirectX::SimpleMath::Vector2() { return ConvertVector2(); }
	operator DirectX::XMFLOAT2() { return ConvertXMFLOAT2(); }

	// 변환 함수
	DirectX::SimpleMath::Vector2 ConvertVector2();
	DirectX::XMFLOAT2 ConvertXMFLOAT2();

	void Rotate(float angle) noexcept;


	static DXVector2 Zero();
};

struct MATH_DLL DXVector3
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
	DXVector3(const DXVector3& v3) = default;
	DXVector3(DXVector3&& v3) = default;

	bool operator==(const DXVector3& v3) const noexcept;
	bool operator!=(const DXVector3& v3) const noexcept;

	DXVector3& operator=(const DXVector3& v3) = default;
	DXVector3& operator=(DXVector3&& v3) = default;

	DXVector3& operator=(const DirectX::XMVECTOR& xmv) noexcept;
	DXVector3& operator=(const DirectX::XMFLOAT3& xmf3) noexcept;
	DXVector3& operator=(const DirectX::SimpleMath::Vector3& v3) noexcept;
	DXVector3& operator=(const DirectX::SimpleMath::Vector4& v4) noexcept;
	DXVector3& operator+=(const  DXVector3& v3) noexcept;
	DXVector3& operator*=(const  DXVector3& v3) noexcept;
	DXVector3& operator*=(const float f) noexcept;
	DXVector3 operator+(const DXVector3& v3) noexcept;
	DXVector3 operator-(const DXVector3& v3) noexcept;
	DXVector3 operator*(const DXVector3& v3) noexcept;
	DXVector3 operator*(const float f) noexcept;

	// 변환 연산자 (Conversion Operator)
	operator DirectX::SimpleMath::Vector4() { return ConvertVector4(); }
	operator DirectX::XMVECTOR()			{ return ConvertXMVECTOR(); }
	operator DirectX::XMFLOAT4()			{ return ConvertXMFLOAT4(); }
	operator DirectX::XMFLOAT3()			{ return ConvertXMFLOAT3(); }

	
	DXVector3	Normalize() noexcept;
	DXVector3	Clamp(const DXVector3& v3min, const DXVector3& v3max) noexcept;
	DXVector3	Cross(const DXVector3& v3) noexcept;
	float		Dot(const DXVector3& v3) noexcept;

	// 변환 함수
	DirectX::SimpleMath::Vector4 ConvertVector4();
	DirectX::XMVECTOR ConvertXMVECTOR();
	DirectX::XMFLOAT4 ConvertXMFLOAT4();
	DirectX::XMFLOAT3 ConvertXMFLOAT3();

	static DXVector3 Reflect(const DXVector3& ivec, const DXVector3& nvec) noexcept;
	static DXVector3 Transform(const DXVector3& v, const DXQuaternion& quat) noexcept;
	static DXVector3 Transform(const DXVector3& v, const DXMatrix4X4& m) noexcept;
	static DXVector3 TransformNormal(const DXVector3& v, const DXMatrix4X4& m) noexcept;
	static DXVector3 Zero();
	static DXVector3 One();
};

struct MATH_DLL DXVector4
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
	DXVector4(const DXVector4& v4) = default;
	DXVector4(DXVector4&& v4) = default;

	bool operator==(const DXVector4& v4) const;
	bool operator!=(const DXVector4& v4) const;

	DXVector4& operator=(const DXVector4& v4) = default;
	DXVector4& operator=(DXVector4&& v4) = default;

	DXVector4& operator=(const DXVector3& v3);
	DXVector4& operator=(const DirectX::XMVECTOR& xmv);
	DXVector4& operator=(const DirectX::XMFLOAT4& xmf4);
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

	DXVector4	Normalize3() noexcept;
	DXVector4	Normalize() noexcept;
	DXVector4	Clamp(const DXVector4& v4min, const DXVector4& v4max) noexcept;
	DXVector4	Cross(const DXVector4& v4) noexcept;
	float		Dot(const DXVector4& v4) noexcept;

	// 변환 함수
	DirectX::XMVECTOR ConvertXMVECTOR();
	DirectX::XMFLOAT4 ConvertXMFLOAT4();
	DirectX::XMFLOAT3 ConvertXMFLOAT3();

	static DXVector4 Reflect(const DXVector4& ivec, const DXVector4& nvec) noexcept;
	static DXVector4 Transform(const DXVector4& v, const DXQuaternion& quat) noexcept;
	static DXVector4 Transform(const DXVector4& v, const DXMatrix4X4& m) noexcept;
	static DXVector4 TransformNormal(const DXVector4& v, const DXMatrix4X4& m) noexcept;
	static DXVector4 Zero();
	static DXVector4 One();
};

struct MATH_DLL DXMatrix4X4
{
	union 
	{
		DXVector4 r[4];
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
	DXMatrix4X4(const DXMatrix4X4& dxm) = default;
	DXMatrix4X4(DXMatrix4X4&& dxm) = default;

	bool operator==(const DXMatrix4X4& dxm) const;

	DXMatrix4X4& operator=(const DXMatrix4X4& dxm) = default;
	DXMatrix4X4& operator=(DXMatrix4X4&& dxm) = default;

	DXMatrix4X4& operator=(const DirectX::XMFLOAT4X4& xmf);
	DXMatrix4X4& operator=(const DirectX::XMMATRIX& xmm);
	DXMatrix4X4& operator=(const DirectX::SimpleMath::Matrix& mx);
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
	DXVector4	operator() (size_t Row) const					{ return DXVector4(m[Row][0], m[Row][1], m[Row][2], m[Row][3]); }
	DXVector4&	operator() (size_t Row)							{ return DXVector4(m[Row][0], m[Row][1], m[Row][2], m[Row][3]); }

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

struct MATH_DLL DXQuaternion : public DXVector4
{
	DXQuaternion();
	DXQuaternion(const DXVector4& v4);
	DXQuaternion(const DXQuaternion&) = default;
	DXQuaternion(DXQuaternion&&) = default;

	DXQuaternion& operator=(const DXQuaternion&) = default;
	DXQuaternion& operator=(DXQuaternion&&) = default;

	bool operator == (const DXQuaternion& q) const;
	bool operator != (const DXQuaternion& q) const;

	DXQuaternion& operator= (const DirectX::XMVECTORF32& vf) noexcept;
	DXQuaternion& operator= (const DirectX::XMVECTOR& v) noexcept;
	DXQuaternion& operator+= (const DXQuaternion& q) noexcept;
	DXQuaternion& operator-= (const DXQuaternion& q) noexcept;
	DXQuaternion& operator*= (const DXQuaternion& q) noexcept;
	DXQuaternion& operator*= (float S) noexcept;
	DXQuaternion& operator/= (const DXQuaternion& q) noexcept;

	DXQuaternion operator+ () const noexcept { return *this; }
	DXQuaternion operator- () const noexcept;

	// Quaternion operations
	float Length() const noexcept;
	float LengthSquared() const noexcept;

	void Normalize() noexcept;
	void Normalize(DXQuaternion& q) const noexcept;

	void Conjugate() noexcept;
	void Conjugate(DXQuaternion& q) const noexcept;

	void Inverse(DXQuaternion& q) const noexcept;

	float Dot(const DXQuaternion& q) const noexcept;

	static DXQuaternion CreateFromAxisAngle(const DXVector3& axis, float angle) noexcept;
	static DXQuaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;
	static DXQuaternion CreateFromRotationMatrix(const DXMatrix4X4& M) noexcept;

	static void Lerp(const DXQuaternion& q1, const DXQuaternion& q2, float t, DXQuaternion& result) noexcept;
	static DXQuaternion Lerp(const DXQuaternion& q1, const DXQuaternion& q2, float t) noexcept;

	static void Slerp(const DXQuaternion& q1, const DXQuaternion& q2, float t, DXQuaternion& result) noexcept;
	static DXQuaternion Slerp(const DXQuaternion& q1, const DXQuaternion& q2, float t) noexcept;

	static void Concatenate(const DXQuaternion& q1, const DXQuaternion& q2, DXQuaternion& result) noexcept;
	static DXQuaternion Concatenate(const DXQuaternion& q1, const DXQuaternion& q2) noexcept;
};

namespace KH_MATH
{
	MATH_DLL DirectX::XMVECTOR XMStoreFloat3(const DXVector3& _v3);
	MATH_DLL DirectX::XMFLOAT3 XMLoadFloat3(const DXVector3& _v3);
	MATH_DLL DirectX::XMVECTOR XMStoreFloat4(const DXVector4& _v4);
	MATH_DLL DirectX::XMFLOAT4 XMLoadFloat4(const DXVector4& _v4);
	MATH_DLL DirectX::XMMATRIX XMStoreFloat4X4(const DXMatrix4X4& _v4);

	MATH_DLL DXVector3 BezierCurveThree(DXVector3& _p1, DXVector3& _p2, DXVector3& _p3, float t);
	MATH_DLL DXVector3 BezierCurveFour(DXVector3& _p1, DXVector3& _p2, DXVector3& _p3, DXVector3& _p4, float t);
	MATH_DLL DXVector3 GetMiddlePoint(DXVector3& _p1, DXVector3& _p2);
	MATH_DLL DXMatrix4X4 RotationAxis(DXVector3& _axis, float _angle);
	MATH_DLL float GetDistance(DXVector3& _p1, DXVector3& _p2);
	MATH_DLL float GetDistance(DXVector2& _p1, DXVector2& _p2);
	MATH_DLL bool CompareXMFLOAT3(DirectX::XMFLOAT3& _xmf1, DirectX::XMFLOAT3& _xmf2);
	
	MATH_DLL DXMatrix4X4 CreateScale(DXVector3 _v3);
	MATH_DLL DXMatrix4X4 CreateScale(float _x, float _y, float _z);
	MATH_DLL DXMatrix4X4 CreateRotationEuler(DXVector3 _v3);
	MATH_DLL DXMatrix4X4 CreateRotationEuler(float _x, float _y, float _z);
	MATH_DLL DXMatrix4X4 CreateTranslation(DXVector3 _v3);
	MATH_DLL DXMatrix4X4 CreateTranslation(float _x, float _y, float _z);
	
	MATH_DLL DXVector3 DXVector3Max(const DXVector3& _v1, const DXVector3& _v2);
	MATH_DLL DXVector3 DXVector3Min(const DXVector3& _v1, const DXVector3& _v2);
	MATH_DLL DXVector3 DXVector3Clamp(const DXVector3& _v3, const DXVector3& _v3min, const DXVector3& _v3max);
	MATH_DLL DXVector3 DXVector3Dot(DXVector3& _v1, DXVector3& _v2);
	MATH_DLL DXVector3 DXVector3Abs(DXVector3& _v3);
	MATH_DLL DXVector3 DXVector3Sqrt(DXVector3& _v3);
	MATH_DLL DXVector3 DXVector3Length(DXVector3& _v3);
	MATH_DLL DXVector3 DXVector3SplatOne(DXVector3& _v3);
	MATH_DLL DXVector3 DXVector3Replicate(float _value);
	MATH_DLL bool DXVector3Less(DXVector3& _v1, DXVector3& _v2);
	
	MATH_DLL DXVector4 DXVector4Max(const DXVector4& _v1, const DXVector4& _v2);
	MATH_DLL DXVector4 DXVector4Min(const DXVector4& _v1, const DXVector4& _v2);
	MATH_DLL DXVector4 DXVector4Clamp(const DXVector4& _v4, const DXVector4& _v4min, const DXVector4& _v4max);
	MATH_DLL DXVector4 DXVector4Abs(DXVector4& _v4);
	MATH_DLL DXVector4 DXVector4Sqrt(DXVector4& _v4);
	MATH_DLL DXVector4 DXVector4SplatOne(DXVector4& _v4);
	MATH_DLL DXVector4 DXVector4Replicate(float _value);
	MATH_DLL bool DXVector4Less(DXVector4& _v1, DXVector4& _v2);
	
	const float Infinity = FLT_MAX;
	const float Pi = 3.1415926535f;
}