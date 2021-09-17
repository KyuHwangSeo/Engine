#pragma once
/// <summary>
/// Transform Class
/// </summary>
/// 
/// 1. 오브젝트마다 한개의 Transform을 지닌다
/// 2. 애니메이션에서 NodeTM을 업데이트 시켜주면 
/// 최종적으로 Transform Component에서 Local / World 업데이트를 해주는 구조
/// 3. 오브젝트가 Hierarchy 구조일 경우 연결된 상위 오브젝트의 변환 행렬을 상속 받는 구조
/// 
class Gizmos;
class Transform : public Component
{
public:
	DXMatrix4X4 m_LocalRot;			// 로컬 회전 행렬
	DXMatrix4X4 m_OffsetTM;

	DXMatrix4X4 m_ConvertTM;		// 변환 행렬

	DXMatrix4X4 m_LocalTM;			// 로컬 TM
	DXMatrix4X4 m_NodeTM;			// 노드 TM

	DXMatrix4X4 m_WorldTM;			// 월드 TM
	DXMatrix4X4 m_PrevWorldTM;		// 이전 프레임 월드 TM
	DXMatrix4X4 m_PrevLocalTM;		// 이전 프레임 변환 TM

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
	// 기본 생성자
	Transform();
	~Transform();

	// Scale 행렬 설정 함수..
	KH_API void SetScale(DXVector3 _scale);
	KH_API void SetScale(float x, float y, float z);
	KH_API void SetScaleX(float x);
	KH_API void SetScaleY(float y);

	KH_API void ChangeScale(DXVector3 _scale);
	KH_API void ChangeScale(float x, float y, float z);

	// Position 행렬 설정 함수..
	KH_API void SetPosition(DXVector3 pos);
	KH_API void SetPosition(float x, float y, float z);
	KH_API void SetPositionX(float x);
	KH_API void SetPositionY(float y);
	KH_API void SetPositionZ(float z);

	// Rotation 행렬 설정 함수..
	KH_API void SetRotate(DXVector3 _rot);
	KH_API void SetRotate(float x, float y, float z);

	// Local Rotation 함수..
	KH_API void RotateLocal(DXVector3 ypr);
	KH_API void RotateLocal(float x, float y, float z);

	// Local Move 함수..
	KH_API void MoveLocal(DXVector3 _move);
	KH_API void MoveLocal(float x, float y, float z);
	KH_API void MoveOffset(DXVector3 _move);
	KH_API void MoveOffset(float x, float y, float z);

	// World Move 함수..
	KH_API void MoveWorld(DXVector3 _move);
	KH_API void MoveWorld(float _x, float _y, float _z);

	KH_API void ResetTM(DXVector3 targetPos);

	void Start();
	void Render(DXMatrix4X4 view, DXMatrix4X4 proj);

	// WorldTM Update 함수..
	void Update(float dTime);
	void PrevUpdate();

	// LocalTM Update 함수..
	void UpdateLocal();
	void UpdateConvert();
	void UpdateChildLocal(Transform* parent);
	void UpdateChildConvert(Transform* parent);

	// LocalTM 설정 함수..
	void SetLocalTM(DXMatrix4X4 _tm);
	// NodeTM 설정 함수..
	void SetNodeTM(DXMatrix4X4 _tm);

	// WorldTM 반환 함수..
	KH_API DXMatrix4X4 GetWorldTM();

	// PrevWorldTM 반환 함수..
	DXMatrix4X4 GetPrevWorldTM();
	// LocalTM 반환 함수..
	DXMatrix4X4 GetLocalTM();

	// Position 반환 함수..
	KH_API DXVector3 GetScale();
	// Position 반환 함수..
	KH_API DXVector3 GetPosition();
	// Rotation 반환 함수..
	KH_API DXVector3 GetRotation();

	// Hierarchy Get & Set
	Transform* GetParent() { return m_Parent; }
	vector<Transform*> GetAllChild() { return m_Child; }
	void SetParent(Transform* parent) { m_Parent = parent; }
	void AddChild(Transform* child) { m_Child.push_back(child); }

	void Release();
};
