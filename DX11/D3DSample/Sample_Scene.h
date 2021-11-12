#pragma once
class Sample_Scene : public Scene
{
public:
	Sample_Scene();
	~Sample_Scene();

public:
	void Create();
	void SceneUpdate(float dTime);

	Transform* m_Transform;
	GameObject* m_Player;
	Material* m_Mat;
	ID3D11ShaderResourceView* diffuese;
	ID3D11ShaderResourceView* gray;

	bool IsTex = true;
};

