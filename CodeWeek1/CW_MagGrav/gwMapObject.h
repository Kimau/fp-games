#pragma once

//------------------------------------
//	Includes
//------------------------------------
#include "gwVector.h"
#include "gwMagnet.h"

#define	GW_OBJECT		0
#define GW_OBJ_SPAWNER	1
#define GW_OBJ_HOME		2

#define GW_DEF_SIZE	8.0f

//------------------------------------
//	Map Object
//------------------------------------
class gwMapObject
{
public:
	gwMapObject() 
		: m_alive(true), m_size(0.0f) {}
	gwMapObject(gwVector _pos) 
		: m_alive(true), m_size(0.0f), m_pos(_pos) {}
	gwMapObject(gwVector _pos, float _size) 
		: m_alive(true), m_size(_size), m_pos(_pos) {}
	gwMapObject(gwVector _pos, float _size, bool _alive) 
		: m_alive(_alive), m_size(_size), m_pos(_pos) {}
	~gwMapObject() {}

	gwVector m_pos;
	float m_size;	// For Avoidence (neg attracts but only on a VERY local scale)

	virtual void Init() { m_alive = true; }
	bool Alive() const { return m_alive; }

	virtual int Type() const { return GW_OBJECT; }
	virtual void Update(unsigned long _delta) {}

protected:
	bool m_alive;
};

//----------------------------------------
//	Spawner :: Start Point
//----------------------------------------
class gwSpawner : public gwMapObject
{
public:
	gwSpawner() 
		: gwMapObject(gwVector(),GW_DEF_SIZE),
		  m_max(0), m_spawned(0), m_delay(0), m_count(0) 
		 {}
	gwSpawner(gwVector _pos, unsigned int _max, unsigned long _delay)
		: gwMapObject(_pos,GW_DEF_SIZE),
		  m_max(_max), m_spawned(0), m_delay(_delay), m_count(0)
		  {}
	~gwSpawner() {}

	void Init();
	void Update(unsigned long _delta);
	int Type() const { return GW_OBJ_SPAWNER; }

	unsigned int m_max;			// Amount that will be generated
	unsigned int m_spawned;		// How many have been spawned
	unsigned int m_delay;		// Delay between spawns
	unsigned long m_count;		// Count since last spawn
};

//----------------------------------------
//	Home :: End Point
//----------------------------------------
class gwHome : public gwMapObject
{
public:
	gwHome()
		: gwMapObject(gwVector(),-GW_DEF_SIZE),
		  m_max(0), m_housed(0)
		{}
	gwHome(gwVector _pos, unsigned int _max)
		: gwMapObject(_pos,-GW_DEF_SIZE),
		  m_max(_max), m_housed(0)
		{}
	~gwHome() {}

	void Init();
	void Update(unsigned long _delta);
	int Type() const { return GW_OBJ_HOME; }

	unsigned int m_max;			// Amount that will be housed
	unsigned int m_housed;		// How many have arrived
};
