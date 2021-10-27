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
};

