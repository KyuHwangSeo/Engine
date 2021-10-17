#include <windows.h>
#include "GameManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	GameManager* GM = new GameManager;

	GM->Initialize();
	GM->Loop();
	GM->Release();
	delete GM;

	return 0;
}