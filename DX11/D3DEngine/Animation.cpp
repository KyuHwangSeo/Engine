#include "DLLDefine.h"
#include "SingleTon.h"
#include "GameTimer.h"
#include "EnumDefine.h"
#include "KHParser.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Animation.h"
#include "DeleteDefine.h"

void Animation::Update(float dTime)
{
	for (auto& ani : m_OneAnimation)
	{
		AnimationUpdate(ani.first, ani.second, dTime);
	}
}

void Animation::AnimationUpdate(Transform* nowMesh, AnimationData* nowAni, float dTime)
{
	// �ִϸ��̼� ������ ������� ó������ �ʴ´�..
	if (nowAni == nullptr) return;

	if (m_Loop == false)
	{
		if (nowAni->m_nextPosIndex == 0)
		{
			m_Finish = true;
			return;
		}
	}

	nowAni->m_frameTime += dTime;

	// �ִϸ��̼� KeyFrame üũ..
	CheckKeyFrame(nowAni);

	int nowScaleIndex = nowAni->m_nowScaleIndex;
	int nowPosIndex = nowAni->m_nowPosIndex;
	int nowRotIndex = nowAni->m_nowRotIndex;
	int nextScaleIndex = nowAni->m_nextScaleIndex;
	int nextPosIndex = nowAni->m_nextPosIndex;
	int nextRotIndex = nowAni->m_nextRotIndex;
	int tickFrame = nowAni->m_tickFrame;

	// ���� LocalTM���� ������ ��ȯ ��Ŀ��..
	XMVECTOR localScale;
	XMVECTOR localRot;
	XMVECTOR localPos;

	// ���� �����..
	XMVECTOR nowVec;
	XMVECTOR nextVec;
	float nowFrame;

	// ���� LocalTM���� ��� ��Ҹ� �����Ѵ�..
	XMMatrixDecompose(&localScale, &localRot, &localPos, nowMesh->m_NodeTM);

	// Scale Lerp
	if (nowAni->m_isPosAnimation)
	{
		// ���� �����Ӱ� ���� �����Ӱ��� ����..
		nowVec = nowAni->m_scale[nowScaleIndex]->m_scale;
		nextVec = nowAni->m_scale[nextScaleIndex]->m_scale;
		nowFrame = (tickFrame - nowAni->m_scale[nowScaleIndex]->m_time) / (nowAni->m_scale[nextScaleIndex]->m_time - nowAni->m_scale[nowScaleIndex]->m_time);
		localScale = XMVectorLerp(nowVec, nextVec, nowFrame);

		// ��� ������Ʈ üũ..
		nowMesh->m_UpdateLocal = true;
	}

	// Position Lerp
	if (nowAni->m_isPosAnimation)
	{
		// ���� �����Ӱ� ���� �����Ӱ��� ����..
		nowVec = nowAni->m_position[nowPosIndex]->m_pos;
		nextVec = nowAni->m_position[nextPosIndex]->m_pos;
		nowFrame = (tickFrame - nowAni->m_position[nowPosIndex]->m_time) / (nowAni->m_position[nextPosIndex]->m_time - nowAni->m_position[nowPosIndex]->m_time);
		localPos = XMVectorLerp(nowVec, nextVec, nowFrame);

		// ��� ������Ʈ üũ..
		nowMesh->m_UpdateLocal = true;
	}

	// Rotation Slerp
	if (nowAni->m_isRotAnimation)
	{
		// ���� �����Ӱ� ���� �����Ӱ��� ����..
		nowVec = nowAni->m_rotation[nowRotIndex]->m_rotQT_accumulation;
		nextVec = nowAni->m_rotation[nextRotIndex]->m_rotQT_accumulation;
		nowFrame = (tickFrame - nowAni->m_rotation[nowRotIndex]->m_time) / (nowAni->m_rotation[nextRotIndex]->m_time - nowAni->m_rotation[nowRotIndex]->m_time);
		localRot = XMQuaternionSlerp(nowVec, nextVec, nowFrame);

		// ��� ������Ʈ üũ..
		nowMesh->m_UpdateLocal = true;
	}

	// �ִϸ��̼� ������ ������ �ִϸ��̼ǿ��� ������ TM���� LocalTM ����..
	// ���� �ִϸ��̼ǿ� ������� ���� LocalTM ��Ҹ� ����..
	DXMatrix4X4 ScaleTM = KH_MATH::CreateScale(localScale);
	DXMatrix4X4 RotTM = XMMatrixRotationQuaternion(localRot);
	DXMatrix4X4 TransTM = KH_MATH::CreateTranslation(localPos);

	// ��������� ���� AnimaionTM ���..
	/// ó�� ������ NodeTM�� ���� ���� LocalTM�� �ִϸ��̼� �������� ���� ��ķ� ��ȯ���ִ��� �˾Ҵµ�
	/// �ִϸ��̼� ������ ���� ������ ����� �� ���� ����� LocalTM�̴�..
	/// ��, �ִϸ��̼� ������ ���� ������ ��İ� ���� LocalTM�� �������ϸ� ���� ����� LocalTM�� ���´�..
	nowMesh->m_NodeTM = ScaleTM * RotTM * TransTM;
}

void Animation::CheckKeyFrame(AnimationData* nowAni)
{
	float frameTick = nowAni->m_ticksperFrame / m_FrameSpeed;
	int nowScaleFrame = nowAni->m_nowScaleIndex;
	int nowPosFrame = nowAni->m_nowPosIndex;
	int nowRotFrame = nowAni->m_nowRotIndex;
	int nextScaleFrame = nowAni->m_nextScaleIndex;
	int nextPosFrame = nowAni->m_nextPosIndex;
	int nextRotFrame = nowAni->m_nextRotIndex;
	int lastScaleFrame = nowAni->m_lastScaleFrame;
	int lastPosFrame = nowAni->m_lastPosFrame;
	int lastRotFrame = nowAni->m_lastRotFrame;

	// deltaTime�� oneTickFrame ��ŭ �ð��� ������� tickFrame ������Ʈ..
	if (nowAni->m_frameTime > frameTick)
	{
		// �Ź� tick update�� ���� frameTime �ʱ�ȭ & tickFrame ����..
		nowAni->m_frameTime = 0.0f;
		nowAni->m_tickFrame++;

		// lastFrame�� ������ ��� �ʱ�ȭ..
		if (nowAni->m_tickFrame > nowAni->m_totalFrame)
		{
			nowAni->m_tickFrame = 0;
			nowScaleFrame = 0;
			nowPosFrame = 0;
			nowRotFrame = 0;
			nextScaleFrame = 1;
			nextPosFrame = 1;
			nextRotFrame = 1;
		}

		/// �Ѱ��� Mesh�� ������ ���� Position & Rotation KeyFrame Time ��..
		/// Position�� Rotation�� �Ѵ� ������� ���� Mesh�̴ϱ� Mesh �������� ������ ������ ���� ������?
		if (nowAni->m_isPosAnimation)
		{
			if (nowAni->m_tickFrame > nowAni->m_position[nextPosFrame]->m_time)
			{
				nowPosFrame++;

				// ������ �����ӿ��� �ٷ� �Ѿ�� �ȵǴ� ������ ������ ����..
				if (nowPosFrame > lastPosFrame)
				{
					nowPosFrame = lastPosFrame;
				}

				nextPosFrame = nowPosFrame + 1;
				if (nextPosFrame > lastPosFrame)
				{
					nextPosFrame = 0;
				}
			}
		}

		if (nowAni->m_isRotAnimation)
		{
			if (nowAni->m_tickFrame > nowAni->m_rotation[nextRotFrame]->m_time)
			{
				nowRotFrame++;

				// ������ �����ӿ��� �ٷ� �Ѿ�� �ȵǴ� ������ ������ ����..
				if (nowRotFrame > lastRotFrame)
				{
					nowRotFrame = lastRotFrame;
				}

				nextRotFrame = nowRotFrame + 1;
				if (nextRotFrame > lastRotFrame)
				{
					nextRotFrame = 0;
				}
			}
		}

		if (nowAni->m_isScaleAnimation)
		{
			if (nowAni->m_tickFrame > nowAni->m_scale[nextScaleFrame]->m_time)
			{
				nowScaleFrame++;

				// ������ �����ӿ��� �ٷ� �Ѿ�� �ȵǴ� ������ ������ ����..
				if (nowScaleFrame > lastScaleFrame)
				{
					nowScaleFrame = lastScaleFrame;
				}

				nextScaleFrame = nowScaleFrame + 1;
				if (nextScaleFrame > lastScaleFrame)
				{
					nextScaleFrame = 0;
				}
			}
		}
	}

	/// ��������� ���� ������ ������ �Է�..
	nowAni->m_nowScaleIndex = nowScaleFrame;
	nowAni->m_nowPosIndex = nowPosFrame;
	nowAni->m_nowRotIndex = nowRotFrame;
	nowAni->m_nextScaleIndex = nextScaleFrame;
	nowAni->m_nextPosIndex = nextPosFrame;
	nowAni->m_nextRotIndex = nextRotFrame;
}

void Animation::AddAnimationData(GameObject* aniMesh, AnimationData* aniData)
{
	// �ִϸ��̼��� �������..
	if (aniData == nullptr) return;

	// �ش� Mesh �ִϸ��̼� 0������ ���� �ʱⰪ ����..
	DXMatrix4X4 pos = KH_MATH::CreateTranslation(aniData->m_position[0]->m_pos);
	DXMatrix4X4 rot = XMMatrixRotationQuaternion(aniData->m_rotation[0]->m_rotQT_accumulation);

	// �ִϸ��̼� ����� �ʱ�ȭ �����Ͱ� �ʿ��ؼ� ����..
	DXMatrix4X4 nodeTM = rot * pos;

	m_ResetData.insert(make_pair(aniMesh->GetTransform(), nodeTM));

	// �ش� ������Ʈ Transform�� ����..
	SetTransform(aniMesh->GetTransform(), nodeTM);

	AnimationData* newData = new AnimationData(*aniData);

	m_OneAnimation.insert(make_pair(aniMesh->GetTransform(), newData));
}

void Animation::SetAnimation(bool loop)
{
	m_Finish = false;
	m_Loop = loop;

	// �ִϸ��̼� ������ Mesh �ʱⰪ ����..
	for (auto& ani : m_ResetData)
	{
		SetTransform(ani.first, ani.second);
	}
}

void Animation::SetTransform(Transform* aniMesh, DXMatrix4X4& originalData)
{
	aniMesh->m_NodeTM = originalData;
}

void Animation::ResetAnimation()
{
	// �ִϸ��̼� ����� �ִϸ��̼� ������ �ʱ�ȭ..
	for (auto& ani : m_OneAnimation)
	{
		if (ani.second == nullptr) continue;

		ResetData(ani.second);
	}

	// Speed Reset
	m_FrameSpeed = 1.0f;
}

void Animation::SetAnimationSpeed(float speed)
{
	m_FrameSpeed = speed;
}

void Animation::AddAnimationSpeed(float speed)
{
	if (m_FrameSpeed < 0.0f)
	{
		m_FrameSpeed = 0.0f;
	}

	m_FrameSpeed += speed;
}

float Animation::GetAnimationSpeed()
{
	return m_FrameSpeed;
}

void Animation::ResetData(AnimationData* nowAni)
{
	nowAni->m_frameTime = 0.0f;
	nowAni->m_tickFrame = 0;
	nowAni->m_nowScaleIndex = 0;
	nowAni->m_nowPosIndex = 0;
	nowAni->m_nowRotIndex = 0;
	nowAni->m_nextScaleIndex = 1;
	nowAni->m_nextPosIndex = 1;
	nowAni->m_nextRotIndex = 1;
}

void Animation::Release()
{
	for (auto& k : m_OneAnimation)
	{
		SAFE_DELETE(k.second);
	}

	m_ResetData.clear();
	m_OneAnimation.clear();
}
