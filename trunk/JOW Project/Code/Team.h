#ifndef JEWEL_O_W_TEAM_BASE_CLASS
#define JEWEL_O_W_TEAM_BASE_CLASS

#include "Structs.h"
#include "DyUnitList.h"

class JWL_Team
{
	//----------------------------------
	//	Initilzation Phase
	//----------------------------------
public:
	JWL_Team();
	~JWL_Team();
	//----------------------------------
	//	Unit Management
	//----------------------------------
	void addUnit(JWL_Unit* _unit);				// Add Unit to List
	bool remUnit(JWL_Unit* _unit);				// Remove Unit from List 
	bool selUnit(JWL_Unit* _unit);				// Select Unit on List
private:
	JWL_DUnitList	m_units;					// Storage of Refrence to Units
	UINT			m_MaxAP;
	UINT			m_UsedAP;
};

/*-----------------------------------------------
+		Explanation of Dynamic Team List		+
-------------------------------------------------
  This is to to explain and hopefully counter
any attempts by some brain dead genuis, such
as myself as three in the morning redesinging
code which has been carefully thought and laid
out on paper. The Dynamic team list was designed
for the following reasons.
  The static team storage method list in main
means all those neat functions that need to loop
through and refrence units can do in a speedy
manner. You can add units to the static list but
never remove them because we track corpses as well
for looting and magic reasons.
  You can't make the team unit list static as its
constantly changing thanks to summon spells and
control spells ect... A team doesn't need to track
its dead units either however.
  So there is no memory management of team units
on this side of the tracks. Okay solved, glad I
could be of aid.
				- A early morning fresh genuis
P.S. Stop reading this and get back to work
-----------------------------------------------*/

#endif