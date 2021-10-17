#pragma once
class Light : public Component
{
public:
	Light(eLightType lightType = eLightType::Direction);
	~Light();

public:
	void Start();
	void Update(float dTime);
	void Render(DXMatrix4X4 view, DXMatrix4X4 proj);
	void Release();
	
	ENGINE_DLL void SetLightType(eLightType lightType);
	ENGINE_DLL void SetLightColor(DXVector4 lightColor);
	void DirLightRotate(DXVector3 axis, float angle);
	void SetPointLight(PointLight pointLight);
	void SetSpotLight(SpotLight spotLight);

	void SetLightData(cbLights& lightData, int m_Index = 0);
	void SetLightListData(cbLightList& lightListData, int m_Index = 0);

	template<typename T>
	void SetPosition(T& lightData);
	ENGINE_DLL void SetTarget(GameObject* obj);

	void SetLightViewProj();

	DXVector3 GetDirection(int m_Index = 0);
	
	eLightType GetLightType()	{ return m_LightType; }
	DXMatrix4X4 GetLightView()	{ return m_LightView; }
	DXMatrix4X4 GetLightProj()	{ return m_LightProj; }
	DirectionalLight GetLightData(int m_Index) { return m_DirLights[m_Index]; }

	Transform* m_Target;

	static Light* g_DirLight;

	DXMatrix4X4 m_LightView;
	DXMatrix4X4 m_LightProj;

	static DXVector3 g_CenterPos;
	static float g_ShadowRadius;
	
	static void SetShadowRadius(float radius) { g_ShadowRadius = radius; }
	static void SetCenterPos(DXVector3 pos) { g_CenterPos = pos; }

private:
	unsigned int m_LightNum;

	DirectionalLight m_DirLights[3];
	PointLight m_PointLight;
	SpotLight m_SpotLight;

	eLightType m_LightType;

	Gizmos* m_Gizmo;
};

template<typename T>
void Light::SetPosition(T& lightData)
{
	lightData.Position = m_GameObject->GetTransform()->GetPosition();
}