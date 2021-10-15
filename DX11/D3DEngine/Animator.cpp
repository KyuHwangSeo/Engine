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
	// ������� ���� �ִϸ��̼� ������Ʈ..
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
	// �̹� ������� �ִϸ��̼�..
	if (overlap == false)
	{
		if (m_NowAni == aniKey) return;
	}

	// �ش� Ű�� �ִϸ��̼��� ���ٸ� ó������ �ʴ´�..
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
	// Ű���� �ִϸ��̼� �̸��� ��������..
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

	// ���� �ִϸ��̼� �ʱ�ȭ..
	m_Animations[m_NowAni]->ResetAnimation();

	// ���� �ִϸ��̼� ����..
	m_NowAni = ani->first;
	m_Animations[m_NowAni]->SetAnimation(loop);
}

void Animator::AddMeshObject(GameObject* obj)
{
	// �ϴ� ��� ������Ʈ ����..
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
