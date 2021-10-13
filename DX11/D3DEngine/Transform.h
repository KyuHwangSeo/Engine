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
	DLL_DECLSPEC void SetScale(DXVector3 _scale);
	DLL_DECLSPEC void SetScale(float x, float y, float z);
	DLL_DECLSPEC void SetScaleX(float x);
	DLL_DECLSPEC void SetScaleY(float y);

	DLL_DECLSPEC void ChangeScale(DXVector3 _scale);
	DLL_DECLSPEC void ChangeScale(float x, float y, float z);

	// Position ��� ���� �Լ�..
	DLL_DECLSPEC void SetPosition(DXVector3 pos);
	DLL_DECLSPEC void SetPosition(float x, float y, float z);
	DLL_DECLSPEC void SetPositionX(float x);
	DLL_DECLSPEC void SetPositionY(float y);
	DLL_DECLSPEC void SetPositionZ(float z);

	// Rotation ��� ���� �Լ�..
	DLL_DECLSPEC void SetRotate(DXVector3 _rot);
	DLL_DECLSPEC void SetRotate(float x, float y, float z);

	// Local Rotation �Լ�..
	DLL_DECLSPEC void RotateLocal(DXVector3 ypr);
	DLL_DECLSPEC void RotateLocal(float x, float y, float z);

	// Local Move �Լ�..
	DLL_DECLSPEC void MoveLocal(DXVector3 _move);
	DLL_DECLSPEC void MoveLocal(float x, float y, float z);
	DLL_DECLSPEC void MoveOffset(DXVector3 _move);
	DLL_DECLSPEC void MoveOffset(float x, float y, float z);

	// World Move �Լ�..
	DLL_DECLSPEC void MoveWorld(DXVector3 _move);
	DLL_DECLSPEC void MoveWorld(float _x, float _y, float _z);

	DLL_DECLSPEC void ResetTM(DXVector3 targetPos);

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
	DLL_DECLSPEC DXMatrix4X4 GetWorldTM();

	// PrevWorldTM ��ȯ �Լ�..
	DXMatrix4X4 GetPrevWorldTM();
	// LocalTM ��ȯ �Լ�..
	DXMatrix4X4 GetLocalTM();

	// Position ��ȯ �Լ�..
	DLL_DECLSPEC DXVector3 GetScale();
	// Position ��ȯ �Լ�..
	DLL_DECLSPEC DXVector3 GetPosition();
	// Rotation ��ȯ �Լ�..
	DLL_DECLSPEC DXVector3 GetRotation();

	// Hierarchy Get & Set
	Transform* GetParent() { return m_Parent; }
	vector<Transform*> GetAllChild() { return m_Child; }
	void SetParent(Transform* parent) { m_Parent = parent; }
	void AddChild(Transform* child) { m_Child.push_back(child); }

	void Release();
};
