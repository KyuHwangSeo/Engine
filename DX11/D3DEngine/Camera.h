#pragma once
#include "CameraFrustom.h"

class Camera : public Component
{
public:
	Camera(eCameraType cameraType = eCameraType::NormalCam);
	~Camera();

	// Get/Set world camera position.
	DXVector3 GetPosition();

	// Get camera basis vectors.
	DXVector3 GetRight();
	DXVector3 GetUp();
	DXVector3 GetLook();
	DXVector3 GetTargetPos();
	DXVector3 GetTargetRot();

	// Get frustum properties.
	float GetNearZ();
	float GetFarZ();
	float GetAspect();
	float GetFovY();
	float GetFovX();

	// Get near and far plane dimensions in view space coordinates.
	float GetNearWindowWidth();
	float GetNearWindowHeight();
	float GetFarWindowWidth();
	float GetFarWindowHeight();

	// Set frustum.
	void SetLens(float fovY, float width, float height, float zn, float zf);

	// Define camera space via LookAt parameters.
	void LookAt(DXVector3 pos, DXVector3 target, DXVector3 worldUp);
	void LookAt_3_Person_FreeView(DXVector3 offsetPos, float offset);
	void LookAt_3_Person(DXVector3 offsetPos, float offset, float offsetY);
	void LookAt_3_Person_MoveX(DXVector3 offsetPos);
	void LookAt_1_Person(DXVector3 offsetPos, float offset);

	// Get View/Proj matrices.
	DXMatrix4X4 View() const;
	DXMatrix4X4 PrevView() const;
	DXMatrix4X4 Proj() const;
	DXMatrix4X4 ViewProj() const;
	DXMatrix4X4 Ortho() const;
	DXVector3 GetQuaternionRot();

	// Strafe/Walk the camera a distance d.
	void Strafe(float d);
	void Walk(float d);
	void WorldUpDown(float d);

	// Rotate the camera.
	void Pitch(float angle);
	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);

	void SmoothMove1(DXVector3& before, DXVector3& after, float deltaTime);
	void SmoothMove2(DXVector3& beforePos, DXVector3 beforeRight, float deltaTime);
	void SmoothMove3(DXVector3 beforePos, float deltaTime);

	void SetType(eCameraType type);

	// After modifying camera position/orientation, call to rebuild the view matrix.
	void Start();
	void Update(float dTime);
	void Render(DXMatrix4X4 view, DXMatrix4X4 proj);

	DLL_DECLSPEC void SetCameraTarget(Transform* target);
	void SetMove(bool state);

	void Release();

	XNA::Frustum GetFrustum() { return m_Frustom->GetFrustum(); }
private:
	// Camera coordinate system with coordinates relative to world space.
	//XMFLOAT3 mPosition;
	DXVector3 mRight;
	DXVector3 mUp;
	DXVector3 mLook;
	DXVector3 mPosition;

	// Cache frustum properties.
	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mNearWindowHeight;
	float mFarWindowHeight;

	float m_Time = 0;
	bool Is_Move = false;

	POINT* m_ClientSize;

	// Cache View/Proj matrices.
	DXMatrix4X4 mView;
	DXMatrix4X4 mPrevView;
	DXMatrix4X4 mProj;
	DXMatrix4X4 mOrtho;

	CameraFrustom* m_Frustom;
	Gizmos* m_Gizmo;

public:
	eCameraType m_CameraType;
	float m_Speed = 0;
	float m_Angle = 0;

	DLL_DECLSPEC static Camera* g_MainCamera;
	DLL_DECLSPEC static Transform* m_Target;
};

