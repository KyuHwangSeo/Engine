#include "EngineDLL.h"
#include "SingleTon.h"
#include "GameTimer.h"
#include "EnumDefine.h"
#include "KHMath.h"
#include "../ShareData/ParserData.h"
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

void Animation::AnimationUpdate(Transform* nowMesh, ParserData::OneAnimation* nowAni, float dTime)
{
	// �ִϸ��̼� ������ ������� ó������ �ʴ´�..
	if (nowAni == nullptr) return;

	if (m_Loop == false)
	{
		if (nowAni->m_NextIndex == 0)
		{
			m_Finish = true;
			return;
		}
	}

	nowAni->m_FrameTime += dTime;

	// �ִϸ��̼� KeyFrame üũ..
	CheckKeyFrame(nowAni);

	// ���� Animation Data..
	m_NowAniData = nowAni->m_AniData[nowAni->m_NowIndex];
	m_NextAniData = nowAni->m_AniData[nowAni->m_NextIndex];

	// ���� LocalTM���� ������ ��ȯ ��Ŀ��..
	XMVECTOR localScale;
	XMVECTOR localRot;
	XMVECTOR localPos;

	// ���� �����..
	XMVECTOR nowVec;
	XMVECTOR nextVec;
	float nowFrame = (nowAni->m_TickFrame - m_NowAniData->m_Time) / (m_NextAniData->m_Time - m_NowAniData->m_Time);

	// ���� LocalTM���� ��� ��Ҹ� �����Ѵ�..
	XMMatrixDecompose(&localScale, &localRot, &localPos, nowMesh->m_NodeTM);

	/// Scale Lerp
	// ���� �����Ӱ� ���� �����Ӱ��� ����..
	nowVec = m_NowAniData->m_Scale;
	nextVec = m_NextAniData->m_Scale;
	localScale = XMVectorLerp(nowVec, nextVec, nowFrame);

	// ��� ������Ʈ üũ..
	nowMesh->m_UpdateLocal = true;

	/// Position Lerp
	// ���� �����Ӱ� ���� �����Ӱ��� ����..
	nowVec = m_NowAniData->m_Pos;
	nextVec = m_NextAniData->m_Pos;
	localPos = XMVectorLerp(nowVec, nextVec, nowFrame);

	// ��� ������Ʈ üũ..
	nowMesh->m_UpdateLocal = true;

	/// Rotation Slerp
	// ���� �����Ӱ� ���� �����Ӱ��� ����..
	nowVec = m_NowAniData->m_RotQt;
	nextVec = m_NextAniData->m_RotQt;
	localRot = XMQuaternionSlerp(nowVec, nextVec, nowFrame);

	// ��� ������Ʈ üũ..
	nowMesh->m_UpdateLocal = true;

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

void Animation::CheckKeyFrame(ParserData::OneAnimation* nowAni)
{
	float frameTick = nowAni->m_TicksPerFrame / m_FrameSpeed;
	int nowFrame = nowAni->m_NowIndex;
	int nextFrame = nowAni->m_NextIndex;
	int lastFrame = nowAni->m_EndFrame;

	// deltaTime�� oneTickFrame ��ŭ �ð��� ������� tickFrame ������Ʈ..
	if (nowAni->m_FrameTime > frameTick)
	{
		// �Ź� tick update�� ���� frameTime �ʱ�ȭ & tickFrame ����..
		nowAni->m_FrameTime = 0.0f;
		nowAni->m_TickFrame++;

		// lastFrame�� ������ ��� �ʱ�ȭ..
		if (nowAni->m_TickFrame > nowAni->m_TotalFrame)
		{
			nowAni->m_TickFrame = 0;
			nowFrame = 0;
			nextFrame = 1;
		}

		/// �Ѱ��� Mesh�� ������ ���� Position & Rotation KeyFrame Time ��..
		/// Position�� Rotation�� �Ѵ� ������� ���� Mesh�̴ϱ� Mesh �������� ������ ������ ���� ������?
		if (nowAni->m_TickFrame > nowAni->m_AniData[nextFrame]->m_Time)
		{
			nowFrame++;

			// ������ �����ӿ��� �ٷ� �Ѿ�� �ȵǴ� ������ ������ ����..
			if (nowFrame > lastFrame)
			{
				nowFrame = lastFrame;
			}

			nextFrame = nowFrame + 1;
			if (nextFrame > lastFrame)
			{
				nextFrame = 0;
			}
		}
	}

	/// ��������� ���� ������ ������ �Է�..
	nowAni->m_NowIndex = nowFrame;
	nowAni->m_NextIndex = nextFrame;
}

void Animation::AddAnimationData(GameObject* aniMesh, ParserData::OneAnimation* aniData)
{
	// �ִϸ��̼��� �������..
	if (aniData == nullptr) return;

	// �ش� Mesh �ִϸ��̼� 0������ ���� �ʱⰪ ����..
	DXMatrix4X4 pos = KH_MATH::CreateTranslation(aniData->m_AniData[0]->m_Pos);
	DXMatrix4X4 rot = XMMatrixRotationQuaternion(aniData->m_AniData[0]->m_RotQt);

	// �ִϸ��̼� ����� �ʱ�ȭ �����Ͱ� �ʿ��ؼ� ����..
	DXMatrix4X4 nodeTM = rot * pos;

	m_ResetData.insert(make_pair(aniMesh->GetTransform(), nodeTM));

	// �ش� ������Ʈ Transform�� ����..
	//SetTransform(aniMesh->GetTransform(), nodeTM);

	ParserData::OneAnimation* newData = new ParserData::OneAnimation(*aniData);

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

void Animation::ResetData(ParserData::OneAnimation* nowAni)
{
	nowAni->m_FrameTime = 0.0f;
	nowAni->m_TickFrame = 0;
	nowAni->m_NowIndex = 0;
	nowAni->m_NextIndex = 1;
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
