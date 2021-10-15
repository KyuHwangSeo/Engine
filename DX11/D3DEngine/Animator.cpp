#include "EngineDLL.h"
#include "SingleTon.h"
#include "GameTimer.h"
#include "EnumDefine.h"
#include "KHParser.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Animation.h"
#include "Animator.h"
#include "DeleteDefine.h"

Animator::Animator()
	:m_Play(false)
{
	SetName<Animator>();
}

Animator::~Animator()
{
}

void Animator::Update(float dTime)
{
	// 재생중인 현재 애니메이션 업데이트..
	if (m_Play)
	{
		m_Animations[m_NowAni]->Update(dTime);

		if (m_Animations[m_NowAni]->GetEndAnimation())
		{
			m_Play = false;
			m_Animations[m_NowAni]->ResetAnimation();
		}
	}
}

void Animator::Release()
{
	for (auto& k : m_Animations)
	{
		RELEASE_COM(k.second);
	}

	m_Animations.clear();
}

ENGINE_DLL void Animator::PlayAnimation(string aniKey, bool loop, bool overlap)
{
	// 이미 재생중인 애니메이션..
	if (overlap == false)
	{
		if (m_NowAni == aniKey) return;
	}

	// 해당 키의 애니메이션이 없다면 처리하지 않는다..
	if (m_Animations.find(aniKey) == m_Animations.end()) return;


	if (m_Play)
		m_Animations[m_NowAni]->ResetAnimation();

	m_NowAni = aniKey;
	m_Animations[m_NowAni]->SetAnimation(loop);

	m_Play = true;
}

ENGINE_DLL void Animator::ResumeAnimation()
{
	m_Play = true;
}

ENGINE_DLL void Animator::StopAnimation()
{
	m_Play = false;
}

ENGINE_DLL void Animator::AddAnimation(string aniKey, Animation* newAni)
{
	// 키값과 애니메이션 이름을 맞춰주자..
	newAni->SetName(aniKey);

	m_Animations.insert(make_pair(aniKey, newAni));
}

ENGINE_DLL void Animator::SetAnimationSpeed(float speed)
{
	m_Animations[m_NowAni]->SetAnimationSpeed(speed);
}

ENGINE_DLL void Animator::AddAnimationSpeed(float speed)
{
	m_Animations[m_NowAni]->AddAnimationSpeed(speed);
}

ENGINE_DLL void Animator::NextAnimation(bool loop)
{
	auto ani = m_Animations.find(m_NowAni)++;

	if (ani == m_Animations.end())
	{
		ani = m_Animations.begin();
	}

	// 이전 애니메이션 초기화..
	m_Animations[m_NowAni]->ResetAnimation();

	// 현재 애니메이션 변경..
	m_NowAni = ani->first;
	m_Animations[m_NowAni]->SetAnimation(loop);
}

void Animator::AddMeshObject(GameObject* obj)
{
	// 일단 모든 오브젝트 보관..
	m_Objects.emplace_back(obj);
}

float Animator::GetAnimationSpeed()
{
	return	m_Animations[m_NowAni]->GetAnimationSpeed();
}

GameObject* Animator::GetMeshObject(int index)
{
	return m_Objects[index];
}
