#pragma comment(lib, "world.lib")

#include <windows.h>
#include "world.h"
#include "IBot.h"
#include "KimBot.h"


// Notes:

_declspec(dllexport) bool GetBotInterface(IBot ** pIBot)
{
	if(!*pIBot)
	{
		*pIBot = new KimBot;
		return true;
	}
	return false;
}


_declspec(dllexport) bool FreeBotInterface(IBot ** pIBot)
{
	if(!*pIBot)
	{
		return false;
	}
	delete *pIBot;
	*pIBot=0;
	return true;
}

