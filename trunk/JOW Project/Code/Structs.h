#ifndef DEFINED_ALL_THE_STRUCTS_AND_STUFF_WE_USE
#define DEFINED_ALL_THE_STRUCTS_AND_STUFF_WE_USE

#include "CVector3.h"
#include "JWLMesh.h"
#include <string.h>

typedef unsigned int UINT;
//--------------------------------------------------
// Game Settings
//--------------------------------------------------
#define _J_SHORTNAME		"JewelEng"
#define _J_LONGNAME			"Jewel of Worlds Engine v0.00"
#define _J_FOV				45.0f
#define _J_CLIPNEAR			0.5f
#define _J_CLIPFAR			150.0f
#define _J_DEFWIDTH			800
#define _J_DEFHEIGHT		600
#define _J_DEFDEPTH			32
#define _J_DEFFULLSCREEN	false
#define _J_FONT				"Arial"
#define _J_FONT_HEIGHT		16

//----------------------------------
// Paths and Strings
//----------------------------------
#define JWL_FN_MODEL		"Data\x5CModels\x5C"
#define JWL_FN_TEXTURE		"Data\x5CTextures\x5C"
#define JWL_FN_MAPS			"Data\x5CMaps\x5C"
#define JWL_FN_UNITS		"Data\x5CUnits\x5C"
#define JWL_FN_LOGIC		"Data\x5CLogic\x5C"
#define JWL_FN_SCENARIOS	"Data\x5CScenarios\x5C"
#define JWL_FN_KEYBOARD		"User\x5CKeys.cfg"

//----------------------------------
// OpenGL
//----------------------------------
#define _MAX_TEX_COUNT	100
#define _MAX_MESH_COUNT	125
#define _MAX_OBJ_COUNT  150
#define _MAX_UNIT_COUNT 100
#define _MAX_TEAM_COUNT 4
#define _MAX_CHAR		255
#define _HIT_BUFFER_SIZE 512

//----------------------------------
// Permenent Defines
//----------------------------------
#define INVALID_PARAM	8419
#define WM_MOUSEWHEEL	0x020A
#define GET_WHEEL_DELTA_WPARAM ((int)(SHORT)HIWORD(wParam))
#define GET_X_LPARAM	((UINT)(SHORT)LOWORD(lParam))
#define GET_Y_LPARAM	((UINT)(SHORT)HIWORD(lParam))

//-----------------------------------------------
//     Circular Buffer Defines
//-----------------------------------------------
#define CB_MAX_MSG_SIZE		255
#define CB_MAX_MSG_COUNT	20
#define CB_BUFSIZE			CB_MAX_MSG_SIZE*CB_MAX_MSG_COUNT

//-----------------------------------------------
//     Network Defines
//-----------------------------------------------
#define NET_CMD_LATENCY		5023
#define NET_PORT_SERV_OUT	502
#define NET_PORT_CLIENT_OUT 503

//--------------------------------------------------
// Game Modes
//--------------------------------------------------
#define JWL_GM_MELEE							   1

//--------------------------------------------------
// Melee Rules
//--------------------------------------------------
#define JWL_MELEE_RL_PLACEMENT					   1			// Allow Unit Placement
#define JWL_MELEE_RL_FOGOWAR					   2			// Fog of War ON

//--------------------------------------------------
// Team Colours
//--------------------------------------------------
#define TC_1		 1.0f,0.0f,0.0f 
#define TC_2		 0.0f,1.0f,0.0f 
#define TC_3		 0.0f,0.0f,1.0f 
#define TC_4		 1.0f,1.0f,0.0f 
#define TC_5		 1.0f,0.0f,1.0f 
#define TC_6		 0.0f,1.0f,1.0f 
//--------------------------------------------------
// Team Defines
//--------------------------------------------------
#define	TEAM_WAR			  1
#define	TEAM_MAGE			  2
#define	TEAM_MECH			  4
#define	TEAM_UNDEAD			  8
#define	TEAM_MON			 16
#define	TEAM_ALCH			 32
#define	TEAM_NAT			 64
#define	TEAM_HOLY			128

//--------------------------------------------------
// Unit Defines
//--------------------------------------------------
#define	U_PHASE_START		1
#define	U_PHASE_MID			2
#define	U_PHASE_END			4

//--------------------------------------------------
// Unit IDs
//--------------------------------------------------
//............. War .............
//............. Mage .............
//............. Mech .............
#define UID_ME_STEAMHEAD			301
#define UID_ME_LUMBERJACK			302
#define UID_ME_SLICENDICE			303
#define UID_ME_RATATAT				304
#define UID_ME_STEAMFIEND			305
#define UID_ME_BIGBOOM				306
#define UID_ME_COPTER				307
//............. Undead .............
//............. Monsters .............
//............. Alchemist .............
//............. Nature .............
//............. Holy .............

//--------------------------------------------------
// Unit Filenames
//--------------------------------------------------
//............. War .............
//............. Mage .............
//............. Mech .............
#define UFN_ME_STEAMHEAD			"Mech\x5CSteamhead.unit"
#define UFN_ME_LUMBERJACK			"Mech\x5CLumberjack.unit"
#define UFN_ME_SLICENDICE			"Mech\x5CSlicenDice.unit"
#define UFN_ME_RATATAT				"Mech\x5CRataTat.unit"
#define UFN_ME_STEAMFIEND			"Mech\x5CSteamfiend.unit"
#define UFN_ME_BIGBOOM				"Mech\x5C_BigBoom.unit"
#define UFN_ME_COPTER				"Mech\x5C_Copter.unit"
//............. Undead .............
//............. Monsters .............
//............. Alchemist .............
//............. Nature .............
//............. Holy .............
//--------------------------------------------------
// Command IDs
//--------------------------------------------------
// System Commands
#define JC_SYS_UPDATEUNIT					  12
#define JC_SYS_STRING						  13

#define JC_SYS_LOOK_SELECTION				  42

#define JC_SYS_CAM_FORWARD					  61
#define JC_SYS_CAM_RIGHT						  62
#define JC_SYS_CAM_BACKW					  63
#define JC_SYS_CAM_LEFT						  64

#define JC_SYS_PLOT_PATH					  71

// Unit Commands
#define JC_UNIT_SELECT						 200
#define JC_UNIT_MOVE						 201
#define JC_UNIT_MOVEXY						 202
#define JC_UNIT_DUCK						 203
#define JC_UNIT_STAND						 204
#define JC_UNIT_CLIMB						 205
#define JC_UNIT_MOVE_PATH					 206

#define JC_UNIT_MOVE_N						 210
#define JC_UNIT_MOVE_NE						 211
#define JC_UNIT_MOVE_E						 212
#define JC_UNIT_MOVE_SE						 213
#define JC_UNIT_MOVE_S						 214
#define JC_UNIT_MOVE_SW						 215
#define JC_UNIT_MOVE_W						 216
#define JC_UNIT_MOVE_NW						 217
#define JC_UNIT_MOVE_UP						 218
#define JC_UNIT_MOVE_DOWN					 219

#define JC_UNIT_ATTACK						 230

#define JC_UNIT_SPECIAL_0					 290
#define JC_UNIT_SPECIAL_1					 291
#define JC_UNIT_SPECIAL_2					 292
#define JC_UNIT_SPECIAL_3					 293
#define JC_UNIT_SPECIAL_4					 294
#define JC_UNIT_SPECIAL_5					 295
#define JC_UNIT_SPECIAL_6					 296
#define JC_UNIT_SPECIAL_7					 297
#define JC_UNIT_SPECIAL_8					 298
#define JC_UNIT_SPECIAL_9					 299

//--------------------------------------------------
// Controller Defines
//--------------------------------------------------
#define LC_KEY_CALL		0x0A2
#define LC_MAX_MODES	3
#define LC_MAX_KEYS		256

//-------------------------------------------
//	MOUSE TOKENS
//		BUTTON	ACTION
//	0	left	double-click
//	1	left	pressed 
//	2	left	released
//	3	right	double-click
//	4	right	pressed 
//	5	right	released
//	6	middle	double-click
//	7	middle	pressed 
//	8	middle	released
//	9			moved
//-------------------------------------------
#define LC_ML_DBL		0
#define LC_ML_PRESS		1
#define LC_ML_RELEASE	2
#define LC_MR_DBL		3
#define LC_MR_PRESS		4
#define LC_MR_RELEASE	5
#define LC_MM_DBL		6
#define LC_MM_PRESS		7
#define LC_MM_RELEASE	8
#define LC_M_MOVE		9

//---------------------------------------------
//	LOGIC BLOCK DEFINES
//---------------------------------------------
#define LO_BLOCK		  1
#define LO_WL_N			  2
#define LO_WL_E			  4
#define LO_WL_S			  8
#define LO_WL_W			 16
#define LO_FLOOR		224	// 224 = (32+64+128) [0
#define LO_NOT_SOLID	256

#define LO_GETFLOOR_X	(((X &32)*0.2f) + ((X & 64)*0.4f) + ((X & 128)*0.8f))

//---------------------------------------------
//	MAP DIRECTION DEFINES
//---------------------------------------------
#define MAP_MOVE_NORTH			 1
#define MAP_MOVE_EAST			 2
#define MAP_MOVE_SOUTH			 4
#define MAP_MOVE_WEST			 8
#define MAP_MOVE_UP				16
#define MAP_MOVE_DOWN			32
#define MAP_MOVE_XY             15

#define IF_X_Y_OUT_RANGE	((x < 0) || (x >= m_Size.x) || (y < 0) || (y >= m_Size.y))
#define IF_INSIDE(x,y)		(((UINT)x < _size.x) && ((UINT)y < _size.y))
#define IF_POS_OUT_RANGE	((_pos.x < 0) || (_pos.x >= m_Size.x) || (_pos.y < 0) || (_pos.y >= m_Size.y))
#define IF_DEST_OUT_RANGE	((_dest.x < 0) || (_des.x >= m_Size.x) || (_dest.y < 0) || (_dest.y >= m_Size.y))
#define GRID_XY				m_Grid[x*m_Size.y + y]
#define GRID_PXY			m_Grid[_pos.x*m_Size.y + _pos.y]

//--------------------------------------------------
// Units Structs
//--------------------------------------------------
struct UNIT_CON
{
	UINT m_type;	// Type of condition (poison, slow, haste ect...)
	UINT m_time;	// Time Left ie Duration
	UINT m_amount;	// Damage, percent ect...
};

typedef UNIT_CON UNIT_FLAG;
//---------------------------------------------
// Map Structs
//---------------------------------------------
struct MAP_SIZE
{
	UINT x;
	UINT y;
};

typedef MAP_SIZE UINT_POINT;

struct MAP_POS
{
	UINT x;
	UINT y;
	UINT z;
};

struct MAP_PATH
{
	MAP_POS m_pos;
	MAP_PATH* m_next;
};

struct MAP_CELL
{
	UINT m_height;
	UINT m_tl;		// Texture Data or Logic
	MAP_CELL* m_next; // DELETE
};

struct MAP_OBJ
{
	UINT x;
	UINT y;
	UINT z;
	UINT r;		// Rotation
	UINT id;	// Object ID for OpenGL
};

struct MAP_XYZ_LOGIC
{
	UINT x;
	UINT y;
	UINT z;
	UINT l;		// Logic
};

struct MAP_O_PAL
{
	char m_mesh[50];		// Mesh Used
	char m_tex[50];			// Texture Used
	UINT xSize;
	UINT ySize;
	UINT m_cellCount;
	MAP_XYZ_LOGIC* m_cells;	// Logic Cell Used (FREE)
};

//-----------------------------------------------
//     Network Structs
//-----------------------------------------------
struct JWL_NET_MSG
{
	char  m_Data[CB_MAX_MSG_SIZE];
	UINT  m_Size;
};

struct IP_ADDRESS_STRING
{
	char String[4*4];
};

//---------------------------------------------
// Command Structs
//---------------------------------------------
struct JWL_CMD
{
	UINT m_TeamID;	// ID specifiying Team			(0 Refers to Global Command)
	UINT m_UnitID;	// ID specifiying Unit			(0 Refers to Team Command)
	UINT m_CmdID;	// ID specifying Command Type	(0 Refers to Null Command)
	UINT x;			// X Position (optional)
	UINT y;			// Y Position (optional)
	UINT z;			// Z Position (optional)
	UINT var;		// Holder for any variable Information (optional)
};

struct Q_CMD
{
	JWL_CMD m_cmd;
	Q_CMD* m_next;	// Next Command in Chain
};

//--------------------------------------------------
// Local Controller Structs
//--------------------------------------------------
struct LC_MAPPED_CMD
{
	UINT m_CmdID;
	UINT m_var;
};

//----------------------------------
// Intermediate Image
// Data Structure
//----------------------------------
#define MAGIC_NUM_JPEG 4354

struct INT_IMAGE
{
	UINT x;
	UINT y;
	UINT m_format;
	unsigned char* m_data;
};

/*---------------------------------------------------
				Notes On Light Components
-----------------------------------------------------
GL_AMBIENT
	Holds Ambient Levels created by light (Red,Green,Blue,Alpha)
	Range [-1:1]
	Default (0,0,0,1)

GL_DIFFUSE
	Holds Diffuse Levels created by light (Red,Green,Blue,Alpha)
	Range [-1:1]
	Default for Light 0 is (1.0, 1.0, 1.0, 1.0) all others its (0.0, 0.0, 0.0, 1.0)

GL_SPECULAR
	Holds Specular Levels created by light (Red,Green,Blue,Alpha)
	Range [-1:1]
	Default for Light 0 is (1.0, 1.0, 1.0, 1.0) all others its (0.0, 0.0, 0.0, 1.0)

GL_POSITION
	Holds position vector of light (X,Y,Z,W) W is BOOL
	IF(W == 0)
	 The light is treated as a directional source. Position is Ignored and ATTENUATION is disabled.
	Default (0,0,1,0)
 
GL_SPOT_DIRECTION
	Holds directional vector of light Direction (X,Y,Z,0)
	IF(GL_SPOT_CUTOFF == 180) which it is by default. GL_SPOT_DIRECTION is Ignored.
	Range [-1:1]
	Default (0,0,–1). 
 
GL_SPOT_EXPONENT
	Holds intensity distribution of the light. (V,0,0,0)
	Range [0:128]
	Higher spot exponents result in a more focused light source, regardless of GL_SPOT_CUTOFF
	Default (0) Resulting in uniform Distribution
 
GL_SPOT_CUTOFF
	Holds the Spread of the light source. (V,0,0,0)
	Range [0:90,180]
	Default (180) Resulting in uniform Distribution

GL_CONSTANT_ATTENUATION & GL_LINEAR_ATTENUATION & GL_QUADRATIC_ATTENUATION
	Holds Attention of Light source. (V,0,0,0)
	Range[>0]
	Default Const(1), Linear(0), Quad(0)
----------------------------------------------------*/
struct INT_LIGHT
{
	UINT  m_ID;
	float m_ambient[4];
	float m_diffuse[4];
	float m_specular[4];
	float m_position[4];

	float m_constant_attenuation;
	float m_linear_attenuation;
	float m_quadratic_attenuation;

	float m_spot_direction[3];
	float m_spot_exponent;
	float m_spot_cutoff;
};

#define INT_LIGHT_DEF {0,{0.0f,0.0f,0.0f,1.0f},{1.0f,1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f},{0.0f,0.0f,1.0f,0.0f},1.0f,0.0f,0.0f,{0.0f,0.0f,-1.0f},0.0f,180.0f}


//----------------------------------
// OBJECT STRUCT FORMAT
//----------------------------------
struct OGL_FILE
{
	char m_file[100];
	UINT m_id;
};

struct OGL_OBJ
{
	CVector3	m_Pos;								// Position Vector
	CVector3	m_Up;								// Up Vector
	float		m_Face;								// Facing Vector
	UINT		m_TexID;							// Texture ID
	UINT		m_MeshID;							// Mesh ID
};

struct OGL_UNIT
{
	CVector3	m_Pos;								// Position Vector
	CVector3	m_Up;								// Up Vector
	float		m_Face;								// Facing Vector
	UINT		m_TexID;							// Texture ID
	UINT		m_MeshID;							// Mesh ID

	UINT		m_Team;

	UINT		m_Anim;								// Animation
	UINT		m_Frame;							// Animation Frame
};

#endif