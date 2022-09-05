#include "framework.h"
#include "Keyboard.h"

// 0x0000 : 이전에 누른 적이 없고 호출 시점에서 안눌린 상태
// 0x8000 : 이전에 누른 적이 없고 호출 시점에서 눌린 상태
// 0x8001 : 이전에 누른 적이 있고 호출 시점에서 눌린 상태
// 0x0001 : 이전에 누른 적이 있고 호출 시점에서 안눌린 상태
/////////////////////////////////////////////////////
// 생성자
////////////////////////////////////////////////////
Keyboard::Keyboard()
{
	// 초기화
	for (int i = 0; i < KEYMAX; i++)
	{
		this->GetKeyUp().set(i, false);
		this->GetKeyDown().set(i, false);
	}
}

Keyboard::~Keyboard()
{
}

void Keyboard::WindowCallbackKeyDown(int key)
{
	//this->SetKeyDown(key, true);
}

void Keyboard::WindowCallbackKeyUp(int key)
{
	//this->SetKeyUp(key, true);
}

bool Keyboard::Down(int key)
{
	HWND hwnd = GetFocus();
	if (hwnd != MAIN->GetWindowHandler())
		return false;

	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (!this->GetKeyDown()[key])
		{
			this->SetKeyDown(key, true);
			return true;
		}
	}
	else
	{
		this->SetKeyDown(key, false);
	}
	return false;
}

bool Keyboard::Up(int key)
{
	HWND hwnd = GetFocus();
	if (hwnd != MAIN->GetWindowHandler())
		return false;

	if (GetAsyncKeyState(key) & 0x8000)
	{
		this->SetKeyUp(key, true);
	}
	else
	{
		if (this->GetKeyUp()[key])
		{
			this->SetKeyUp(key, false);
			return true;
		}
	}
	return false;
}

bool Keyboard::Press(int key)
{
	HWND hwnd = GetFocus();
	if (hwnd != MAIN->GetWindowHandler())
		return false;

	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

bool Keyboard::Toggle(int key)
{
	HWND hwnd = GetFocus();
	if (hwnd != MAIN->GetWindowHandler())
		return false;

	if (GetAsyncKeyState(key) & 0x0001) return true;

	return false;
}

bool Keyboard::DownAny()
{
	HWND hwnd = GetFocus();
	if (hwnd != MAIN->GetWindowHandler())
		return false;

	for (int i = 8; i <= 127; i++)
	{
		if (GetAsyncKeyState(i) & 0x8000)
		{
			if (!this->GetKeyDown()[i])
			{
				this->SetKeyDown(i, true);
				return true;
			}
		}
		else
		{
			this->SetKeyDown(i, false);
		}
	}
	return false;
}

bool Keyboard::UpAny()
{
	HWND hwnd = GetFocus();
	if (hwnd != MAIN->GetWindowHandler())
		return false;

	for (int i = 8; i <= 127; i++)
	{
		if (GetAsyncKeyState(i) & 0x8000)
		{
			this->SetKeyUp(i, true);
		}
		else
		{
			if (this->GetKeyUp()[i])
			{
				this->SetKeyUp(i, false);
				return true;
			}
		}
	}
	return false;
}

bool Keyboard::PressAny()
{
	HWND hwnd = GetFocus();
	if (hwnd != MAIN->GetWindowHandler())
		return false;

	for (int i = 8; i <= 127; i++)
	{
		if (GetAsyncKeyState(i) & 0x8000)
			return true;
	}
	return false;
}
