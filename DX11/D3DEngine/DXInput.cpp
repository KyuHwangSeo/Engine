// DirectX Input 전용 라이브러리
#pragma comment(lib, "dinput8.lib") 
#pragma comment(lib, "dxguid.lib")

#include "SingleTon.h"
#include "DeleteDefine.h"
#include "DXInput.h"

DXInput::DXInput()
{
	m_MousePos = new POINT;
}

DXInput::~DXInput()
{
	SAFE_DELETE(m_MousePos);
}

bool DXInput::IsKeyUP(BYTE Key_Input)
{
	/// 만약 입력된 키가 눌렸으면 현재 눌린상태로 플래그바꿈..
	if (GetAsyncKeyState(Key_Input))
	{
		m_keyflag[Key_Input].Key_Current_State = true;
	}
	else
	{
		m_keyflag[Key_Input].Key_Current_State = false;
	}

	/// 이전번에 눌렸던 상태고 이번에 키가 떼졌으면 ( 키를 뗏을 때 )
	if (m_keyflag[Key_Input].Key_Current_State == false && m_keyflag[Key_Input].Key_Prev_State == true)
	{
		m_keyflag[Key_Input].Key_Prev_State = m_keyflag[Key_Input].Key_Current_State;
		return true;
	}

	/// 이전상태 저장.
	m_keyflag[Key_Input].Key_Prev_State = m_keyflag[Key_Input].Key_Current_State;

	return false;
}

bool DXInput::IsKeyDown(BYTE Key_Input)
{
	/// 만약 입력된 키가 눌렸으면 현재 눌린상태로 플래그바꿈..
	if (GetAsyncKeyState(Key_Input))
	{
		m_keyflag[Key_Input].Key_Current_State = true;
	}
	else
	{
		m_keyflag[Key_Input].Key_Current_State = false;
	}

	/// 만약 이번에 눌렸고 이전번에 눌리지않았으면 ( 키가 눌렸을 때 )
	if (m_keyflag[Key_Input].Key_Current_State == true && m_keyflag[Key_Input].Key_Prev_State == false)
	{
		m_keyflag[Key_Input].Key_Prev_State = m_keyflag[Key_Input].Key_Current_State;
		return true;
	}

	return false;
}

bool DXInput::IsKeyKeep(BYTE Key_Input)
{
	/// 만약 입력된 키가 눌렸으면 현재 눌린상태로 플래그바꿈..
	if (GetAsyncKeyState(Key_Input))
	{
		m_keyflag[Key_Input].Key_Current_State = true;

	}
	else
	{
		m_keyflag[Key_Input].Key_Current_State = false;
	}

	/// 만약 이번에 눌렸고 이전번에 눌리지않았으면 ( 키가 눌렸을 때 )
	if (m_keyflag[Key_Input].Key_Current_State == true &&
		m_keyflag[Key_Input].Key_Prev_State == false)
	{
		m_keyflag[Key_Input].Key_Prev_State = m_keyflag[Key_Input].Key_Current_State;
		return true;
	}
	/// 이전번에 눌렸던 상태고 이번에 키가 떼졌으면 ( 키를 뗏을 때 )
	if (m_keyflag[Key_Input].Key_Current_State == false &&
		m_keyflag[Key_Input].Key_Prev_State == true)
	{
		m_keyflag[Key_Input].Key_Prev_State = m_keyflag[Key_Input].Key_Current_State;

		return true;
	}

	/// 이전상태 저장.
	m_keyflag[Key_Input].Key_Prev_State = m_keyflag[Key_Input].Key_Current_State;

	return false;
}

bool DXInput::IsKeyDownKeep(BYTE Key_Input)
{
	if (GetAsyncKeyState(Key_Input))
	{
		return true;
	}

	return false;
}

bool DXInput::IsKeyDoubleDown(BYTE Key_Input)
{
	static int Set_frame = 15;

	if (m_keyflag[Key_Input].Key_Dash_Flag)
	{
		if (IsKeyUP(Key_Input))
		{
			m_keyflag[Key_Input].Key_Dash_Flag = false;
		}
	}
	else
	{
		if (IsKeyDown(Key_Input))
		{
			// 이전에 이미 눌린적이 있고 키 입력 유효 시간이 아직 지나지 않았다면.
			if (m_keyflag[Key_Input].Key_Timer_Flag == true && m_keyflag[Key_Input].Key_Timer != 0)
			{
				m_keyflag[Key_Input].Key_Dash_Flag = true;
			}
			else
			{
				ResetFlagTimer();
				m_keyflag[Key_Input].Key_Timer_Flag = true;
				m_keyflag[Key_Input].Key_Timer = Set_frame;
			}
		}
	}

	m_keyflag[Key_Input].Key_Timer--;

	if (m_keyflag[Key_Input].Key_Timer < 0)
	{
		m_keyflag[Key_Input].Key_Timer = 0;
	}

	return m_keyflag[Key_Input].Key_Dash_Flag;
}

void DXInput::OnMouseMove(int x, int y)
{
	m_MousePos->x = x;
	m_MousePos->y = y;
}

void DXInput::ResetFlagTimer()
{
	for (int i = 0; i < 256; i++)
	{
		m_keyflag[i].Key_Timer_Flag = false;
		m_keyflag[i].Key_Timer = 0;
	}
}
