#ifndef RAIDER_HEADER_CHIP_HEAD
#define RAIDER_HEADER_CHIP_HEAD

/*----------------------------------
//	Includes
----------------------------------*/
#include <ptk.h>

#include "lib/chipmunk.h"
#pragma comment ( lib,"lib/Chipmunk4MSVC2k3.lib")

/*----------------------------------
	 Wall Data
 ----------------------------------*/
struct cpWallData
{
	int m_deadTime;
};
/*----------------------------------
//	Define
----------------------------------*/
#define		FP_SCREENW		800
#define		FP_SCREENH		600

#define		FP_WORLDW		800
#define		FP_WORLDH		600

#define		FP_TITLE		"Flammable Penguins: Raiders of the Lost Block"

/*----------------------------------
	 Enums
 ----------------------------------*/
enum gpCollide{GPC_NULL, GPC_DELETE, GPC_STATIC, GPC_MOVING, GPC_PLAYER, GPC_ITEM};
enum gpState{GPS_NULL,GPS_SETUP,GPS_READY,GPS_PLAYING,GPS_PAUSE,GPS_STOP};
enum gpAction{GPA_NULL,GPA_GRABY,GPA_STICKY,GPA_JUMP,GPA_FLOOR};

extern gpAction g_action;

#endif