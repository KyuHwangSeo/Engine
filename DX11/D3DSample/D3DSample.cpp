#include "GameManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	GameManager* GM = new GameManager;

	GM->Initialize();
	GM->Loop();
	RELEASE_COM(GM);

	return 0;
}