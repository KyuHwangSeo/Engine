#pragma once
const bool FULL_SCREEN = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GameManager
{
public:
	GameManager();
	~GameManager();

	bool Initialize();
	void Release();
	void Loop();
	
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

	void OnMouseDown(int x, int y);
	void OnMouseUp(int x, int y);
	float GetAspectRatio() const;

private:
	void Update();
	void Render();

	void LoadData();
	void LoadScene();

	void InitializeWindows(int&, int&);
	void RecalcWindowSize();
	void ShutdownWindows();

	POINT m_LastMousePos = { 0, };

private:
	LPCWSTR m_applicationName = 0;
	HINSTANCE m_hinstance = 0;
	HWND m_hwnd = 0;
	MSG m_Msg = { 0, };

	int m_screenWidth;
	int m_screenHeight;
};

/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static GameManager* ApplicationHandle = 0;