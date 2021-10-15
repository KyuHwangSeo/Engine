#include "EngineDLL.h"
#include "D3DDefine.h"
#include <DirectXMath.h>
#include "EnumDefine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Shader.h"
#include "Gizmos.h"

Transform::Transform()
{
	SetName<Transform>();
}

Transform::~Transform()
{

}

ENGINE_DLL void Transform::SetScale(DXVector3 _scale)
{
	m_LocalRot._11 *= _scale.x;
	m_LocalRot._22 *= _scale.y;
	m_LocalRot._33 *= _scale.z;

	m_Scale *= _scale;

	m_UpdateConvert = true;
}

ENGINE_DLL void Transform::SetScale(float x, float y, float z)
{
	m_LocalRot._11 *= x;
	m_LocalRot._22 *= y;
	m_LocalRot._33 *= z;

	m_Scale *= DXVector3(x, y, z);

	m_UpdateConvert = true;
}

ENGINE_DLL void Transform::SetScaleX(float x)
{
	m_LocalRot._11 = x;

	m_Scale.x = x;

	m_UpdateConvert = true;
}

ENGINE_DLL void Transform::SetScaleY(float y)
{
	m_LocalRot._22 = y;

	m_Scale.y = y;

	m_UpdateConvert = true;
}

ENGINE_DLL void Transform::ChangeScale(DXVector3 _scale)
{
	m_LocalRot._11 = _scale.x;
	m_LocalRot._22 = _scale.y;
	m_LocalRot._33 = _scale.z;

	m_Scale = _scale;

	m_UpdateConvert = true;
}

ENGINE_DLL void Transform::ChangeScale(float x, float y, float z)
{
	m_LocalRot._11 = x;
	m_LocalRot._22 = y;
	m_LocalRot._33 = z;

	m_Scale = DXVector3(x, y, z);

	m_UpdateConvert = true;
}

ENGINE_DLL void Transform::SetPosition(DXVector3 pos)
{
	m_Pos = pos;

	m_LocalRot._41 = pos.x;
	m_LocalRot._42 = pos.y;
	m_LocalRot._43 = pos.z;
	m_UpdateConvert = true;
}

ENGINE_DLL void Transform::SetPosition(float x, float y, float z)
{
	m_Pos = DXVector3(x,y,z);

	m_LocalRot._41 = x;
	m_LocalRot._42 = y;
	m_LocalRot._43 = z;
	m_UpdateConvert = true;
}

ENGINE_DLL void Transform::SetPositionX(float x)
{
	m_Pos.x = x;

	m_LocalRot._41 = x;
	m_UpdateConvert = true;
}

ENGINE_DLL void Transform::SetPositionY(float y)
{
	m_Pos.y = y;

	m_LocalRot._42 = y;
	m_UpdateConvert = true;
}

ENGINE_DLL void Transform::SetPositionZ(float z)
{
	m_Pos.z = z;

	m_LocalRot._43 = z;
	m_UpdateConvert = true;
}

ENGINE_DLL void Transform::SetRotate(DXVector3 _rot)
{
	const auto yaw = _rot.y * KH_MATH::Pi / 180.0f;
	const auto pitch = _rot.x * KH_MATH::Pi / 180.0f;
	const auto roll = _rot.z * KH_MATH::Pi / 180.0f;

	Quaternion q = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

	m_UpdateConvert = true;

	m_OffsetTM = XMMatrixRotationQuaternion(q);
}

ENGINE_DLL void Transform::SetRotate(float x, float y, float z)
{
	const auto yaw = y * KH_MATH::Pi / 180.0f;
	const auto pitch = x * KH_MATH::Pi / 180.0f;
	const auto roll = z * KH_MATH::Pi / 180.0f;

	Quaternion q = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

	m_UpdateConvert = true;

	m_OffsetTM = XMMatrixRotationQuaternion(q);
}

ENGINE_DLL void Transform::RotateLocal(DXVector3 ypr)
{
	m_Rot += ypr;

	const auto yaw = m_Rot.y * KH_MATH::Pi / 180.0f;
	const auto pitch = m_Rot.x * KH_MATH::Pi / 180.0f;
	const auto roll = m_Rot.z * KH_MATH::Pi / 180.0f;

	Quaternion q = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

	m_UpdateConvert = true;

	m_LocalRot = XMMatrixRotationQuaternion(q) * m_LocalRot;
}

ENGINE_DLL void Transform::RotateLocal(float x, float y, float z)
{
	m_Rot += DXVector3(x, y, z);

	const auto yaw = m_Rot.y * KH_MATH::Pi / 180.0f;
	const auto pitch = m_Rot.x * KH_MATH::Pi / 180.0f;
	const auto roll = m_Rot.z * KH_MATH::Pi / 180.0f;

	Quaternion q = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

	m_UpdateConvert = true;

	m_LocalRot = XMMatrixRotationQuaternion(q) * m_LocalRot;
}

ENGINE_DLL void Transform::MoveLocal(float _x, float _y, float _z)
{
	DXVector3 localMove;

	localMove.x = _x * m_LocalRot._11 + _y * m_LocalRot._21 + _z * m_LocalRot._31;
	localMove.y = _x * m_LocalRot._12 + _y * m_LocalRot._22 + _z * m_LocalRot._32;
	localMove.z = _x * m_LocalRot._13 + _y * m_LocalRot._23 + _z * m_LocalRot._33;

	m_LocalRot._41 += localMove.x;
	m_LocalRot._42 += localMove.y;
	m_LocalRot._43 += localMove.z;

	m_UpdateConvert = true;
}

ENGINE_DLL void Transform::MoveLocal(DXVector3 _move)
{
	DXVector3 localMove;

	localMove.x = _move.x * m_LocalRot._11 + _move.y * m_LocalRot._21 + _move.z * m_LocalRot._31;
	localMove.y = _move.x * m_LocalRot._12 + _move.y * m_LocalRot._22 + _move.z * m_LocalRot._32;
	localMove.z = _move.x * m_LocalRot._13 + _move.y * m_LocalRot._23 + _move.z * m_LocalRot._33;

	m_LocalRot._41 += localMove.x;
	m_LocalRot._42 += localMove.y;
	m_LocalRot._43 += localMove.z;

	m_UpdateConvert = true;
}

ENGINE_DLL void Transform::MoveOffset(DXVector3 _move)
{
	DXVector3 offsetMove;

	offsetMove.x = _move.x * m_OffsetTM._11 + _move.y * m_OffsetTM._21 + _move.z * m_OffsetTM._31;
	offsetMove.y = _move.x * m_OffsetTM._12 + _move.y * m_OffsetTM._22 + _move.z * m_OffsetTM._32;
	offsetMove.z = _move.x * m_OffsetTM._13 + _move.y * m_OffsetTM._23 + _move.z * m_OffsetTM._33;

	m_LocalRot._41 += offsetMove.x;
	m_LocalRot._42 += offsetMove.y;
	m_LocalRot._43 += offsetMove.z;

	m_UpdateConvert = true;
}

ENGINE_DLL void Transform::MoveOffset(float x, float y, float z)
{
	DXVector3 offsetMove;

	offsetMove.x = x * m_OffsetTM._11 + y * m_OffsetTM._21 + z * m_OffsetTM._31;
	offsetMove.y = x * m_OffsetTM._12 + y * m_OffsetTM._22 + z * m_OffsetTM._32;
	offsetMove.z = x * m_OffsetTM._13 + y * m_OffsetTM._23 + z * m_OffsetTM._33;

	m_LocalRot._41 += offsetMove.x;
	m_LocalRot._42 += offsetMove.y;
	m_LocalRot._43 += offsetMove.z;

	m_UpdateConvert = true;
}

ENGINE_DLL void Transform::MoveWorld(float _x, float _y, float _z)
{
	m_LocalRot._41 += _x;
	m_LocalRot._42 += _y;
	m_LocalRot._43 += _z;
	m_UpdateConvert = true;
}

ENGINE_DLL void Transform::MoveWorld(DXVector3 _move)
{
	m_LocalRot._41 += _move.x;
	m_LocalRot._42 += _move.y;
	m_LocalRot._43 += _move.z;
	m_UpdateConvert = true;
}

ENGINE_DLL void Transform::ResetTM(DXVector3 targetPos)
{
	// 현재 위치와 이전 프레임 위치..
	DXVector3 nowPos = m_WorldTM.GetRow(3);
	DXVector3 prevPos = m_PrevWorldTM.GetRow(3);

	// 타겟과의 거리 비교..
	float nowDistance = KH_MATH::GetDistance(targetPos, nowPos);
	float prevDistance = KH_MATH::GetDistance(targetPos, prevPos);

	// 현재 위치가 멀다면 이전 프레임 위치로 돌리지 않는다..
	if (nowDistance > prevDistance) return;

	m_WorldTM = m_PrevWorldTM;
	m_LocalRot = m_PrevLocalTM;

	m_UpdateConvert = true;

	UpdateConvert();
}

void Transform::Start()
{
#ifdef _DEBUG
	m_Gizmo = new Gizmos(eGizmoType::Axis, DXVector3(5.0f, 5.0f, 5.0f));
	m_Gizmo->SetGameObject(m_GameObject);
#endif
}

void Transform::Render(DXMatrix4X4 view, DXMatrix4X4 proj)
{
#ifdef _DEBUG
	if (m_GameObject->GetPick())
		m_Gizmo->Render(view, proj);
#endif
}

void Transform::Update(float dTime)
{
	PrevUpdate();

	// 로컬 업데이트시 Hierarchy 연산 과정..
	if (m_UpdateLocal)
	{
		UpdateLocal();
	}

	// 변환행렬 업데이트시 Hierarchy 연산 과정..
	if (m_UpdateConvert)
	{
		UpdateConvert();
	}

	// 로컬 * 변환행렬 * 월드변환행렬 -> 오브젝트의 월드
	// Hierarchy 구조에서 Scale을 제외한 변환 행렬은 상속받는다..
	m_WorldTM = m_LocalTM * m_ConvertTM;
}

void Transform::PrevUpdate()
{
	DXMatrix4X4 prevRot = m_LocalRot;
	prevRot.m[3][0] = m_ConvertTM.m[3][0];
	prevRot.m[3][1] = m_ConvertTM.m[3][1];
	prevRot.m[3][2] = m_ConvertTM.m[3][2];

	m_PrevLocalTM = prevRot;
	m_PrevWorldTM = m_WorldTM;
}

void Transform::UpdateLocal()
{
	/// 현 노드의 LocalTM
	// 1) 최상위 노드일 경우
	//  -> 자신의 WorldTM = 자신의 LocalTM(변환행렬을 곱한 LocalTM)
	// 2) 노드의 부모가 있을 경우
	//  -> 자신의 WorldTM = 자신의 LocalTM(변환행렬을 곱한 LocalTM) * 부모의 WorldTM
	if (m_Parent != nullptr)
	{
		m_LocalTM = m_NodeTM * m_Parent->m_LocalTM;
	}
	else
	{
		m_LocalTM = m_NodeTM;
	}

	// 연결된 자식들 LocalTM 업데이트..
	UpdateChildLocal(this);

	// 업데이트 후 플래그 변경..
	m_UpdateLocal = false;
}

void Transform::UpdateConvert()
{
	if (m_Parent != nullptr)
	{
		m_ConvertTM = m_OffsetTM * m_LocalRot * m_Parent->m_ConvertTM;
	}
	else
	{
		m_ConvertTM = m_OffsetTM * m_LocalRot;
	}

	// 연결된 자식들 ConvertTM 업데이트..
	UpdateChildConvert(this);

	// 업데이트 후 플래그 변경..
	m_UpdateConvert = false;
}

void Transform::UpdateChildLocal(Transform* parent)
{
	/// 재귀를 이용한 노드 업데이트
	// LocalTM이 변한 노드의 자식들만 업데이트시켜주자..
	for (Transform* child : parent->m_Child)
	{
		child->m_LocalTM = child->m_NodeTM * parent->m_LocalTM;

		UpdateChildLocal(child);

		// 업데이트후 플래그 변경..
		child->m_UpdateLocal = false;
	}
}

void Transform::UpdateChildConvert(Transform* parent)
{
	for (Transform* child : parent->m_Child)
	{
		child->m_ConvertTM = child->m_LocalRot * parent->m_ConvertTM;

		UpdateChildConvert(child);

		// 업데이트후 플래그 변경..
		child->m_UpdateConvert = false;
	}
}

void Transform::SetLocalTM(DXMatrix4X4 _tm)
{
	m_LocalTM = _tm;
}

void Transform::SetNodeTM(DXMatrix4X4 _tm)
{
	m_NodeTM = _tm;
}

ENGINE_DLL DXMatrix4X4 Transform::GetWorldTM()
{
	return m_WorldTM;
}

DXMatrix4X4 Transform::GetPrevWorldTM()
{
	return m_PrevWorldTM;
}

DXMatrix4X4 Transform::GetLocalTM()
{
	return m_LocalTM;
}

ENGINE_DLL DXVector3 Transform::GetScale()
{
	return DXVector3(m_WorldTM._11, m_WorldTM._22, m_WorldTM._33);
}

ENGINE_DLL DXVector3 Transform::GetPosition()
{
	return DXVector3(m_WorldTM._41, m_WorldTM._42, m_WorldTM._43);
}

ENGINE_DLL DXVector3 Transform::GetRotation()
{
	return DXVector3(m_WorldTM._31, m_WorldTM._32, m_WorldTM._33);
}

void Transform::Release()
{
	m_Parent = nullptr;
	m_Child.clear();
}
