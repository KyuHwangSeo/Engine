#pragma once
class IManager
{
public:
	IManager() = default;
	virtual ~IManager() = default;

public:
	virtual void Update(float dTime) abstract;
	virtual void Render(DXMatrix4X4 view, DXMatrix4X4 proj) abstract;
	virtual void AddObject(GameObject* obj) abstract;

	virtual GameObject* FindObject(string objName) abstract;
};

