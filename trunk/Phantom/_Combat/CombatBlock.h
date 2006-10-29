/*=======================================================================================
==    COMBAT BLOCK
==	Basic Flow is that the player attacks the ICE and the Ice Attacks Player
==	The player can move in a hemisphere and shoot in a turret
=======================================================================================*/

#ifndef COMBAT_BLOCK_DOOM_232
#define COMBAT_BLOCK_DOOM_232

//------------------------------------------
//	Includes (uses)
//------------------------------------------
#include "..\gameBlock.h"
#include "ICEzombie.h"

//------------------------------------------
//	Defines
//------------------------------------------
#define CMB_BLOCKSIZE 2.0f
#define CMB_PLAYERDIST 15.0f
#define CMB_PLAYERMOVE 0.1f
#define CMB_TURRETDIST 5.0f

//------------------------------------------
//	Support Classes (isolated)
//------------------------------------------
#include "ICEbase.h"

//------------------------------------------
//	Combat Class
//------------------------------------------
class combatBlock : public gameBlock
{
public:
	//--------------------------
	// Combat Block
	//--------------------------
	combatBlock();
	~combatBlock();
	//--------------------------
	// Interface Functions
	//--------------------------
	int Open();
	int Start(comPData _player, comICEdata _ice);
	int Step();
	int Input(UINT _iMsg, WPARAM wParam, LPARAM lParam);
	int Exit();
	//--------------------------
	//	Support Functions
	//--------------------------
	void Render();
	void movePlayer();
	//--------------------------
	//	Varibles
	//--------------------------
private:
	comPData m_Player;				// Player
	comICE* m_ice;					// Ice
	CVector2 m_MouseMove;			// Mouse Vector
	int m_MouseState;				// Mouse Button State

	BGL_Bitmap m_bitmap;
	BGLRasterFont m_font;
};

#endif
