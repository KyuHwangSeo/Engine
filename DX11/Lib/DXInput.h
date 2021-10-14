#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#define VK_A	0x41
#define VK_B	0x42
#define VK_C	0x43
#define VK_D	0x44
#define VK_E	0x45
#define VK_F	0x46
#define VK_G	0x47
#define VK_H	0x48
#define VK_I	0x49
#define VK_J	0x4a
#define VK_K	0x4b
#define VK_L	0x4c
#define VK_M	0x4d
#define VK_N	0x4e
#define VK_O	0x4f
#define VK_P	0x50
#define VK_Q	0x51
#define VK_R	0x52
#define VK_S	0x53
#define VK_T	0x54
#define VK_U	0x55
#define VK_V	0x56
#define VK_W	0x57
#define VK_X	0x58
#define VK_Y	0x59
#define VK_Z	0x5a

struct KeyPlag
{
	bool Key_Current_State = false;
	bool Key_Prev_State = false;
	bool Key_Timer_Flag = false;
	bool Key_Dash_Flag = false;
	int Key_Timer = 0;
};

class DXInput
{
public:
	DXInput();
	~DXInput();

	bool IsKeyUP(BYTE Key_Input);
	bool IsKeyDown(BYTE Key_Input);
	bool IsKeyKeep(BYTE Key_Input);
	bool IsKeyDownKeep(BYTE Key_Input);
	bool IsKeyDoubleDown(BYTE Key_Input);

	void OnMouseMove(int x, int y);

	void ResetFlagTimer();

	POINT* GetMousePos() { return m_MousePos; }

private:
	unsigned char m_keyboardState[256] = { 0, };
	KeyPlag m_keyflag[256];

	POINT* m_MousePos;

	int m_screenWidth, m_screenHeight;
};

