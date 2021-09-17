#define _CRT_SECURE_NO_WARNINGS

#include "D3DEngineDLL.h"
#include "GameManager.h"
#include "Sample_Scene.h"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

bool GameManager::Initialize()
{
	// 화면 비율 설정..
	m_screenWidth = 1600;
	m_screenHeight = 900;

	// Windows Api 초기화..
	InitializeWindows(m_screenWidth, m_screenHeight);

	// Window Size 재설정..
	RecalcWindowSize();

	// Engine 초기화..
	KH_ENGINE::Initialize((INT_PTR)m_hinstance, (INT_PTR)m_hwnd, m_screenWidth, m_screenHeight);

	// Data Load..
	LoadData();

	// Scene Load..
	LoadScene();

	return true;
}

void GameManager::Update()
{
	// Engine 업데이트..
	KH_ENGINE::Update();
}

void GameManager::Render()
{
	KH_ENGINE::Render();

	KH_ENGINE::EndRender();
}

void GameManager::LoadData()
{
	KH_UTILITY::LoadData(eLoadType::FBX, "CHARACTER_idle", "Character/CHARACTER_idle.fbx", false);
	
	// FBX Mananger 해제..
	KH_UTILITY::ResetFBX();
}

void GameManager::LoadScene()
{
	Sample_Scene* Sample = new Sample_Scene();
	KH_ENGINE::AddScene("Sample", Sample);

	// First Scene Select..
	KH_ENGINE::SelectScene("Sample");
}

void GameManager::Release()
{
	// Engine, Utility 해제..
	KH_ENGINE::Release();

	ShutdownWindows();
}

void GameManager::Loop()
{
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			break;
		}
		else
		{
			Update();
			Render();
		}
	}

	return;
}

LRESULT CALLBACK GameManager::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_SIZE:
		if (KH_ENGINE::CheckDevice())
		{
			int width = LOWORD(lparam);
			int height = HIWORD(lparam);
		
			// 화면을 최소화할 경우 재설정을 할 필요가 없으므로 리턴..
			if (width > 0 && height > 0)
			{
				KH_ENGINE::OnResize(width, height);
				KH_ENGINE::CameraSetLens(width, height);
			}
		}
		return 0;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(LOWORD(lparam), HIWORD(lparam));
		return 0;
	case WM_LBUTTONUP:
		KH_ENGINE::Picking(LOWORD(lparam), HIWORD(lparam));
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(LOWORD(lparam), HIWORD(lparam));
		return 0;
	case WM_MOUSEMOVE:
		KH_KEYINPUT::OnMouseMove(LOWORD(lparam), HIWORD(lparam));
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

void GameManager::OnMouseDown(int x, int y)
{
	m_LastMousePos.x = x;
	m_LastMousePos.y = y;

	SetCapture(m_hwnd);
}

void GameManager::OnMouseUp(int x, int y)
{
	ReleaseCapture();
}

float GameManager::GetAspectRatio() const
{
	return static_cast<float>(m_screenWidth) / m_screenHeight;
}

void GameManager::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	int posX, posY;

	// 객체에 대한 외부 포인터를 가져온다..	
	ApplicationHandle = this;

	// 인스턴스를 가져온다..
	m_hinstance = GetModuleHandle(NULL);

	m_applicationName = L"D3D11Engine_SKH";

	// Window Class 설정..
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	// 화면 설정..
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 전체화면 & 일반화면 설정
	if (FULL_SCREEN)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// 전체화면으로 전환!
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		screenWidth = 1600;
		screenHeight = 900;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// 화면 설정으로 창을 만들고 핸들을 가져옵니다..
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_OVERLAPPEDWINDOW,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// 창을 화면 위로 가져와서 주 포커스로 설정..
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// 커서 숨기기..
	//ShowCursor(false);

	return;
}

void GameManager::RecalcWindowSize()
{
	RECT nowRect;
	DWORD _style = (DWORD)GetWindowLong(m_hwnd, GWL_STYLE);
	DWORD _exstyle = (DWORD)GetWindowLong(m_hwnd, GWL_EXSTYLE);

	GetWindowRect(m_hwnd, &nowRect);

	RECT newRect;
	newRect.left = 0;
	newRect.top = 0;
	newRect.right = m_screenWidth;
	newRect.bottom = m_screenHeight;

	AdjustWindowRectEx(&newRect, _style, NULL, _exstyle);

	// 클라이언트 영역보다 윈도 크기는 더 커야 한다. (외곽선, 타이틀 등)
	int _newWidth = (newRect.right - newRect.left);
	int _newHeight = (newRect.bottom - newRect.top);

	SetWindowPos(m_hwnd, HWND_NOTOPMOST, nowRect.left, nowRect.top,
		_newWidth, _newHeight, SWP_SHOWWINDOW);
}


void GameManager::ShutdownWindows()
{
	ShowCursor(true);

	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);

		return 0;
	}

	case WM_CLOSE:
	{
		PostQuitMessage(0);

		return 0;
	}

	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}