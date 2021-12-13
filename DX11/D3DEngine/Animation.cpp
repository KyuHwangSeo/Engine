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
	// 애니메이션 정보가 없을경우 처리하지 않는다..
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

	// 애니메이션 KeyFrame 체크..
	CheckKeyFrame(nowAni);

	// 현재 Animation Data..
	m_NowAniData = nowAni->m_AniData[nowAni->m_NowIndex];
	m_NextAniData = nowAni->m_AniData[nowAni->m_NextIndex];

	// 현재 LocalTM에서 추출할 변환 행렬요소..
	XMVECTOR localScale;
	XMVECTOR localRot;
	XMVECTOR localPos;

	// 보간 결과값..
	XMVECTOR nowVec;
	XMVECTOR nextVec;
	float nowFrame = (nowAni->m_TickFrame - m_NowAniData->m_Time) / (m_NextAniData->m_Time - m_NowAniData->m_Time);

	// 현재 LocalTM에서 행렬 요소를 추출한다..
	XMMatrixDecompose(&localScale, &localRot, &localPos, nowMesh->m_NodeTM);

	/// Scale Lerp
	// 현재 프레임과 다음 프레임간의 보간..
	nowVec = m_NowAniData->m_Scale;
	nextVec = m_NextAniData->m_Scale;
	localScale = XMVectorLerp(nowVec, nextVec, nowFrame);

	// 노드 업데이트 체크..
	nowMesh->m_UpdateLocal = true;

	/// Position Lerp
	// 현재 프레임과 다음 프레임간의 보간..
	nowVec = m_NowAniData->m_Pos;
	nextVec = m_NextAniData->m_Pos;
	localPos = XMVectorLerp(nowVec, nextVec, nowFrame);

	// 노드 업데이트 체크..
	nowMesh->m_UpdateLocal = true;

	/// Rotation Slerp
	// 현재 프레임과 다음 프레임간의 보간..
	nowVec = m_NowAniData->m_RotQt;
	nextVec = m_NextAniData->m_RotQt;
	localRot = XMQuaternionSlerp(nowVec, nextVec, nowFrame);

	// 노드 업데이트 체크..
	nowMesh->m_UpdateLocal = true;

	// 애니메이션 정보가 있으면 애니메이션에서 추출한 TM으로 LocalTM 생성..
	// 만약 애니메이션에 없을경우 기존 LocalTM 요소를 대입..
	DXMatrix4X4 ScaleTM = KH_MATH::CreateScale(localScale);
	DXMatrix4X4 RotTM = XMMatrixRotationQuaternion(localRot);
	DXMatrix4X4 TransTM = KH_MATH::CreateTranslation(localPos);

	// 결과적으로 나온 AnimaionTM 계산..
	/// 처음 생각은 NodeTM을 통해 얻은 LocalTM을 애니메이션 정보에서 얻은 행렬로 변환해주는줄 알았는데
	/// 애니메이션 정보를 통해 추출한 행렬이 곧 현재 노드의 LocalTM이다..
	/// 즉, 애니메이션 정보를 통해 추출한 행렬과 기존 LocalTM을 재조합하면 현재 노드의 LocalTM이 나온다..
	nowMesh->m_NodeTM = ScaleTM * RotTM * TransTM;
}

void Animation::CheckKeyFrame(ParserData::OneAnimation* nowAni)
{
	float frameTick = nowAni->m_TicksPerFrame / m_FrameSpeed;
	int nowFrame = nowAni->m_NowIndex;
	int nextFrame = nowAni->m_NextIndex;
	int lastFrame = nowAni->m_EndFrame;

	// deltaTime이 oneTickFrame 만큼 시간이 지날경우 tickFrame 업데이트..
	if (nowAni->m_FrameTime > frameTick)
	{
		// 매번 tick update시 마다 frameTime 초기화 & tickFrame 증가..
		nowAni->m_FrameTime = 0.0f;
		nowAni->m_TickFrame++;

		// lastFrame에 도달할 경우 초기화..
		if (nowAni->m_TickFrame > nowAni->m_TotalFrame)
		{
			nowAni->m_TickFrame = 0;
			nowFrame = 0;
			nextFrame = 1;
		}

		/// 한개의 Mesh의 프레임 기준 Position & Rotation KeyFrame Time 비교..
		/// Position과 Rotation이 둘다 있을경우 같은 Mesh이니까 Mesh 내에서의 프레임 간격은 같지 않을까?
		if (nowAni->m_TickFrame > nowAni->m_AniData[nextFrame]->m_Time)
		{
			nowFrame++;

			// 마지막 프레임에서 바로 넘어가면 안되니 마지막 프레임 유지..
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

	/// 결과적으로 나온 프레임 데이터 입력..
	nowAni->m_NowIndex = nowFrame;
	nowAni->m_NextIndex = nextFrame;
}

void Animation::AddAnimationData(GameObject* aniMesh, ParserData::OneAnimation* aniData)
{
	// 애니메이션이 없을경우..
	if (aniData == nullptr) return;

	// 해당 Mesh 애니메이션 0프레임 기준 초기값 설정..
	DXMatrix4X4 pos = KH_MATH::CreateTranslation(aniData->m_AniData[0]->m_Pos);
	DXMatrix4X4 rot = XMMatrixRotationQuaternion(aniData->m_AniData[0]->m_RotQt);

	// 애니메이션 변경시 초기화 데이터가 필요해서 보관..
	DXMatrix4X4 nodeTM = rot * pos;

	m_ResetData.insert(make_pair(aniMesh->GetTransform(), nodeTM));

	// 해당 오브젝트 Transform과 연동..
	//SetTransform(aniMesh->GetTransform(), nodeTM);

	ParserData::OneAnimation* newData = new ParserData::OneAnimation(*aniData);

	m_OneAnimation.insert(make_pair(aniMesh->GetTransform(), newData));
}

void Animation::SetAnimation(bool loop)
{
	m_Finish = false;
	m_Loop = loop;

	// 애니메이션 설정시 Mesh 초기값 설정..
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
	// 애니메이션 변경시 애니메이션 데이터 초기화..
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
