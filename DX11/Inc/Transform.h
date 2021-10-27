#pragma once
/// <summary>
/// Transform Class
/// </summary>
/// 
/// 1. ������Ʈ���� �Ѱ��� Transform�� ���Ѵ�
/// 2. �ִϸ��̼ǿ��� NodeTM�� ������Ʈ �����ָ� 
/// ���������� Transform Component���� Local / World ������Ʈ�� ���ִ� ����
/// 3. ������Ʈ�� Hierarchy ������ ��� ����� ���� ������Ʈ�� ��ȯ ����� ��� �޴� ����
/// 
class Gizmos;
class Transform : public Component
{
public:
	DXMatrix4X4 m_LocalRot;			// ���� ȸ�� ���
	DXMatrix4X4 m_OffsetTM;

	DXMatrix4X4 m_ConvertTM;		// ��ȯ ���

	DXMatrix4X4 m_LocalTM;			// ���� TM
	DXMatrix4X4 m_NodeTM;			// ��� TM

	DXMatrix4X4 m_WorldTM;			// ���� TM
	DXMatrix4X4 m_PrevWorldTM;		// ���� ������ ���� TM
	DXMatrix4X4 m_PrevLocalTM;		// ���� ������ ��ȯ TM

	DXVector3 m_Scale = DXVector3::One();
	DXVector3 m_Rot;
	DXVector3 m_Pos;
	
	Transform* m_Parent;
	vector<Transform*>	m_Child;

	bool m_UpdateLocal = false;
	bool m_UpdateConvert = false;
	float angle = 0;
	Gizmos* m_Gizmo;

public:
	// �⺻ ������
	Transform();
	~Transform();

	// Scale ��� ���� �Լ�..
	ENGINE_DLL void SetScale(DXVector3 _scale);
	ENGINE_DLL void SetScale(float x, float y, float z);
	ENGINE_DLL void SetScaleX(float x);
	ENGINE_DLL void SetScaleY(float y);

	ENGINE_DLL void ChangeScale(DXVector3 _scale);
	ENGINE_DLL void ChangeScale(float x, float y, float z);

	// Position ��� ���� �Լ�..
	ENGINE_DLL void SetPosition(DXVector3 pos);
	ENGINE_DLL void SetPosition(float x, float y, float z);
	ENGINE_DLL void SetPositionX(float x);
	ENGINE_DLL void SetPositionY(float y);
	ENGINE_DLL void SetPositionZ(float z);

	// Rotation ��� ���� �Լ�..
	ENGINE_DLL void SetRotate(DXVector3 _rot);
	ENGINE_DLL void SetRotate(float x, float y, float z);

	// Local Rotation �Լ�..
	ENGINE_DLL void RotateLocal(DXVector3 ypr);
	ENGINE_DLL void RotateLocal(float x, float y, float z);

	// Local Move �Լ�..
	ENGINE_DLL void MoveLocal(DXVector3 _move);
	ENGINE_DLL void MoveLocal(float x, float y, float z);
	ENGINE_DLL void MoveOffset(DXVector3 _move);
	ENGINE_DLL void MoveOffset(float x, float y, float z);

	// World Move �Լ�..
	ENGINE_DLL void MoveWorld(DXVector3 _move);
	ENGINE_DLL void MoveWorld(float _x, float _y, float _z);

	ENGINE_DLL void ResetTM(DXVector3 targetPos);

	void Start();
	void Render(DXMatrix4X4 view, DXMatrix4X4 proj);

	// WorldTM Update �Լ�..
	void Update(float dTime);
	void PrevUpdate();

	// LocalTM Update �Լ�..
	void UpdateLocal();
	void UpdateConvert();
	void UpdateChildLocal(Transform* parent);
	void UpdateChildConvert(Transform* parent);

	// LocalTM ���� �Լ�..
	void SetLocalTM(DXMatrix4X4 _tm);
	// NodeTM ���� �Լ�..
	void SetNodeTM(DXMatrix4X4 _tm);

	// WorldTM ��ȯ �Լ�..
	ENGINE_DLL DXMatrix4X4 GetWorldTM();

	// PrevWorldTM ��ȯ �Լ�..
	DXMatrix4X4 GetPrevWorldTM();
	// LocalTM ��ȯ �Լ�..
	DXMatrix4X4 GetLocalTM();

	// Position ��ȯ �Լ�..
	ENGINE_DLL DXVector3 GetScale();
	// Position ��ȯ �Լ�..
	ENGINE_DLL DXVector3 GetPosition();
	// Rotation ��ȯ �Լ�..
	ENGINE_DLL DXVector3 GetRotation();

	// Hierarchy Get & Set
	Transform* GetParent() { return m_Parent; }
	vector<Transform*> GetAllChild() { return m_Child; }
	void SetParent(Transform* parent) { m_Parent = parent; }
	void AddChild(Transform* child) { m_Child.push_back(child); }

	void Release();
};
