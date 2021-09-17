// DirectX Input ���� ���̺귯��
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
	/// ���� �Էµ� Ű�� �������� ���� �������·� �÷��׹ٲ�..
	if (GetAsyncKeyState(Key_Input))
	{
		m_keyflag[Key_Input].Key_Current_State = true;
	}
	else
	{
		m_keyflag[Key_Input].Key_Current_State = false;
	}

	/// �������� ���ȴ� ���°� �̹��� Ű�� �������� ( Ű�� ���� �� )
	if (m_keyflag[Key_Input].Key_Current_State == false && m_keyflag[Key_Input].Key_Prev_State == true)
	{
		m_keyflag[Key_Input].Key_Prev_State = m_keyflag[Key_Input].Key_Current_State;
		return true;
	}

	/// �������� ����.
	m_keyflag[Key_Input].Key_Prev_State = m_keyflag[Key_Input].Key_Current_State;

	return false;
}

bool DXInput::IsKeyDown(BYTE Key_Input)
{
	/// ���� �Էµ� Ű�� �������� ���� �������·� �÷��׹ٲ�..
	if (GetAsyncKeyState(Key_Input))
	{
		m_keyflag[Key_Input].Key_Current_State = true;
	}
	else
	{
		m_keyflag[Key_Input].Key_Current_State = false;
	}

	/// ���� �̹��� ���Ȱ� �������� �������ʾ����� ( Ű�� ������ �� )
	if (m_keyflag[Key_Input].Key_Current_State == true && m_keyflag[Key_Input].Key_Prev_State == false)
	{
		m_keyflag[Key_Input].Key_Prev_State = m_keyflag[Key_Input].Key_Current_State;
		return true;
	}

	return false;
}

bool DXInput::IsKeyKeep(BYTE Key_Input)
{
	/// ���� �Էµ� Ű�� �������� ���� �������·� �÷��׹ٲ�..
	if (GetAsyncKeyState(Key_Input))
	{
		m_keyflag[Key_Input].Key_Current_State = true;

	}
	else
	{
		m_keyflag[Key_Input].Key_Current_State = false;
	}

	/// ���� �̹��� ���Ȱ� �������� �������ʾ����� ( Ű�� ������ �� )
	if (m_keyflag[Key_Input].Key_Current_State == true &&
		m_keyflag[Key_Input].Key_Prev_State == false)
	{
		m_keyflag[Key_Input].Key_Prev_State = m_keyflag[Key_Input].Key_Current_State;
		return true;
	}
	/// �������� ���ȴ� ���°� �̹��� Ű�� �������� ( Ű�� ���� �� )
	if (m_keyflag[Key_Input].Key_Current_State == false &&
		m_keyflag[Key_Input].Key_Prev_State == true)
	{
		m_keyflag[Key_Input].Key_Prev_State = m_keyflag[Key_Input].Key_Current_State;

		return true;
	}

	/// �������� ����.
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
			// ������ �̹� �������� �ְ� Ű �Է� ��ȿ �ð��� ���� ������ �ʾҴٸ�.
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
