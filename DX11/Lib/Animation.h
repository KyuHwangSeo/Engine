#pragma once
/// <summary>
/// Animation Class
/// </summary>
/// 
/// ���� Ű���� �������� �ִϸ��̼� ����
/// 
/// - �߰��ؾ� �� ���� -
/// �ִϸ��̼� ����� ���� �ִϸ��̼� ������ ������ ���� ���� �ִϸ��̼� ���� �����ӱ��� �����ϱ�

class Animation : public Object
{
public:
	Animation() = default;
	~Animation() = default;

public:
	void Update(float dTime);
	void AnimationUpdate(Transform* nowMesh, ParserData::OneAnimation* nowAni, float dTime);
	void CheckKeyFrame(ParserData::OneAnimation* nowAni);

	void AddAnimationData(GameObject* aniMesh, ParserData::OneAnimation* aniData);
	void SetAnimation(bool loop);
	void SetTransform(Transform* aniMesh, DXMatrix4X4& originalData);
	void ResetAnimation();

	void SetAnimationSpeed(float speed);
	void AddAnimationSpeed(float speed);
	void ResetData(ParserData::OneAnimation* nowAni);

	void Release();

	float GetAnimationSpeed();
	bool GetEndAnimation() { return m_Finish; }

private:
	// �Ѱ��� �ִϸ��̼�..
	unordered_map <Transform*, ParserData::OneAnimation*> m_OneAnimation;

	// ���� ������ ��..
	unordered_map <Transform*, DXMatrix4X4> m_ResetData;

	ParserData::OneFrame* m_NowAniData;
	ParserData::OneFrame* m_NextAniData;

	DXVector3 m_NowPos;
	DXVector3 m_NextPos;

	DXVector4 m_NowRot;
	DXVector4 m_NextRot;

	float m_FrameSpeed = 1.0f;

	bool m_Loop = false;
	bool m_Finish = false;
};