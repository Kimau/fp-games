#ifndef THE_MASSIVLY_AWESOME_CFLOCK_IN3D_34554
#define THE_MASSIVLY_AWESOME_CFLOCK_IN3D_34554

//----------------------------------------------------------
//	Includes
//----------------------------------------------------------
#include "..\Math\CVector3.h"
#include "..\Math\CSurface.h"
//----------------------------------------------------------
//	This is the almighty Boid Flocking Class
//	
//	A basic breakdown is as follows. You wil notice the
//	boid class. These are the elements of the flock.
//	They are stupid in that they are storage classes with
//	little to no functionality.
//	The three basic rules that govern the flock are
//	- Avoidence
//	- Alignment
//	- Attraction
//	Each of these rules is implemented by the appropriate
//	functions
//
//	The flock is updated with a time elapsed. Each boid is
//	then updated as per request
//----------------------------------------------------------
class bBoid3
{
public:
	//----------------------------
	//	Functions
	//----------------------------
	bBoid3() {}
	bBoid3(CVector3 _pos, float _mass): m_Pos(_pos), m_Mass(_mass)	{}
	bBoid3(CVector3 _pos, CVector3 _dir, float _mass): m_Pos(_pos), m_Dir(_dir), m_Mass(_mass)	{}
	~bBoid3()	{}
	//----------------------------
	//	Varibles
	//----------------------------
	CVector3 m_Pos;
	CVector3 m_Dir;
	float m_Mass;
};

class bFlock3
{
	//---------------------------
	//	Construction
	//---------------------------
public:
	bFlock3();
	bFlock3(UINT _size, CVector3 _maxSpace);	
	bFlock3& operator=(const bFlock3& _ref);
	bFlock3(const bFlock3& _ref);
	~bFlock3();
	//---------------------------
	//	Functions
	//---------------------------
private:
	CVector3 Attract(UINT _index, float* _influence);
	CVector3 Avoid(UINT _index, float* _influence);
	CVector3 Align(UINT _index, float* _influence);
	void CalcInfluence(UINT _index, float* _influence);
	void UpdateBoid(UINT _index);
public:
	void Update();
	void Move(float _time);
	void RandomBoid();
	//---------------------------
	//	Setup Functions
	//---------------------------
	void setWrapped(bool _wrap);
	void setMouse(bool _mouse);
	void setAvoid(float _avoid);
	void setAlign(float _align);
	void setAttract(float _attract);
	void setSpeed(float _speed);
	void setMouse(CVector3 _start, CVector3 _dir);
	//---------------------------
	//	Accessors
	//---------------------------
	CVector3 getBoidPos(UINT _index) const;
	float getBoidMass(UINT _index) const;
	CVector3 getBoidDir(UINT _index) const;
	bool isWrapped() const;
	bool isMoused() const;
	float getAvoidence() const;
	float getAlignment() const;
	float getAttraction() const;
	float getSpeed() const;
	UINT getSize() const;

	//---------------------------
	//	Varibles
	//---------------------------
private:
	const UINT m_size;				// Size of flock
	bBoid3* m_boids;				// Storage for the boids
	CVector3 m_maxSpace;			// The farethest point in space they are allowed
	CVector3 m_mouseStart;			// The start point of the Mouse Vector
	CVector3 m_mouseDir;			// The Direction Vector for the mouse
	bool m_wrapped;					// If space is wrapped then you are teleported to other side instead of bounced, false
	bool m_mouse;					// Are they scared of the mouse
	float m_avoid;					// Avoidence Strength. Default 1.0f
	float m_align;					// Alignment Strength. Default 1.0f
	float m_attract;				// Attraction Strength. Default 1.0f
	float m_speed;					// Flock Speed. Default 1.0f
};

#endif