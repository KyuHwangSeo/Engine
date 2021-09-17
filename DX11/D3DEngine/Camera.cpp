#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "GeometryGenerator.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Patch_Chapters.h"
#include "Gizmos.h"
#include "Camera.h"
 
using namespace SimpleMath;

Camera* Camera::g_MainCamera = nullptr;
Transform* Camera::m_Target = nullptr;

Camera::Camera(eCameraType cameraType)
	: m_CameraType(cameraType),
	mPosition(0.0f, 0.0f, 0.0f),
	mRight(1.0f, 0.0f, 0.0f),
	mUp(0.0f, 1.0f, 0.0f),
	mLook(0.0f, 0.0f, 1.0f)
{
	SetName<Camera>();
	m_ClientSize = D3DEngine::GetInstance()->GetClinetSize();
}

Camera::~Camera()
{

}

DXVector3 Camera::GetPosition()
{
	return mPosition;
}

DXVector3 Camera::GetRight()
{
	return mRight;
}

DXVector3 Camera::GetUp()
{
	return mUp;
}

DXVector3 Camera::GetLook()
{
	return mLook;
}

DXVector3 Camera::GetTargetPos()
{
	return m_Target->GetPosition();
}

DXVector3 Camera::GetTargetRot()
{
	return m_Target->GetRotation();
}

float Camera::GetNearZ()
{
	return mNearZ;
}

float Camera::GetFarZ()
{
	return mFarZ;
}

float Camera::GetAspect()
{
	return mAspect;
}

float Camera::GetFovY()
{
	return mFovY;
}

float Camera::GetFovX()
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / mNearZ);
}

float Camera::GetNearWindowWidth()
{
	return mAspect * mNearWindowHeight;
}

float Camera::GetNearWindowHeight()
{
	return mNearWindowHeight;
}

float Camera::GetFarWindowWidth()
{
	return mAspect * mFarWindowHeight;
}

float Camera::GetFarWindowHeight()
{
	return mFarWindowHeight;
}

void Camera::SetLens(float fovY, float width, float height, float zn, float zf)
{
	// cache properties
	mFovY = fovY;
	mAspect = width / height;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);

	mProj = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	//mOrtho = XMMatrixOrthographicLH(width, height, zn, zf);
	mOrtho = XMMatrixOrthographicOffCenterLH(0.0f, width, height, 0.0f, zn, zf);

	//m_Frustom->ComputeFrustum(mProj);
}

void Camera::LookAt(DXVector3 pos, DXVector3 target, DXVector3 worldUp)
{
	// �ü����Ϳ� ������� �����ϸ� right���Ͱ� ������
	// �ٽ� �� right���Ϳ� �ü����͸� �����ϸ� �� up���Ͱ� ����
	mLook = XMVector3Normalize(XMVectorSubtract(target, pos));
	mRight = XMVector3Normalize(XMVector3Cross(worldUp, mLook));
	mUp = XMVector3Cross(mLook, mRight);

	mPosition = pos;
}

void Camera::LookAt_3_Person_FreeView(DXVector3 offsetPos, float offset)
{
	if (m_Target == nullptr) return;
	
	// Look Vector �������� ī�޶� �̵������� �� Target �������� ī�޶� ����..
	DXVector3 targetPos = GetTargetPos() + offsetPos;
	DXVector3 camPos = targetPos;
	camPos.x -= offset * mLook.x;
	camPos.y -= offset * mLook.y;
	camPos.z -= offset * mLook.z;

	LookAt(camPos, targetPos, DXVector3(0, 1.0f, 0));
}

void Camera::LookAt_3_Person(DXVector3 offsetPos, float offset, float offsetY)
{
	if (m_Target == nullptr) return;
	
	// TargetRotate �������� ī�޶� ȸ����������..
	// 3��Ī ī�޶� �̹Ƿ� ���ʿ��� Target�� ���� �������� ī�޶� ����..
	DXVector3 targetPos = GetTargetPos() + offsetPos;
	DXVector3 targetRot = GetTargetRot();
	DXVector3 camPos = targetPos;
	camPos.x -= offset * targetRot.x;
	camPos.y -= offsetY + offset * targetRot.y;
	camPos.z -= offset * targetRot.z;

	LookAt(camPos, targetPos, DXVector3(0, 1.0f, 0));
}

void Camera::LookAt_3_Person_MoveX(DXVector3 offsetPos)
{
	if (m_Target == nullptr) return;
	DXVector3 targetPos = GetTargetPos();
	targetPos.y += 10.0f;

	if (m_Target->GetPosition().x > 7.5f)
	{
		DXVector3 camPos = offsetPos;
		camPos.x += 7.5f;
		camPos.y += targetPos.y;
		LookAt(camPos, targetPos, DXVector3(0, 1.0f, 0));
		return;
	}

	if (m_Target->GetPosition().x < -7.5f)
	{
		DXVector3 camPos = offsetPos;
		camPos.x -= 7.5f;
		camPos.y += targetPos.y;
		LookAt(camPos, targetPos, DXVector3(0, 1.0f, 0));
		return;
	}

	if (m_Target->GetPosition().z < -3.0f)
	{
		DXVector3 camPos = offsetPos;
		camPos.x += targetPos.x;
		camPos.y += targetPos.y;

		targetPos.z = -3.0f;

		LookAt(camPos, targetPos, DXVector3(0, 1.0f, 0));

		return;
	}

	// TargetRotate �������� ī�޶� ȸ����������..
	// 3��Ī ī�޶� �̹Ƿ� ���ʿ��� Target�� ���� �������� ī�޶� ����..
	DXVector3 camPos = offsetPos;
	camPos.x += targetPos.x;
	camPos.y += targetPos.y;

	LookAt(camPos, targetPos, DXVector3(0, 1.0f, 0));
}

void Camera::LookAt_1_Person(DXVector3 offsetPos, float offset)
{
	if (m_Target == nullptr) return;
	
	// TargetRotate �������� ī�޶� ȸ����������..
	// 1��Ī ī�޶� �̹Ƿ� Target ��ġ���� Target �� ���� �������� ī�޶� ����..
	DXVector3 targetPos = GetTargetPos() + offsetPos;
	DXVector3 targetRot = GetTargetRot();
	DXVector3 camPos = targetPos;
	camPos.x -= offset * targetRot.x;
	camPos.y -= offset * targetRot.y;
	camPos.z -= offset * targetRot.z;
	LookAt(camPos, targetPos, XMFLOAT3(0, 1.0f, 0));
}

DXMatrix4X4 Camera::View() const
{
	return mView;
}

DXMatrix4X4 Camera::PrevView() const
{
	return mPrevView;
}

DXMatrix4X4 Camera::Proj() const
{
	return mProj;
}

DXMatrix4X4 Camera::ViewProj() const
{
	return View() * Proj();
}

DXMatrix4X4 Camera::Ortho() const
{
	return mOrtho;
}

DXVector3 Camera::GetQuaternionRot()
{
	XMVECTOR ScaleTM;
	XMVECTOR RotTM;
	XMVECTOR PosTM;
	XMMatrixDecompose(&ScaleTM, &RotTM, &PosTM, mView);

	return RotTM;
}

void Camera::Strafe(float d)
{
	// mPosition += d*mRight
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = mRight;
	XMVECTOR p = mPosition;
	mPosition = XMVectorMultiplyAdd(s, r, p);
}

void Camera::Walk(float d)
{
	// mPosition += d*mLook
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = mLook;
	XMVECTOR p = mPosition;
	mPosition = XMVectorMultiplyAdd(s, l, p);
}

void Camera::WorldUpDown(float d)
{
	// mPosition += d * mUp
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR u = mUp;
	XMVECTOR p = mPosition;
	mPosition = XMVectorMultiplyAdd(s, u, p);
}

void Camera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.
	XMMATRIX R = XMMatrixRotationAxis(mRight, angle);

	mUp = XMVector3TransformNormal(mUp, R);
	mLook = XMVector3TransformNormal(mLook, R);
}

void Camera::RotateX(float angle)
{
	m_Angle += angle;

	/// Angle �� ����..
	/// �߽����� �Ѿ�ԵǸ� ī�޶� ȸ���� ���´�..
	if (m_Angle > 0.99f || m_Angle < -0.99f)
	{
		m_Angle -= angle;
		return;
	}

	XMMATRIX R = XMMatrixRotationAxis(mRight, angle);

	mUp = XMVector3TransformNormal(mUp, R);
	mLook = XMVector3TransformNormal(mLook, R);
}

void Camera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.
	XMMATRIX R = XMMatrixRotationY(angle);

	mRight = XMVector3TransformNormal(mRight, R);
	mLook = XMVector3TransformNormal(mLook, R);
}

void Camera::RotateZ(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationZ(angle);

	mRight = XMVector3TransformNormal(mRight, R);
	mUp = XMVector3TransformNormal(mUp, R);
}

void Camera::SmoothMove1(DXVector3& before, DXVector3& after, float deltaTime)
{
	// �߸��� ��� 1
	/// ���� ī�޶� ��� -> ���� ī�޶� ���
	/// 1. Decompose�� ����, ���� ī�޶� ����� QuaternionRot, Position ��������
	/// 2. ����, ���� ī�޶��� Position -> Lerp / QuaternionRot -> SLerp
	/// 3. ������ ��� ������
	/// �̷��� �������� �ڿ������� �������� ������ ��� ����..
	/// �غ��� ���� �ȵǴ� ����.. �������� �˼��� ����...

	if (Is_Move)
	{
		m_Time += deltaTime * 1000;

		XMVECTOR localScale;
		XMVECTOR localRot;
		XMVECTOR localPos;
		XMMatrixDecompose(&localScale, &localRot, &localPos, mView);

		XMFLOAT3 Pos;
		Pos.x = MathHelper::Lerp<float>(before.x, after.x, m_Time / 1000);
		Pos.y = MathHelper::Lerp<float>(before.y, after.y, m_Time / 1000);
		Pos.z = MathHelper::Lerp<float>(before.z, after.z, m_Time / 1000);
		Pos.x += Pos.y;

		if (fabs(after.y - Pos.y) <= 0.05f &&
			fabs(after.z - Pos.z) <= 0.05f)
		{
			LookAt(after, GetTargetPos(), XMFLOAT3(0.0f, 1.0f, 0.0f));
			Is_Move = false;
			m_Time = 0;
			return;
		}

		//before = movePos;

		LookAt(Pos, GetTargetPos(), XMFLOAT3(0.0f, 1.0f, 0.0f));
	}
}

void Camera::SmoothMove2(DXVector3& beforePos, DXVector3 beforeRight, float deltaTime)
{
	// �߸��� ��� 2
	/// BezierCurve ����
	/// 1. ���� ī�޶�, ���� ī�޶� ������ ��ġ�� Right Vector�� �����´�
	/// 2. �� ������ Right Vector ���� �� ������ �߰��Ѵ�
	/// 3. ��������� ���� 4���� ������ BezierCurve�� ���� �������ش�
	/// �ε巴�� �����̱� ������ ���� ���ϴ� ������ �ƴϴ�..
	/// �� ī�޶��� ��ġ�� ���� ������ �޶����°��� ����..

	if (Is_Move)
	{
		m_Time += deltaTime;

		if (m_Time / 10 > 1.0f)
		{
			LookAt(beforePos, GetTargetPos(), XMFLOAT3(0.0f, 1.0f, 0.0f));
			Is_Move = false;
			m_Time = 0.0f;
		}

		DXVector3 nowPos = GetPosition();
		float dis = KH_MATH::GetDistance(nowPos, beforePos) / 2;
		if (dis == 0.0f)	return;

		DXVector3 nowRight = GetRight() * dis;
		beforeRight *= -dis;

		DXVector3 result = KH_MATH::BezierCurveFour(beforePos, beforeRight, nowRight, nowPos, m_Time / 10);

		LookAt(result, GetTargetPos(), XMFLOAT3(0.0f, 1.0f, 0.0f));
	}
}

void Camera::SmoothMove3(DXVector3 beforePos, float deltaTime)
{
	if (Is_Move)
	{
		m_Time += deltaTime;

		if (m_Time > 1.0f)
		{
			Is_Move = false;
			return;
		}

		// ���� ��������..
		DXVector3 endPos = GetPosition();

		// �߰� ����..
		DXVector3 midPos = endPos - beforePos;

		// �Ÿ��� ������..
		float dis = KH_MATH::GetDistance(endPos, beforePos) / 2;

		// ������ ī�޶� ��ġ����..
		DXVector3 nowPos = midPos;

		DXVector3 nowRight = XMVector3Cross(midPos, XMVECTOR{ 0.0f, 1.0f, 0.0f });
		DXVector3 nowUp = XMVector3Cross(midPos, nowRight);
		
		nowPos = XMVector3Transform(nowPos, KH_MATH::RotationAxis(nowUp, m_Time));
		
		LookAt(nowPos, GetTargetPos(), XMFLOAT3(0.0f, 1.0f, 0.0f));
	}
}

void Camera::SetType(eCameraType type)
{
	m_CameraType = type;
}

void Camera::Start()
{
	// Camera Frustom ����..
	//m_Frustom = new CameraFrustom();

	// Camera Gizmo ����..
#ifdef _DEBUG
	m_Gizmo = new Gizmos(eGizmoType::Camera, DXVector3(0.5f, 0.5f, 0.5f));
	m_Gizmo->SetGameObject(m_GameObject);
#endif

	SetLens(0.25f * KH_MATH::Pi, (float)m_ClientSize->x, (float)m_ClientSize->y, 1.0f, 1000.0f);
	LookAt(XMFLOAT3(0.0f, 7.5f, -18.0f), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1.0f, 0));
	WorldUpDown(10.0f);
	Walk(-100.0f);
	Pitch(0.2f);
}

void Camera::Update(float dTime)
{
	mPrevView = mView;

	XMVECTOR R = mRight;
	XMVECTOR U = mUp;
	XMVECTOR L = mLook;
	XMVECTOR P = mPosition;

	// Keep camera's axes orthogonal to each other and of unit length.
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = XMVector3Cross(U, L);

	// Fill in the view matrix entries.
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	mRight = R;
	mUp = U;
	mLook = L;

	mView(0, 0) = mRight.x;
	mView(1, 0) = mRight.y;
	mView(2, 0) = mRight.z;
	mView(3, 0) = x;

	mView(0, 1) = mUp.x;
	mView(1, 1) = mUp.y;
	mView(2, 1) = mUp.z;
	mView(3, 1) = y;

	mView(0, 2) = mLook.x;
	mView(1, 2) = mLook.y;
	mView(2, 2) = mLook.z;
	mView(3, 2) = z;

	mView(0, 3) = 0.0f;
	mView(1, 3) = 0.0f;
	mView(2, 3) = 0.0f;
	mView(3, 3) = 1.0f;
	
	m_GameObject->GetTransform()->SetPosition(mPosition);

	//m_Frustom->Update(pCamera->View(), pCamera->Proj(), deltaTime);
	//m_Frustom->BuildGeometryBuffers(ViewProj());
}

void Camera::Render(DXMatrix4X4 view, DXMatrix4X4 proj)
{
#ifdef _DEBUG
	if (this == g_MainCamera) return;

	m_Gizmo->Render(view, proj);
#endif
}

void Camera::SetCameraTarget(Transform* target)
{
	m_Target = target;
}

void Camera::SetMove(bool state)
{
	m_Time = 0.0f;
	Is_Move = state;
}

void Camera::Release()
{
	SAFE_DELETE(m_Frustom);
	SAFE_DELETE(m_Gizmo);
}
