#pragma once
///////////////////////////////////////////////////////////////////////////////////
// Math Struct and Function
///////////////////////////////////////////////////////////////////////////////////

/// DLL Export ���� struct �籸��
/// ������ �籸���Ϸ��ٰ� ��ȯ�ϴ� ������ �ʹ� �����Ƽ� ��ȯ �����ڵ� �߰�
/// �⺻������ �ʿ��� ����, ����, ����ȭ�� ����
/// ��ȯ ������ �� �߰� �Լ� ���� (XMLoad, XMStore�� ��ȯ ���ص� �ȴ�)

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

	// ��ȯ ������ (Conversion Operator)
	operator DirectX::SimpleMath::Vector2() { return ConvertVector2(); }
	operator DirectX::XMFLOAT2() { return ConvertXMFLOAT2(); }

	// ��ȯ �Լ� (Conversion Function)
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

	// ��ȯ ������ (Conversion Operator)
	operator DirectX::SimpleMath::Vector4() { return ConvertVector4(); }
	operator DirectX::XMVECTOR()			{ return ConvertXMVECTOR(); }
	operator DirectX::XMFLOAT4()			{ return ConvertXMFLOAT4(); }
	operator DirectX::XMFLOAT3()			{ return ConvertXMFLOAT3(); }

	// ��ȯ �Լ� (Conversion Function)
	DirectX::SimpleMath::Vector4 ConvertVector4();
	DirectX::XMVECTOR ConvertXMVECTOR();
	DirectX::XMFLOAT4 ConvertXMFLOAT4();
	DirectX::XMFLOAT3 ConvertXMFLOAT3();
	
	DXVector3	Normalize() noexcept;
	DXVector3	Clamp(const DXVector3& v3min, const DXVector3& v3max) noexcept;
	DXVector3	Cross(const DXVector3& v3) noexcept;
	float		Dot(const DXVector3& v3) noexcept;

	static DXVector3 Reflect(const DXVector3& ivec, const DXVector3& nvec) noexcept;
	static DXVector3 Transform(const DXVector3& m_V, const DXQuaternion& quat) noexcept;
	static DXVector3 Transform(const DXVector3& m_V, const DXMatrix4X4& m) noexcept;
	static DXVector3 TransformNormal(const DXVector3& m_V, const DXMatrix4X4& m) noexcept;
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

	// ��ȯ ������ (Conversion Operator)
	operator DirectX::XMVECTOR() { return ConvertXMVECTOR(); }
	operator DirectX::XMFLOAT4() { return ConvertXMFLOAT4(); }
	operator DirectX::XMFLOAT3() { return ConvertXMFLOAT3(); }

	// ��ȯ �Լ� (Conversion Function)
	DirectX::XMVECTOR ConvertXMVECTOR();
	DirectX::XMFLOAT4 ConvertXMFLOAT4();
	DirectX::XMFLOAT3 ConvertXMFLOAT3();

	DXVector4	Normalize3() noexcept;
	DXVector4	Normalize() noexcept;
	DXVector4	Clamp(const DXVector4& v4min, const DXVector4& v4max) noexcept;
	DXVector4	Cross(const DXVector4& v4) noexcept;
	float		Dot(const DXVector4& v4) noexcept;

	static DXVector4 Reflect(const DXVector4& ivec, const DXVector4& nvec) noexcept;
	static DXVector4 Transform(const DXVector4& m_V, const DXQuaternion& quat) noexcept;
	static DXVector4 Transform(const DXVector4& m_V, const DXMatrix4X4& m) noexcept;
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

	// ��ȯ ������ (Conversion Operator)
	operator DirectX::SimpleMath::Matrix()	{ return ConvertXMFLOAT4X4(); }
	operator DirectX::XMFLOAT4X4()			{ return ConvertXMFLOAT4X4(); }
	operator DirectX::XMMATRIX()			{ return ConvertXMMATRIX(); }

	// ��ȯ �Լ� (Conversion Function)
	DirectX::SimpleMath::Matrix ConvertMatrix();
	DirectX::XMFLOAT4X4 ConvertXMFLOAT4X4();
	DirectX::XMMATRIX ConvertXMMATRIX();

	// Matrix Properties
	DXVector3 Up() const noexcept { return DXVector3(_21, _22, _23); }
	void Up(const DXVector3& m_V) noexcept { _21 = m_V.x; _22 = m_V.y; _23 = m_V.z; }

	DXVector3 Down() const  noexcept { return DXVector3(-_21, -_22, -_23); }
	void Down(const DXVector3& m_V) noexcept { _21 = -m_V.x; _22 = -m_V.y; _23 = -m_V.z; }

	DXVector3 Right() const noexcept { return DXVector3(_11, _12, _13); }
	void Right(const DXVector3& m_V) noexcept { _11 = m_V.x; _12 = m_V.y; _13 = m_V.z; }

	DXVector3 Left() const noexcept { return DXVector3(-_11, -_12, -_13); }
	void Left(const DXVector3& m_V) noexcept { _11 = -m_V.x; _12 = -m_V.y; _13 = -m_V.z; }

	DXVector3 Forward() const noexcept { return DXVector3(-_31, -_32, -_33); }
	void Forward(const DXVector3& m_V) noexcept { _31 = -m_V.x; _32 = -m_V.y; _33 = -m_V.z; }

	DXVector3 Backward() const noexcept { return DXVector3(_31, _32, _33); }
	void Backward(const DXVector3& m_V) noexcept { _31 = m_V.x; _32 = m_V.y; _33 = m_V.z; }
	
	DXVector3 Translation() const  noexcept { return DXVector3(_41, _42, _43); }
	void Translation(const DXVector3& m_V) noexcept { _41 = m_V.x; _42 = m_V.y; _43 = m_V.z; }
	
	// Matrix Function
	bool Decompose(DXVector3& scale, DXQuaternion& rotation, DXVector3& translation) noexcept;
	float Determinant() const noexcept;
	DXMatrix4X4 Transpose() const noexcept;
	DXMatrix4X4 Inverse() const noexcept;
	DXVector4 GetRow(int row) const noexcept;

	// Static Function
	static DXMatrix4X4 CreateTranslation(const DXVector3& position) noexcept;
	static DXMatrix4X4 CreateTranslation(float x, float y, float z) noexcept;

	static DXMatrix4X4 CreateScale(const DXVector3& scales) noexcept;
	static DXMatrix4X4 CreateScale(float xs, float ys, float zs) noexcept;
	static DXMatrix4X4 CreateScale(float scale) noexcept;

	static DXMatrix4X4 CreateRotationX(float radians) noexcept;
	static DXMatrix4X4 CreateRotationY(float radians) noexcept;
	static DXMatrix4X4 CreateRotationZ(float radians) noexcept;

	static DXMatrix4X4 CreateFromAxisAngle(const DXVector3& axis, float angle) noexcept;

	static DXMatrix4X4 CreatePerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;
	static DXMatrix4X4 CreatePerspective(float width, float height, float nearPlane, float farPlane) noexcept;
	static DXMatrix4X4 CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane) noexcept;
	static DXMatrix4X4 CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane) noexcept;
	static DXMatrix4X4 CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) noexcept;

	static DXMatrix4X4 CreateLookAt(const DXVector3& eye, const DXVector3& target, const DXVector3& up) noexcept;
	static DXMatrix4X4 CreateWorld(const DXVector3& position, const DXVector3& forward, const DXVector3& up) noexcept;

	static DXMatrix4X4 CreateFromQuaternion(const DXQuaternion& rotation) noexcept;
	static DXMatrix4X4 CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;
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
	DXQuaternion& operator= (const DirectX::XMVECTOR& m_V) noexcept;
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
	MATH_DLL DirectX::XMVECTOR XMLoadVector3(const DXVector3& _v3);
	MATH_DLL DirectX::XMFLOAT3 XMLoadFloat3(const DXVector3& _v3);
	MATH_DLL DirectX::XMVECTOR XMLoadVector4(const DXVector4& _v4);
	MATH_DLL DirectX::XMFLOAT4 XMLoadFloat4(const DXVector4& _v4);
	MATH_DLL DirectX::XMMATRIX XMLoadFloat4X4(const DXMatrix4X4& _v4);

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