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
	
	DLL_DECLSPEC void PlayAnimation(string aniKey, bool loop = true, bool overlap = false);
	DLL_DECLSPEC void ResumeAnimation();
	DLL_DECLSPEC void StopAnimation();
	DLL_DECLSPEC void AddAnimation(string aniKey, Animation* newAni);
	DLL_DECLSPEC void SetAnimationSpeed(float speed);
	DLL_DECLSPEC void AddAnimationSpeed(float speed);
	DLL_DECLSPEC void NextAnimation(bool loop = true);
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
