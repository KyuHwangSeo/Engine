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
	// 애니메이션 정보가 없을경우 처리하지 않는다..
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

	// 애니메이션 KeyFrame 체크..
	CheckKeyFrame(nowAni);

	int nowScaleIndex = nowAni->m_nowScaleIndex;
	int nowPosIndex = nowAni->m_nowPosIndex;
	int nowRotIndex = nowAni->m_nowRotIndex;
	int nextScaleIndex = nowAni->m_nextScaleIndex;
	int nextPosIndex = nowAni->m_nextPosIndex;
	int nextRotIndex = nowAni->m_nextRotIndex;
	int tickFrame = nowAni->m_tickFrame;

	// 현재 LocalTM에서 추출할 변환 행렬요소..
	XMVECTOR localScale;
	XMVECTOR localRot;
	XMVECTOR localPos;

	// 보간 결과값..
	XMVECTOR nowVec;
	XMVECTOR nextVec;
	float nowFrame;

	// 현재 LocalTM에서 행렬 요소를 추출한다..
	XMMatrixDecompose(&localScale, &localRot, &localPos, nowMesh->m_NodeTM);

	// Scale Lerp
	if (nowAni->m_isPosAnimation)
	{
		// 현재 프레임과 다음 프레임간의 보간..
		nowVec = nowAni->m_scale[nowScaleIndex]->m_scale;
		nextVec = nowAni->m_scale[nextScaleIndex]->m_scale;
		nowFrame = (tickFrame - nowAni->m_scale[nowScaleIndex]->m_time) / (nowAni->m_scale[nextScaleIndex]->m_time - nowAni->m_scale[nowScaleIndex]->m_time);
		localScale = XMVectorLerp(nowVec, nextVec, nowFrame);

		// 노드 업데이트 체크..
		nowMesh->m_UpdateLocal = true;
	}

	// Position Lerp
	if (nowAni->m_isPosAnimation)
	{
		// 현재 프레임과 다음 프레임간의 보간..
		nowVec = nowAni->m_position[nowPosIndex]->m_pos;
		nextVec = nowAni->m_position[nextPosIndex]->m_pos;
		nowFrame = (tickFrame - nowAni->m_position[nowPosIndex]->m_time) / (nowAni->m_position[nextPosIndex]->m_time - nowAni->m_position[nowPosIndex]->m_time);
		localPos = XMVectorLerp(nowVec, nextVec, nowFrame);

		// 노드 업데이트 체크..
		nowMesh->m_UpdateLocal = true;
	}

	// Rotation Slerp
	if (nowAni->m_isRotAnimation)
	{
		// 현재 프레임과 다음 프레임간의 보간..
		nowVec = nowAni->m_rotation[nowRotIndex]->m_rotQT_accumulation;
		nextVec = nowAni->m_rotation[nextRotIndex]->m_rotQT_accumulation;
		nowFrame = (tickFrame - nowAni->m_rotation[nowRotIndex]->m_time) / (nowAni->m_rotation[nextRotIndex]->m_time - nowAni->m_rotation[nowRotIndex]->m_time);
		localRot = XMQuaternionSlerp(nowVec, nextVec, nowFrame);

		// 노드 업데이트 체크..
		nowMesh->m_UpdateLocal = true;
	}

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

	// deltaTime이 oneTickFrame 만큼 시간이 지날경우 tickFrame 업데이트..
	if (nowAni->m_frameTime > frameTick)
	{
		// 매번 tick update시 마다 frameTime 초기화 & tickFrame 증가..
		nowAni->m_frameTime = 0.0f;
		nowAni->m_tickFrame++;

		// lastFrame에 도달할 경우 초기화..
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

		/// 한개의 Mesh의 프레임 기준 Position & Rotation KeyFrame Time 비교..
		/// Position과 Rotation이 둘다 있을경우 같은 Mesh이니까 Mesh 내에서의 프레임 간격은 같지 않을까?
		if (nowAni->m_isPosAnimation)
		{
			if (nowAni->m_tickFrame > nowAni->m_position[nextPosFrame]->m_time)
			{
				nowPosFrame++;

				// 마지막 프레임에서 바로 넘어가면 안되니 마지막 프레임 유지..
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

				// 마지막 프레임에서 바로 넘어가면 안되니 마지막 프레임 유지..
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

				// 마지막 프레임에서 바로 넘어가면 안되니 마지막 프레임 유지..
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

	/// 결과적으로 나온 프레임 데이터 입력..
	nowAni->m_nowScaleIndex = nowScaleFrame;
	nowAni->m_nowPosIndex = nowPosFrame;
	nowAni->m_nowRotIndex = nowRotFrame;
	nowAni->m_nextScaleIndex = nextScaleFrame;
	nowAni->m_nextPosIndex = nextPosFrame;
	nowAni->m_nextRotIndex = nextRotFrame;
}

void Animation::AddAnimationData(GameObject* aniMesh, AnimationData* aniData)
{
	// 애니메이션이 없을경우..
	if (aniData == nullptr) return;

	// 해당 Mesh 애니메이션 0프레임 기준 초기값 설정..
	DXMatrix4X4 pos = KH_MATH::CreateTranslation(aniData->m_position[0]->m_pos);
	DXMatrix4X4 rot = XMMatrixRotationQuaternion(aniData->m_rotation[0]->m_rotQT_accumulation);

	// 애니메이션 변경시 초기화 데이터가 필요해서 보관..
	DXMatrix4X4 nodeTM = rot * pos;

	m_ResetData.insert(make_pair(aniMesh->GetTransform(), nodeTM));

	// 해당 오브젝트 Transform과 연동..
	SetTransform(aniMesh->GetTransform(), nodeTM);

	AnimationData* newData = new AnimationData(*aniData);

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
