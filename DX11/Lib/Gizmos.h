#pragma once
class Transform;
class Gizmos : public Object
{
public:
	Gizmos(eGizmoType gizmoType, DXVector3 scale = DXVector3::One(), DXVector3 rot = DXVector3::One(), DXVector3 pos = DXVector3::Zero());
	~Gizmos();

public:
	void Render(DXMatrix4X4 view, DXMatrix4X4 proj);
	void Release();

	void SetVertexBuffer(VertexBuffer* vBuffer);
	void SetGameObject(GameObject* obj);

	void SetEnabled(bool enabled) { m_Enable = enabled; };
	bool GetEnabled() { return m_Enable; };

private:
	Transform* m_ObjectTrans;

	DXMatrix4X4 m_OffsetTM;

	eGizmoType m_GizmoType;
	eTopologyType m_TopologyType;

	bool m_Enable;

	ComPtr<ID3D11DeviceContext> m_DeviceContext;

	ComPtr<ID3D11Buffer> m_VB;		// 버텍스버퍼
	ComPtr<ID3D11Buffer> m_IB;		// 인덱스버퍼

	UINT m_IndexCount;
	UINT m_Stride;
	UINT m_Offset;

	Shader* m_Shader;

	cbPerObject m_ObjectData;

	DirectX::BoundingBox m_Bounds;
};

