#pragma once
/// ����ν� �ִϸ����͸� ����Ϸ��� Animation���� �������� �߰� ������ FSM�� �ʿ�� ������ ����� �Ұ���.. 
/// ���Ŀ� �����ϸ� �߰��ϵ��� ����.. �ϴ��� ���� ���¿��� Animator�� �� AnimationController ����..


class Animator : public Component
{
public:
	Animator();
	~Animator();

public:
	void Update(float dTime);
	void Release();
	
	KH_API void PlayAnimation(string aniKey, bool loop = true, bool overlap = false);
	KH_API void ResumeAnimation();
	KH_API void StopAnimation();
	KH_API void AddAnimation(string aniKey, Animation* newAni);
	KH_API void SetAnimationSpeed(float speed);
	KH_API void AddAnimationSpeed(float speed);
	KH_API void NextAnimation(bool loop = true);
	void AddMeshObject(GameObject* obj);

	float GetAnimationSpeed();
	GameObject* GetMeshObject(int index);

	std::string GetKey() { return m_NowAni; }
	bool GetState() { return m_Play; }

private:
	// �� ������Ʈ�� ���õ� �ִϸ��̼ǵ�..
	unordered_map<std::string, Animation*> m_Animations;
	vector<GameObject*> m_Objects;

	// ������� �ִϸ��̼� Ű��..
	std::string m_NowAni;

	// �ִϸ��̼� ����..
	bool m_Play;
};
