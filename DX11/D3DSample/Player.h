#pragma once
class Player : public Component
{
public:
	Player();
	~Player();

public:
	void Start();
	void Update(float dTime);
	void LateUpdate(float dTime);
	void FinalUpdate(float dTime);

	void Release();

	void CollisionStart(Collision collsion);
	void CollisionStay(Collision collsion);
	void CollisionExit(Collision collsion);

private:
	int m_Hp;
	int m_Att;
};

