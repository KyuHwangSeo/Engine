#pragma once
/// 현재로썬 애니메이터를 사용하려면 Animation간의 동적으로 추가 가능한 FSM이 필요로 하지만 현재는 불가능.. 
/// 추후에 가능하면 추가하도록 하자.. 일단은 지금 상태에선 Animator가 곧 AnimationController 역할..


class Animator : public Component
{
public:
	Animator();
	~Animator();

public:
	void Update(float dTime);
	void Release();
	
	ENGINE_DLL void PlayAnimation(string aniKey, bool loop = true, bool overlap = false);
	ENGINE_DLL void ResumeAnimation();
	ENGINE_DLL void StopAnimation();
	ENGINE_DLL void AddAnimation(string aniKey, Animation* newAni);
	ENGINE_DLL void SetAnimationSpeed(float speed);
	ENGINE_DLL void AddAnimationSpeed(float speed);
	ENGINE_DLL void NextAnimation(bool loop = true);
	void AddMeshObject(GameObject* obj);

	float GetAnimationSpeed();
	GameObject* GetMeshObject(int index);

	std::string GetKey() { return m_NowAni; }
	bool GetState() { return m_Play; }

private:
	// 한 오브젝트에 관련된 애니메이션들..
	unordered_map<std::string, Animation*> m_Animations;
	vector<GameObject*> m_Objects;

	// 재생중인 애니메이션 키값..
	std::string m_NowAni;

	// 애니메이션 상태..
	bool m_Play;
};
