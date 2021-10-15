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
	// �� ������Ʈ�� ���õ� �ִϸ��̼ǵ�..
	unordered_map<std::string, Animation*> m_Animations;
	vector<GameObject*> m_Objects;

	// ������� �ִϸ��̼� Ű��..
	std::string m_NowAni;

	// �ִϸ��̼� ����..
	bool m_Play;
};
