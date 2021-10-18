#pragma once
/// <summary>
/// Animation Class
/// </summary>
/// 
/// 현재 키값을 기준으로 애니메이션 설정
/// 
/// - 추가해야 할 사항 -
/// 애니메이션 변경시 이전 애니메이션 마지막 프레임 기준 현재 애니메이션 시작 프레임까지 보간하기

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
	// 한개의 애니메이션..
	unordered_map <Transform*, ParserData::OneAnimation*> m_OneAnimation;

	// 원본 데이터 값..
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