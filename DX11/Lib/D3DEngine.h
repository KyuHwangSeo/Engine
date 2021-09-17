#pragma once

class DXTKFont;
class GameObject;
class D3DRenderer;
class Factory;
class Scene;
class SceneManager;

class D3DEngine : public Singleton<D3DEngine>
{
public:
	D3DEngine();
	~D3DEngine();

	bool Initialize(INT_PTR hinstance, INT_PTR hwnd, int screenWidth, int screenHeight);
	void CreateDevice();

	void CreateHelper();
	void Release();

	void SceneStart();
	void Update();

	void OnResize(int width, int height);

	void Render();
	void EndRender();

	void CreateRenderStates();
	void SetSolid();
	void SetWireFrame();
	void SetNoCull();
	void SetDepth();

	ComPtr<ID3D11Device> GetDevice();
	ComPtr<ID3D11DeviceContext> GetDeviceContext();

	void DrawTextColor(int x, int y, float scale, XMFLOAT4 color, TCHAR* text, ...);

	// 외부 프로젝트에 필요한 함수들..
	void AddScene(const char* name, Scene* scene);
	void SelectScene(const char* name);
	void RemoveScene(const char* name);
	Scene* FindScene(const char* name);

	void Picking(int x, int y);

	Factory* GetFactory();
	Scene* GetScene();
	GameObject* FindObject(const char* objName, eObjectType objType);

	bool GetDebug() { return m_DebugMode; }
	POINT* GetClinetSize()	{ return m_ClientSize; }

private:
	POINT* m_ClientSize = nullptr;
	bool Full_Screen = false;

	HWND	m_Mainhwnd = 0;

	ComPtr<ID3D11Device> m_Device = nullptr;
	ComPtr<ID3D11DeviceContext> m_DeviceContext = nullptr;
	ComPtr<IDXGISwapChain> m_SwapChain = nullptr;

	ComPtr<ID3D11RasterizerState> m_WireframeRS = nullptr;
	ComPtr<ID3D11RasterizerState> m_SolidRS = nullptr;
	ComPtr<ID3D11RasterizerState> m_NoCullRS = nullptr;
	ComPtr<ID3D11RasterizerState> m_DepthRS = nullptr;

	D3D_FEATURE_LEVEL m_FeatureLevel;

private:
	void Draw_Text();
	void CheckKeyInput();

private:
	DXTKFont* m_Font = nullptr;

	SceneManager* m_SceneManager;
	Factory* m_Factory;

	Scene* m_Scene;

	D3DRenderer* m_Renderer;

	float dTime;
	bool m_DebugMode = false;
};
