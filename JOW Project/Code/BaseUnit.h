#ifndef BASE_UNITSTRUCTURE_FOR_JEWEL_OF_WORLDS_HOLD_YOUR_BREATH_NOW
#define BASE_UNITSTRUCTURE_FOR_JEWEL_OF_WORLDS_HOLD_YOUR_BREATH_NOW

#include "Structs.h"
#include "FileHandler.h"
#include <fstream>
#include <iostream>
#include "BattleMap.h"

/*------------------------------------
+		Base Unit IE Ghost
+-------------------------------------
Ghost or Base units have all the basic
props of a real unit and the functions 
re used later but for now they are a
basic supplement. Ghosts are only
rendered for debugging pusposes.
------------------------------------*/
class JWL_Unit
{
	friend class JWL_Scenario;
public:
	//----------------------------------
	//	Initilasation
	//----------------------------------
	JWL_Unit();
	void Load(const char* _filename, MAP_OBJ _obj, JWL_Map* _mapRef);
	~JWL_Unit();

	//----------------------------------
	//	Accessors
	//----------------------------------
	UINT getStance() const;
	UINT getFacing() const;
	MAP_POS getPosition() const;
	UINT getAnimation() const;
	UNIT_CON getCondition(UINT _id) const; // 0..9
	UINT getCost() const;
	UINT getController() const;
	UINT getRace() const;
	UINT getGui() const;
	const char* getName() const;
	const char* getMesh() const;
	const char* getTex() const;
	//----------------------------------
	//	Update Functions (ie Passive)
	//----------------------------------
	void Update(UINT _phase);
	//----------------------------------
	//	Movement Functions
	//----------------------------------
	void Stance(UINT _stance);
	void Face(UINT _dir);
	void Move(MAP_POS _dest);
		void Move_BasicWalk(MAP_POS _dest);
	void Climb(UINT _dir, bool _up);
	//----------------------------------
	//	Offensive Functions
	//----------------------------------
	void Attack(JWL_Unit& _target);
	void Fire(MAP_POS _pos);
	void Charge(UINT _dir);
	//----------------------------------
	//	Special Functions
	//----------------------------------
	void Special(MAP_POS _pos, JWL_Unit& _target, UINT _id);
private:
	//---------------------------
	//	Physical Stats
	//---------------------------
	UINT m_hp;				// Units Health Points
	UINT m_bod;				// Max HP
	UINT m_armour;			// Soak
	UINT m_size;			// Unit Size [Minute, Small, Normal, Big, Huge, Massive] 
	UINT m_speed;			// Move Speed
	UINT m_agil;			// Agility for Climbing and Jumping
	UINT m_dodge;			// Dodge Ability
	UINT m_cost;			// Army Point Cost

	//---------------------------
	//	Current Status
	//---------------------------
	UINT m_time;			// Units Time Points: Max of 100 unless modified (eg haste, slow ect...)
	UINT m_morale;			// Units Morale: Max of 100, ALWAYS!
	UINT m_stance;			// Standing, Crouching ect...
	UINT m_face;			// Facing
	MAP_POS m_pos;			// Map Position
	UNIT_CON m_cond[10];	// Conditions (poison, paralysed ect...)

	//---------------------------
	//	Type Information
	//	Stored in flags
	//---------------------------
	char m_name[50];		// Unit Name eg Warrior Footman
	UINT mf_type;			// Unit Type (Warrior Footman, Nature Nymph)
	UINT mf_race;			// Team(s) that unit belongs to (War,Holy ect...)
	UINT mf_update;			// Update Flags
	UINT mf_move;			// Move Type (Foot, Hover, Fly)
	UNIT_CON mf_melee;		// Melee Attack
	UNIT_CON mf_direct;		// Direct, Ranged Attack
	UNIT_CON mf_indirect;	// Indirect Attack
	UINT mf_climb;			// Unit Climb Info
	UNIT_CON mf_abil[10];	// Special Abilities
	//---------------------------
	//	Team Information
	//---------------------------
	UINT m_control;			// Team that controls the unit

	//---------------------------
	//	Renderer Information
	//---------------------------
	UINT m_animation;		// Animation Refrence
	UINT m_guiID;			// Used by Gui to ID
	char m_mesh[50];		// Model Mesh
	char m_skin[50];		// Unit Skin Texture

	//---------------------------
	//	Battle Map Refrence
	//---------------------------
	JWL_Map* m_mapRef;
};

#endif